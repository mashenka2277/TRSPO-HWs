#pragma comment(lib, "ws2_32.lib")
#include <iostream>
#include <string>
#include <winsock2.h> // Ліба для використання WinSock2

int main()
{
    // Завантажуємо необхідну версію бібліотеки
    WSAData wsaData;
    WORD DLLVersion = MAKEWORD(2, 1); 
    // Перевірка: чи завантажилась бібліотека 
    if(WSAStartup(DLLVersion, &wsaData) != 0)
    {
        std::cout << "Error: failed to initialize WinSock." << std::endl;
    }

    // Заповнюємо інформацію про адресу сокета
    SOCKADDR_IN addr; // Структура для зберігання адреси 
    int sizeofaddr = sizeof(addr);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // "127.0.0.1" - localhost
    addr.sin_port = htons(1111); 
    addr.sin_family = AF_INET; 

    SOCKET Connection = socket(AF_INET, SOCK_STREAM, 0); // Створюємо новий сокет для з'єднання з сервером

    // Спроба під'єднання до серверу
    if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0)
    {
        std::cout << "Error: failed connect to server.\n";
        return 1;
    } 
    std::cout << "Connected to server." << std::endl;

    // Обмін повідомленнями
    char buffer[1024];
    while (true)
    {
        // Відправка повідомлення серверу
        std::string msg;
        std::cout << "Your message to Server: ";
        std::getline(std::cin, msg);

        if (msg.empty())  
        {
            std::cout << "Exiting...\n";
            closesocket(Connection);  // Клієнт закриває сокет
        }

        send(Connection, msg.c_str(), msg.size(), 0);

        // Прийом відповіді від сервера
        int read_ans = recv(Connection, buffer, sizeof(buffer) - 1, 0);

        if (read_ans <= 0)
        {
            std::cout << "The connection to the server is closed.\n";
            break;
        }

        buffer[read_ans] = '\0'; // Завершення рядка
        std::cout << "Server: " << buffer << std::endl;
    }

    // Закриття сокета
    WSACleanup();

    return 0;
}