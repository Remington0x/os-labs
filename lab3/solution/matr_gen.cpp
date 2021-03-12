#include <iostream>
#include <fstream>

int main() {
    std::ofstream fout("in.txt");
    int size_y;
    int size_x;

    std::cout << "RAND_MAX = " << RAND_MAX << std::endl;

    std::cout << "Enter size_y and size_x\n";
    std::cin >> size_y >> size_x;

    int srn;
    std::cout << "Enter srand value\n";
    std::cin >> srn;

    srand(srn);

    fout << size_y << '\n';
    fout << size_x << '\n';

    for (int i = 0; i < size_y; ++i) {
        for (int j = 0; j < size_x; ++j) {
            fout << rand() % 100 << ' ';
        }
        fout << '\n';
    }

    fout.close();
}
