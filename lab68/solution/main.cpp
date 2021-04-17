/*
Max has variant 11
    Topology 1
    Command type 4 (поиск подстроки в строке)
        Формат команды:
        > exec id
        > text_string
        > pattern_string
        [result] – номера позиций, где найден образец, разделенный точкой с запятой
        text_string — текст, в котором искать образец. Алфавит: [A-Za-z0-9]. Максимальная длина строки
        108 символов
        pattern_string — образец
        Пример:
        > exec 10
        > abracadabra
        > abra
        Ok:10:0;7
        > exec 10
        > abracadabra
        > mmm
        Ok:10: -1
        Примечания: Выбор алгоритма поиска не важен

    Knot accessibility check 2
        Формат команды: ping id
        Команда проверяет доступность конкретного узла. Если узла нет, то необходимо выводить
        ошибку: «Error: Not found»
        Пример:
        > ping 10
        Ok: 1 // узел 10 доступен
        > ping 17
        Ok: 0 // узел 17 недоступен

My variant is 34
    T 3
        Все вычислительные узлы хранятся в бинарном дереве поиска. [parent] — является
        необязательным параметром.
    C 4
    K 1
        Формат команды: pingall
        Вывод всех недоступных узлов вывести разделенные через точку запятую.
        Пример:
        > pingall
        Ok: -1 // Все узлы доступны
        > pingall
        Ok: 7;10;15 // узлы 7, 10, 15 — недоступны
        Команда проверки 2
        Формат команды: ping id
        Команда проверяет доступность конкретного узла. Если узла нет, то необходимо выводить
        ошибку: «Error: Not found»
        Пример:
        > ping 10
        Ok: 1 // узел 10 доступен
        > ping 17
        Ok: 0 // узел 17 недоступен

*/

#include <iostream>
#include "topology.hpp"

int main() {
    std::shared_ptr< topology_t<int> > Item(new topology_t<int>());
    int q;
    std::cout << "Enter command:\n1 -- add node\n2 -- del node\n3 -- print tree\n";
    int key;
    while (scanf("%d", &q) > 0) {
        switch (q) {
            case 1:
                std::cout << "Enter key for new node\n";
                std::cin >> key;
                Item->add(key);
                break;
            case 2:
                std::cout << "Enter key for node to delete\n";
                std::cin >> key;
                Item->rm(key);
                break;
            case 3:
                Item->print();
                break;
            default:
                std::cout << "No command found\n";
                break;
        }
        std::cout << "Enter command:\n1 -- add node\n2 -- del node\n3 -- print tree\n";
    }




























    return 0;
}
