%module geoda

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
    #include "../src/library.h"
%}

#include <vector>

%include <../src/library.h>
