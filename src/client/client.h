#pragma once
#define ASIO_STANDALONE
#include <asio.hpp>
#include <iostream>
#include <string>

using asio::ip::tcp;

class TcpClient {
public:
    TcpClient(asio::io_context& io_context, const std::string& host, short port);

    void send_message(const std::string& message);

    void get_messages();

private:
    tcp::socket socket_;
};
