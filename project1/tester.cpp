#include <iostream>
#include <vector>
#include <limits.h>

#include "dataset.cpp"
#include "perceptron.cpp"

class Tester {
public:
    Tester (DataSet& ds, int ord, double learning_factor) {
        data_set = &ds;
        order = ord;
        int attrib = data_set->get_num_attr();

        for (int i = 0; i < data_set->num_classes(); i++) {
            machines.push_back(
                new Perceptron(attrib, order, learning_factor)
            );
        }
    }

    ~Tester () {
        for (int i = 0; i < machines.size(); i++)
            delete machines[i];
        machines.clear();
    }

    void test (int epochs) {
        int size = data_set->data.size();
        int bound = size / 2;

        std::cout<<"epoch#,correct,incorrect,error_rate" << std::endl;
        // Each perceptron will be exposed to `epochs` and tested after each.
        for (int i = 0; i < epochs; i++) {
            std::cout << i << ",";
            // Learn on the first half of the data.
            for (int j = 0; j < bound; j++) {
                for (int k = 0; k < machines.size(); k++) {
                    auto v = (data_set->data)[j];
                    machines[k]->learn(v, v->get_class_id() == k);
                }
            }

            // Test on the second half of the data.
            int correct = 0;
            int incorrect = 0;
            for (int j = bound; j < size; j++) {
                int max_sum = INT_MIN;
                int max_index = 0;

                AttributeVector* v = data_set->data[j];

                for (int k = 0; k < machines.size(); k++) {
                    double result = machines[k]->test(v);

                    if (result >= max_sum) {
                        max_sum = result;
                        max_index = k;
                    }
                }

                if (max_index == v->get_class_id())
                    correct++;
                else
                    incorrect++;
            }

            double error_rate = double(incorrect) / (correct + incorrect);

            std::cout << correct << ",";
            std::cout << incorrect << ",";
            std::cout << error_rate << std::endl;
        }
    }
private:
    int order;
    DataSet* data_set;
    std::vector<Perceptron*> machines;
};
