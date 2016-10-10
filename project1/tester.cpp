#include <iostream>
#include "dataset.cpp"
#include <vector>
// #include "perceptron.cpp"

int main () {
    CSVParser p("data/bezdekIris.data.txt", 4);
    DataSet d(p, 4);

    auto data = d.data;

    // std::cout << "size: " << data.size() << std::endl;
    std::cout << "items before scaling:" << std::endl;

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 4; j++) {
            std::cout << (*data[i])[j] << "\t";
            // data[i][j] = 0;
        }
        std::cout << std::endl;
    }

    std::cout << "items after scaling:" << std::endl;

    d.scale();

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 4; j++)
            std::cout << (*data[i])[j] << "\t";
        std::cout << std::endl;
    }
}
