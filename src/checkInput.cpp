#include "checkInput.h"

std::string correctInput1() {

    std::string input1;

    while (true) {
        std::cout << "������� �������������� �����: ";
        std::cin >> input1;

        if ((input1[0] >= '0' && input1[0] <= '2') && (input1[1] >= '0' && input1[1] <= '4') &&
            (input1[2] >= '0' && input1[2] <= '2') && (input1[3] >= '0' && input1[3] <= '3'))
            return input1;

        std::cout << "�������� ����! ���������� �����." << std::endl;

    }

}

int correctInput2() {

    int number;

    while (true) {
        std::cout << "������� ����� �� 1 �� 24: ";
        std::cin >> number;

        if (std::cin.fail() || number < 1 || number > 24) {
            std::cout << "�������� ����! ���������� �����." << std::endl;

            std::cin.clear();
            std::cin.ignore(9999, '\n');
        }
        else {
            return number;
        }
    }

}