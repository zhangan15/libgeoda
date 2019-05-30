
#ifndef EXAMPLE_H
#define EXAMPLE_H

#include <vector>

class GeoDaProxy {
public:
    GeoDaProxy();

    GeoDaProxy(const char* ds_path);

    virtual ~GeoDaProxy();

protected:
    std::vector<const char*> layer_names;

public:
    const std::vector<const char *> &getLayer_names() const;

    int num_obs;
};

#endif

