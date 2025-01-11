// web-chat_server.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

int main()
{
    setlocale(LC_ALL, "Rus");
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "ОШибка инициализации Winsock" << std::endl;
        return 1;
    }

    //создание серверного сокета
    SOCKET listensocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listensocket == INVALID_SOCKET) {
        std::cerr << "Ошибка создания сокета" << std::endl;
        WSACleanup();
        return 1;
    }

    //настройка адреса и порта сервера
    sockaddr_in serverAddr; //тип данных который используется дл хранения адреса сокета 
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    //привязка сокета к адресу
    if (bind(listensocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "ОШибка привязки сокета" << std::endl;
        closesocket(listensocket);
        WSACleanup();
        return 1;
    }

    if (listen(listensocket, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "Ошбика при запуске прослушивания" << std::endl;
        closesocket(listensocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Сервер запущен и слушает порт 8080..." << std::endl;

     //ожидаение подключения клиента
    sockaddr_in clientAddr;
    int clientAddrSize = sizeof(clientAddr);
    SOCKET clientSocket = accept(listensocket, (sockaddr*)&clientAddr, &clientAddrSize);
    if (clientSocket == INVALID_SOCKET) {
        std::cerr << "Ошибка принятия подключения" << std::endl;
        closesocket(listensocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Клиент подключился!" << std::endl;

    //обработка данных от клиента
    char buffer[1024];
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        /*std::cout << "Сообщение от клиента: " << buffer << std::endl;

        //отправляем ответ клиенту
        std::string response = "Сообщение принято: " + std::string(buffer);*/

        std::string response = "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: 20\r\n"
            "\r\n"
            "<h1>Hello, World!</h1>";

        send(clientSocket, response.c_str(), response.size(), 0);
    }

    closesocket(clientSocket);
    closesocket(listensocket);
    WSACleanup();

    std::cout << "Сервер завершил работу" << std::endl;
    return 0;

}


