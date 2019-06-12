%module libgeoda

%include "stl.i"
%include "std_string.i"
%include "std_vector.i"

namespace std {
        %template(VecVoid) vector<void*>;
        %template(VecFloat) vector<float>;
        %template(VecString) vector<string>;
        %template(VecDouble) vector<double>;
        %template(VecChar) vector<char>;
        %template(VecVecDouble) vector< vector<double> >;
        %template(VecInt) vector<int>;
        %template(VecVecInt) vector< vector<int> >;
        %template(VecUINT8) vector<unsigned char>;
        %template(VecVecUINT8) vector<vector<unsigned char> >;
        %template(VecVecChar) vector<vector<char> >;
}

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

namespace std {
        %template(VecGeoDaColumn) vector<GeoDaColumn*>;
        %template(VecRaw) vector<unsigned char*>;
}
