#include "server.h"
#include <thread>
#include <chrono>
#include <atomic>
#include <iostream>

int main() {
    try {
        asio::io_context io_context;

        // Tworzymy serwer na porcie 12345
        TcpServer server(io_context, 12345);
        std::cout << "Serwer uruchomiony na porcie 12345..." << std::endl;

        // Flaga do kontrolowania zakoñczenia pêtli
        std::atomic<bool> running(true);

        // Uruchamiamy io_context w osobnym w¹tku
        std::thread io_thread([&io_context]() {
            io_context.run();
            });

        if (io_thread.joinable()) io_thread.join();

        std::cout << "Serwer zamkniêty." << std::endl;

    }
    catch (std::exception& e) {
        std::cerr << "Wyjatek: " << e.what() << std::endl;
    }

    return 0;
}
