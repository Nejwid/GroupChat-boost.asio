#include "client.h"

TcpClient::TcpClient(asio::io_context& io_context, const std::string& host, short port)
    : socket_(io_context) {
    tcp::resolver resolver(io_context);
    auto endpoints = resolver.resolve(host, std::to_string(port));
    asio::connect(socket_, endpoints);
    std::cout << "Polaczono z serwerem " << host << ":" << port << std::endl;
}

void TcpClient::send_message(const std::string& message) {
    std::string msg_with_newline = message + "\n"; // Asio read_until oczekuje '\n'
    asio::write(socket_, asio::buffer(msg_with_newline));
}

void TcpClient::get_messages() {
    send_message("GET");

    asio::streambuf response;
    std::error_code ec;
    while (true) {
        std::size_t n = asio::read_until(socket_, response, '\n', ec);
        if (ec && ec != asio::error::eof) {
            std::cerr << "Blad podczas odczytu: " << ec.message() << std::endl;
            break;
        }

        std::istream is(&response);
        std::string line;
        while (std::getline(is, line)) {
            if (!line.empty()) {
                std::cout << line << std::endl;
            }
        }

        if (ec == asio::error::eof) break;
        if (n == 0) break;
    }
}
