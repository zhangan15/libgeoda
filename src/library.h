#ifndef LIBGEODA_LIBRARY_H
#define LIBGEODA_LIBRARY_H

#include <vector>

int Factorial(int n);

void hello();

std::vector<const char*> GetLayerNames(const char *ds_path);

int  CountFeatures(const char *ds_path);

#endif