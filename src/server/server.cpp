#include "server.h"

TcpServer::TcpServer(asio::io_context& io_context, short port)
    : io_context_(io_context),
    acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
    start_accept();
}

bool TcpServer::has_message() const {
    std::lock_guard<std::mutex> lock(messages_mutex_);
    return !messages_.empty();
}

void TcpServer::start_accept() {
    // Tworzymy socket bezpoœrednio na io_context_
    auto socket = std::make_shared<tcp::socket>(io_context_);

    acceptor_.async_accept(*socket,
        [this, socket](std::error_code ec) {
            if (!ec) {
                std::string client = socket->remote_endpoint().address().to_string()
                    + ":" + std::to_string(socket->remote_endpoint().port());
                std::cout << "User " << client << " joined the chat!" << std::endl;

                // Rozpoczynamy asynchroniczne czytanie od klienta
                start_read(socket, client);
            }
            else {
                std::cout << "Blad przy akceptacji polaczenia: " << ec.message() << std::endl;
            }

            // Przyjmij kolejne po³¹czenie
            start_accept();
        });
}

void TcpServer::start_read(std::shared_ptr<tcp::socket> socket, const std::string& client) {
    auto buffer = std::make_shared<asio::streambuf>();

    asio::async_read_until(*socket, *buffer, '\n',
        [this, socket, buffer, client](std::error_code ec, std::size_t /*length*/) {
            if (!ec) {
                std::istream is(buffer.get());
                std::string line;
                std::getline(is, line);

                if (line == "GET") {
                    // Klient ¿¹da wszystkich wiadomoœci
                    std::string reply;
                    {
                        std::lock_guard<std::mutex> lock(messages_mutex_);
                        std::queue<std::pair<std::string, std::string>> temp = messages_;
                        while (!temp.empty()) {
                            auto msg = temp.front();
                            temp.pop();
                            reply += msg.first + ": " + msg.second + "\n";
                        }
                    }

                    // Wysy³amy wiadomoœci do klienta
                    asio::async_write(*socket, asio::buffer(reply),
                        [socket](std::error_code ec, std::size_t /*length*/) {
                            if (ec) {
                                std::cout << "Blad wysylania odpowiedzi: " << ec.message() << std::endl;
                            }
                        });
                }
                else {
                    // Zwyk³a wiadomoœæ - dodajemy do kolejki
                    {
                        std::lock_guard<std::mutex> lock(messages_mutex_);
                        messages_.push({ client, line });
                    }
                    std::cout << "Otrzymano od " << client << ": " << line << std::endl;
                }

                // Kontynuujemy czytanie kolejnych wiadomoœci
                start_read(socket, client);
            }
            else {
                std::cout << "Polaczenie z " << client << " zamkniete: " << ec.message() << std::endl;
            }
        });
}
