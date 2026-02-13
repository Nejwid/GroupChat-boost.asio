#pragma once

#define ASIO_STANDALONE
#include <asio.hpp>
#include <iostream>
#include <queue>
#include <string>
#include <mutex>
#include <memory>

using asio::ip::tcp;

class TcpServer {
public:
    TcpServer(asio::io_context& io_context, short port);

    // Sprawdza, czy s¹ jakieœ wiadomoœci w kolejce
    bool has_message() const;

private:
    void start_accept();
    void start_read(std::shared_ptr<tcp::socket> socket, const std::string& client);

private:
    asio::io_context& io_context_;  // REFERENCJA do io_context
    tcp::acceptor acceptor_;
    mutable std::mutex messages_mutex_;
    std::queue<std::pair<std::string, std::string>> messages_;
};
