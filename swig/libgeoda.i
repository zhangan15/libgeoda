%module libgeoda

%include "stl.i"
%include "std_string.i"
%include "std_vector.i"

namespace std {
  %template(VecFloat) vector<float>;
  %template(VecString) vector<string>;
  %template(VecDouble) vector<double>;
  %template(VecVecDouble) vector< vector<double> >;
  %template(VecInt) vector<int>;
  %template(VecVecInt) vector< vector<int> >;
  %template(VecUINT8) vector<unsigned char>;
  %template(VecVecUINT8) vector<vector<unsigned char> >;
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
