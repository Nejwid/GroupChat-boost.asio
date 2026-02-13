#include "client.h"
#include <iostream>
#include <string>

int main() {
    asio::io_context io_context;
    TcpClient client(io_context, "tcp_server", 12345);

    std::string line;
    while (true) {
        std::cout << "Wpisz wiadomoœæ (GET aby pobraæ, QUIT aby wyjœæ): ";
        std::getline(std::cin, line);

        if (line == "QUIT") break;

        if (line == "GET") {
            client.get_messages();
        }
        else {
            client.send_message(line);
        }
    }
    return 0;
}

