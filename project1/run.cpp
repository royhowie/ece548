#include <iostream>
#include <vector>
#include "csv_parser.cpp"
#include "dataset.cpp"
#include "tester.cpp"

int main (void) {
    // CSVParser p("data/iris.txt", 4);
    CSVParser p("data/iris_linear.txt", 4);
    DataSet d(p, 4);
    // CSVParser p("data/skin.txt", 3, '\t');
    // DataSet d(p, 3);
    // CSVParser p("data/breast_cancer.txt", 30);
    // DataSet d(p, 30);
    // CSVParser p("data/bank_note.txt", 4);
    // DataSet d(p, 4);
    // CSVParser p("data/fertility.txt", 10);
    // DataSet d(p, 10);
    // CSVParser p("data/blood.txt", 4);
    // DataSet d(p, 4);
    // CSVParser p("data/wine.txt", 13);
    // DataSet d(p, 13);


    d.scale();
    d.shuffle();

    int epochs = 50;
    int order = 8;

    std::cout << "order,epoch,error_rate" << std::endl;
    for (int i = 1; i <= order; i++) {
        Tester(d, i, 0.2).test(epochs);
    }

    p.parse();
}
