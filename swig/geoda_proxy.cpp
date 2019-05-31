
#include <library.h>

#include "geoda_proxy.h"


GeoDaProxy::GeoDaProxy()
{
    
}

GeoDaProxy::GeoDaProxy(const char* ds_path)
{
    this->layer_names = GetLayerNames(ds_path);
    this->num_obs = CountFeatures(ds_path);
    this->num_obs = 10;
}

GeoDaProxy::~GeoDaProxy() {

}

const std::vector<const char *> &GeoDaProxy::getLayer_names() const {
    return layer_names;
}
