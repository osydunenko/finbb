#pragma once

#include "net_config.hpp"

class global_router: public http_router
{
public:
    using self_type = global_router;

    using router_type = http_router;

    static global_router &get_instance();

    global_router(const global_router &) = delete;
    void operator=(const global_router &) = delete;

private:
    global_router();
};

