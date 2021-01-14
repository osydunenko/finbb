#pragma once

#include "net_config.hpp"
#include "logger_config.hpp"

struct logging_handler 
{
    static bool handle(const beast_http_request &req, http_context &, const std::smatch &)
    {
        const auto method = req.method_string();
        SPDLOG_INFO("{} {}", 
            std::string_view(method.data(), method.size()), 
            req.target().to_string()
        );
        return true;
    };
}; 

