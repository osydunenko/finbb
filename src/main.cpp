#include <iostream>

#include "options_config.hpp"
#include "net_config.hpp"
#include "logger_config.hpp"
#include "router.hpp"

int main(int argc, char *argv[])
{
    std::uint16_t port_num = 8080;
    std::string address = "127.0.0.1";
    std::uint8_t threads_num = 4;

    po::options_description options{"Allowed options"};
    options.add_options()
        ("help,h", "This help message")
        ("port,p", po::value<decltype(port_num)>(&port_num), "Port number")
        ("address,a", po::value<decltype(address)>(&address), "IP Address i.e. 127.0.0.1")
        ("threads,t", po::value<decltype(threads_num)>(&threads_num), "Threads number");

    try {
        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, options), vm);
        po::notify(vm);

        init_spdlog();

        if (vm.count("help")) {
            std::cerr << options << std::endl;
            return 1;
        }

        http_listener::on_error_type on_error = [](boost::system::error_code ec, boost::string_view v) {
            if (ec == boost::system::errc::address_in_use ||
                ec == boost::system::errc::permission_denied) {
                ioc.stop();
            }
        };

        http_listener::on_accept_type on_accept = [&](http_listener::socket_type socket) {
            SPDLOG_DEBUG("got new connection from {}:{}", 
                    socket.remote_endpoint().address().to_string(),
                    socket.remote_endpoint().port());
            http_session::recv(std::move(socket), global_router::get_instance(), on_error);
        };

        // Start listening
        http_listener::launch(ioc, {ip::make_address(address), port_num}, on_accept, on_error);
        sig_int_term.async_wait([](boost::system::error_code const&, int sig) {
            ioc.stop();
        });

        // Run the I/O Service on the requested number of threads
        std::vector<std::thread> threads;
        threads.reserve(threads_num);
        for (decltype(threads_num) idx = 0; idx < threads_num; idx++)
            threads.emplace_back(std::bind(static_cast<std::size_t (boost::asio::io_context::*)()>
                                           (&boost::asio::io_context::run), std::ref(ioc)));

        for (auto &thr : threads)
            thr.join();

    } catch(std::exception &ex) {
        std::cerr << ex.what() << std::endl;
        return 1;
    }

    return 0;
}
