%module libgeoda

%include "stl.i"
%include "std_string.i"
%include "std_vector.i"

%template(VecVoid) std::vector<void*>;
%template(VecFloat) std::vector<float>;
%template(VecString) std::vector<std::string>;
%template(VecDouble) std::vector<double>;
%template(VecChar) std::vector<char>;
%template(VecVecDouble) std::vector<std::vector<double> >;
%template(VecInt) std::vector<int>;
%template(VecVecInt) std::vector<std::vector<int> >;
%template(VecUINT8) std::vector<unsigned char>;
%template(VecVecUINT8) std::vector<std::vector<unsigned char> >;
%template(VecVecChar) std::vector<std::vector<char> >;


#if defined(SWIGR)
%feature("novaluewrapper") std::vector<std::vector<unsigned char>>;
%template() std::vector<std::vector<unsigned char>>;
%typemap(out) std::vector<std::vector<unsigned char>>
{
    int sz = $1.size();
    SEXP vec = Rf_allocVector(VECSXP, sz);
    for(int i = 0; i <sz; ++i)
    {
        int subLength = $1[i].size();
        SEXP raws = Rf_allocVector(RAWSXP, subLength);
        Rbyte* p_raws = RAW(raws);
        for (int j=0; j < subLength; ++j)
        {
            p_raws[j] = $1[i][j];
        }
        SET_VECTOR_ELT(vec, i, raws);
    }
    //UNPROTECT(sz+1);
    $result = vec;
}
%typemap(scoerceout) std::vector<std::vector<unsigned char>>
%{ # override SWIG generate return an unkonwn type %}

#elif defined(SWIGPYTHON)

#endif

%{
    #include <GeodaWeight.h>
    #include <AbstractLocalSA.h>
    #include <UniLisa.h>
    #include <libgeoda.h>
%}

%include <GeodaWeight.h>
%include <AbstractLocalSA.h>
%include <UniLisa.h>
%include <libgeoda.h>

%template(VecGeoDaColumn) std::vector<GeoDaColumn*>;

%inline %{                                   // Example code.
std::vector<std::vector<unsigned char> > func()
{
    std::vector<std::vector<unsigned char> > vv;
    std::vector<unsigned char> v;
    v.push_back((unsigned char)1);
    v.push_back((unsigned char)2);
    v.push_back((unsigned char)3);
    vv.push_back(v);
    v.clear();
    v.push_back((unsigned char)4);
    v.push_back((unsigned char)5);
    vv.push_back(v);
    return vv;
}
%}