#include <iostream>
#include <vector>
#include "csv_parser.cpp"
#include "dataset.cpp"
#include "tester.cpp"

int main () {
    CSVParser p("data/iris.txt", 4);
    // CSVParser p("data/skin.txt", 3);
    DataSet d(p, 3);

    d.scale();
    d.shuffle();

    int epochs = 50;
    int order = 5;

    for (int i = 1; i <= order; i++) {
        std::cout << "order=" << i << std::endl;
        Tester(d, i, 0.2).test(epochs);
    }
}
