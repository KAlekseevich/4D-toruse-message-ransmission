#include <iostream>
#include "Network.h"
#include "windows.h"
#include "checkInput.h"

int main() {

    SetConsoleOutputCP(1251);
    // создаем суперкомпьютерную сеть с топологией 4D тора
    Network network(3, 5, 3, 4);

    while (true) {
        std::cout << "\nМеню:\n";
        std::cout << "1. Отправить сообщение\n";
        std::cout << "2. Выйти\n";
        std::cout << "Выберите действие: ";

        int choice;
        std::cin >> choice;

        if (choice == 1) {

            std::cout << "Введите адрес исотчника, длину сообщения и адрес получателя в формате xxxx-y-xxxx:" << std::endl;

            std::string Str1 = correctInput1();
            int Length = correctInput2();
            std::string Str2 = correctInput1();

            Node* s = network.getNode(Str1[0] - '0', Str1[1] - '0', Str1[2] - '0', Str1[3] - '0');
            Node* r = network.getNode(Str2[0] - '0', Str2[1] - '0', Str2[2] - '0', Str2[3] - '0');

            network.sendMessage(network, s, Length, r); 
        }
        else if (choice == 2) {
            std::cout << "Выход из программы." << std::endl;
            break;  
        }
        else {
            std::cout << "Неверный выбор, попробуйте снова." << std::endl;
        }
    }

    return 0;
}
