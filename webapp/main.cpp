#include <iostream>
#include <cstdlib>
#include <windows.h>
#include <thread>

void startServer(void) {
    std::cout << "Start server\n";
    if(system("cd .. && python -m http.server 8000") != 0) {
        std::cout << "\nError trying to start python local server";
    }
}

int main(void) {
    std::thread th_server(startServer);

    Sleep(1000);
    std::cout << "\nOpen webpage\n";
    if (system("start http://localhost:8000/webapp/index.html") != 0) {
        std::cout << "\nError while opening webpage";
    }

    th_server.join();

    return 0;
}