//
// Created by Xun Li on 2019-06-05.
//

#ifndef GEODA_ABSTRACTLOCALSA_H
#define GEODA_ABSTRACTLOCALSA_H

#include <list>
#include <wx/string.h>
#include <wx/thread.h>

#include "./geoda/ShapeOperations/GeodaWeight.h"
#include "./geoda/ShapeOperations/GalWeight.h"

class AbstractLocalSA;

class AbstractWorkerThread : public wxThread
{
public:
    AbstractWorkerThread(int obs_start,
                         int obs_end,
                         uint64_t seed_start,
                         AbstractLocalSA* a_coord,
                         wxMutex* worker_list_mutex,
                         wxCondition* worker_list_empty_cond,
                         std::list<wxThread*> *worker_list,
                         int thread_id);
    virtual ~AbstractWorkerThread();
    virtual void* Entry();  // thread execution starts here

    int obs_start;
    int obs_end;
    uint64_t seed_start;
    int thread_id;

    AbstractLocalSA* a_coord;
    wxMutex* worker_list_mutex;
    wxCondition* worker_list_empty_cond;
    std::list<wxThread*> *worker_list;
};

class AbstractLocalSA
{
public:
    AbstractLocalSA();

    virtual ~AbstractLocalSA(){};

    virtual void ComputeLoalSA() = 0;

    virtual void CalcPseudoP();

    virtual void CalcPseudoP_threaded();

    virtual void CalcPseudoP_range(int obs_start, int obs_end, uint64_t seed_start);

    // compare local SA value of current obs to local SA values of random picked nbrs
    virtual bool LargerPermLocalSA(int cnt, std::vector<int>& permNeighbors) = 0;

    virtual void Run();

    virtual void SetSignificanceFilter(int filter_id);
    virtual int GetSignificanceFilter();

    virtual double GetSignificanceCutoff();
    virtual void SetSignificanceCutoff(double val);

    virtual double GetUserCutoff();
    virtual void SetUserCutoff(double val);

    virtual double GetBO();
    virtual void SetBO(double val);

    virtual double GetFDR();
    virtual void SetFDR(double val);

    virtual int GetNumPermutations();
    virtual void SetNumPermutations(int val);

    virtual uint64_t GetLastUsedSeed();
    virtual void SetLastUsedSeed(uint64_t seed);

    virtual bool IsReuseLastSeed();
    virtual void SetReuseLastSeed(bool reuse);

    virtual bool GetHasIsolates();

    virtual bool GetHasUndefined();

    virtual std::vector<std::string> GetDefaultCategories();

    virtual std::vector<double> GetDefaultCutoffs();

    virtual std::vector<double> GetLocalSignificanceValues();

    virtual std::vector<int> GetClusterIndicators();

    virtual std::vector<int> GetSigCatIndicators();

protected:
    int nCPUs;
    int num_obs; // total # obs including neighborless obs

    int significance_filter; // 0: >0.05 1: 0.05, 2: 0.01, 3: 0.001, 4: 0.0001
    int permutations; // any number from 9 to 99999, 99 will be default

    double significance_cutoff; // either 0.05, 0.01, 0.001 or 0.0001
    double bo; //Bonferroni bound
    double fdr; //False Discovery Rate
    double user_sig_cutoff; // user defined cutoff

    bool has_undefined;
    bool has_isolates;
    bool calc_significances; // if false, then p-vals will never be needed
    uint64_t last_seed_used;
    bool reuse_last_seed;

    GalWeight* weights;

    std::vector<double> sig_local_vec;
    std::vector<int> sig_cat_vec;
    std::vector<int> cluster_vec;
};


#endif //GEODA_ABSTRACTLOCALSA_H
