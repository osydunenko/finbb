#pragma once

#include <thread>

#include <boost/asio/signal_set.hpp>
#include <boost/asio/ip/address.hpp>
#include <boost/beast/http/message.hpp>

#include "beast-router/listener.hpp"
#include "beast-router/session.hpp"
#include "beast-router/router.hpp"
#include "beast-router/base/lockable.hpp"

using http_listener = server::default_listener;
using http_session = server::default_session;
using http_router = server::router<http_session>;

using http_context = typename http_session::context_type;
using beast_http_request = typename http_session::request_type;
using beast_http_response = boost::beast::http::response<boost::beast::http::string_body>;

namespace ip = boost::asio::ip;

static boost::asio::io_context ioc;
static boost::asio::signal_set sig_int_term{ioc, SIGINT, SIGTERM};


