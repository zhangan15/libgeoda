//
// Created by Xun Li on 2019-06-05.
//

#include <math.h>

#include "./geoda/GeoDaSet.h"
#include "./geoda/GenUtils.h"

#include "AbstractLocalSA.h"

AbstractWorkerThread::AbstractWorkerThread(int obs_start_s,
                                           int obs_end_s,
                                           uint64_t	seed_start_s,
                                           AbstractLocalSA* a_coord_s,
                                           wxMutex* worker_list_mutex_s,
                                           wxCondition* worker_list_empty_cond_s,
                                           std::list<wxThread*> *worker_list_s,
                                           int thread_id_s)
        : wxThread(),
          obs_start(obs_start_s),
          obs_end(obs_end_s),
          seed_start(seed_start_s),
          a_coord(a_coord_s),
          worker_list_mutex(worker_list_mutex_s),
          worker_list_empty_cond(worker_list_empty_cond_s),
          worker_list(worker_list_s),
          thread_id(thread_id_s)
{
}

AbstractWorkerThread::~AbstractWorkerThread()
{
}

wxThread::ExitCode AbstractWorkerThread::Entry()
{
    // call work for assigned range of observations
    a_coord->CalcPseudoP_range(obs_start, obs_end, seed_start);
    wxMutexLocker lock(*worker_list_mutex);
    // remove ourself from the list
    worker_list->remove(this);
    // if empty, signal on empty condition since only main thread
    // should be waiting on this condition
    if (worker_list->empty()) {
        worker_list_empty_cond->Signal();
    }
    return NULL;
}


AbstractLocalSA::AbstractLocalSA()
: nCPUs(8), calc_significances(true)
{

}

void AbstractLocalSA::SetSignificanceFilter(int filter_id)
{
    if (filter_id == -1) {
        // user input cutoff
        significance_filter = filter_id;
        return;
    }
    // 0: >0.05 1: 0.05, 2: 0.01, 3: 0.001, 4: 0.0001
    if (filter_id < 1 || filter_id > 4) return;
    significance_filter = filter_id;
    if (filter_id == 1) significance_cutoff = 0.05;
    if (filter_id == 2) significance_cutoff = 0.01;
    if (filter_id == 3) significance_cutoff = 0.001;
    if (filter_id == 4) significance_cutoff = 0.0001;
}

int AbstractLocalSA::GetSignificanceFilter()
{
    return significance_filter;
}

double AbstractLocalSA::GetSignificanceCutoff()
{
    return significance_cutoff;
}

void AbstractLocalSA::SetSignificanceCutoff(double val)
{
    significance_cutoff = val;
}

double AbstractLocalSA::GetUserCutoff()
{
    return user_sig_cutoff;
}

void AbstractLocalSA::SetUserCutoff(double val)
{
    user_sig_cutoff = val;
}

double AbstractLocalSA::GetFDR()
{
    return fdr;
}
void AbstractLocalSA::SetFDR(double val)
{
    fdr = val;
}

double AbstractLocalSA::GetBO()
{
    return bo;
}
void AbstractLocalSA::SetBO(double val)
{
    bo = val;
}

int AbstractLocalSA::GetNumPermutations()
{
    return permutations;
}
void AbstractLocalSA::SetNumPermutations(int val)
{
    permutations = val;
}

uint64_t AbstractLocalSA::GetLastUsedSeed()
{
    return last_seed_used;
}

bool AbstractLocalSA::IsReuseLastSeed()
{
    return reuse_last_seed;
}

void AbstractLocalSA::SetLastUsedSeed(uint64_t seed)
{
    reuse_last_seed = true;
    last_seed_used = seed;
}

bool AbstractLocalSA::GetHasIsolates()
{
    return has_isolates;
}

bool AbstractLocalSA::GetHasUndefined()
{
    return has_undefined;

}

void AbstractLocalSA::CalcPseudoP()
{
    if (!calc_significances) return;
    CalcPseudoP_threaded();
}

