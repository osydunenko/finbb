#include "router.hpp"
#include "handlers/logging_handler.hpp"

global_router::global_router()
{
    get(R"(^/.*$)", logging_handler::handle);
}

global_router &global_router::get_instance()
{
    static global_router router;
    return router;
}

