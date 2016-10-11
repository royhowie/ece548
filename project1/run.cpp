#include "csv_parser.cpp"
#include "dataset.cpp"
#include "tester.cpp"

int main () {
    CSVParser p("data/iris.txt", 4);
    DataSet d(p, 4);

    d.scale();
    d.shuffle();

    Tester(d, 2, 0.2).test(10);
}