void AbstractLocalSA::CalcPseudoP_threaded()
{
    if (nCPUs < 1) nCPUs = wxThread::GetCPUCount();

    // mutext protects access to the worker_list
    wxMutex worker_list_mutex;
    // signals that worker_list is empty
    wxCondition worker_list_empty_cond(worker_list_mutex);
    // mutex should be initially locked
    worker_list_mutex.Lock();

    // List of all the threads currently alive.  As soon as the thread
    // terminates, it removes itself from the list.
    std::list<wxThread*> worker_list;

    // divide up work according to number of observations
    // and number of CPUs
    int work_chunk = num_obs / nCPUs;

    if (work_chunk == 0) work_chunk = 1;

    int obs_start = 0;
    int obs_end = obs_start + work_chunk;

    bool is_thread_error = false;
    int quotient = num_obs / nCPUs;
    int remainder = num_obs % nCPUs;
    int tot_threads = (quotient > 0) ? nCPUs : remainder;

    //boost::thread_group threadPool;

    if (!reuse_last_seed) last_seed_used = time(0);

    for (int i=0; i<tot_threads && !is_thread_error; i++) {
        int a=0;
        int b=0;
        if (i < remainder) {
            a = i*(quotient+1);
            b = a+quotient;
        } else {
            a = remainder*(quotient+1) + (i-remainder)*quotient;
            b = a+quotient-1;
        }
        uint64_t seed_start = last_seed_used+a;
        uint64_t seed_end = seed_start + ((uint64_t) (b-a));
        int thread_id = i+1;
        //boost::thread* worker = new boost::thread(boost::bind(&AbstractCoordinator::CalcPseudoP_range,this, a, b, seed_start));
        //threadPool.add_thread(worker);
        AbstractWorkerThread* thread =
                new AbstractWorkerThread(a, b, seed_start, this,
                                         &worker_list_mutex,
                                         &worker_list_empty_cond,
                                         &worker_list, thread_id);
        if ( thread->Create() != wxTHREAD_NO_ERROR ) {
            delete thread;
            is_thread_error = true;
        } else {
            worker_list.push_front(thread);
        }
    }
    if (is_thread_error) {
        // fall back to single thread calculation mode
        CalcPseudoP_range(0, num_obs-1, last_seed_used);
    } else {
        std::list<wxThread*>::iterator it;
        for (it = worker_list.begin(); it != worker_list.end(); it++) {
            (*it)->Run();
        }
        while (!worker_list.empty()) {
            // wait until thread_list might be empty
            worker_list_empty_cond.Wait();
            // We have been woken up. If this was not a false
            // alarm (sprious signal), the loop will exit.
        }
    }
    //threadPool.join_all();
}

void AbstractLocalSA::CalcPseudoP_range(int obs_start, int obs_end, uint64_t seed_start)
{
    GeoDaSet workPermutation(num_obs);
    int max_rand = num_obs-1;

    GalElement* w = weights->gal;

    for (size_t cnt=obs_start; cnt<=obs_end; cnt++) {

        // get full neighbors even if has undefined value
        int numNeighbors = 0;
        if (w[cnt].Size() > numNeighbors) {
            numNeighbors = w[cnt].Size();
        }
        int _sigCat = 0;
        if (w[cnt].Size() == 0) {
            _sigCat = 5;
        }

        if (numNeighbors == 0) {
            // isolate: don't do permutation
            continue;
        }

        uint64_t countLarger = 0;

        for (size_t perm=0; perm<permutations; perm++) {
            int rand=0;
            double rng_val;
            int newRandom;
            while (rand < numNeighbors) {
                // computing 'perfect' permutation of given size
                rng_val = Gda::ThomasWangHashDouble(seed_start++) * max_rand;
                // round is needed to fix issue
                // https://github.com/GeoDaCenter/geoda/issues/488
                newRandom = (int)(rng_val<0.0?ceil(rng_val - 0.5):floor(rng_val + 0.5));

                if (newRandom != cnt && !workPermutation.Belongs(newRandom) && w[newRandom].Size()>0) {
                    workPermutation.Push(newRandom);
                    rand++;
                }
            }
            std::vector<int> permNeighbors(numNeighbors);
            for (int cp=0; cp<numNeighbors; cp++) {
                permNeighbors[cp] = workPermutation.Pop();
            }

            if (LargerPermLocalSA(cnt, permNeighbors)) {
                countLarger += 1;
            }
        }

        // pick the smallest counts
        if (permutations-countLarger <= countLarger) {
            countLarger = permutations-countLarger;
        }

        double _sigLocal = (countLarger+1.0)/(permutations+1);

        // 'significance' of local Moran
        if (_sigLocal <= 0.0001) _sigCat = 4;
        else if (_sigLocal <= 0.001) _sigCat = 3;
        else if (_sigLocal <= 0.01) _sigCat = 2;
        else if (_sigLocal <= 0.05) _sigCat = 1;
        else _sigCat = 0;

        // observations with no neighbors get marked as isolates
        // NOTE: undefined should be marked as well, however, since undefined_cat has covered undefined category, we don't need to handle here
    }
}


std::vector<std::string> AbstractLocalSA::GetDefaultCategories()
{
    std::vector<std::string> cats;
    cats.push_back("p = 0.05");
    cats.push_back("p = 0.01");
    cats.push_back("p = 0.001");
    cats.push_back("p = 0.0001");
    return cats;
}

std::vector<double> AbstractLocalSA::GetDefaultCutoffs()
{
    std::vector<double> cutoffs;
    cutoffs.push_back(0.05);
    cutoffs.push_back(0.01);
    cutoffs.push_back(0.001);
    cutoffs.push_back(0.0001);
    return cutoffs;
}

std::vector<double> AbstractLocalSA::GetLocalSignificanceValues()
{
    return sig_local_vec;
}

std::vector<int> AbstractLocalSA::GetClusterIndicators()
{
    return cluster_vec;
}

std::vector<int> AbstractLocalSA::GetSigCatIndicators()
{
    return sig_cat_vec;
}