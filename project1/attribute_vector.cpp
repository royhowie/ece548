#pragma once

#include <vector>
#include <string>
#include <stdlib.h>

class AttributeVector {
public:
    AttributeVector (std::vector<std::string> &attributes, int id) {
        class_id = id;
        for (auto& value: attributes) {
            data.push_back(atof(value.c_str()));
        }
    }

    AttributeVector (std::vector<double> &attributes, int id) {
        for (auto value: attributes)
            data.push_back(value);
        class_id = id;
    }

    double &operator[] (const std::size_t);

    // void set (int index, double val) {
    //     data[index] = val;
    // }
    // double get (int index) {
    //     return data[index];
    // }

    std::size_t size() const { return data.size(); }

    int get_class_id () { return class_id; }

    std::vector<double>& get_data () { return data; }

private:
    int class_id;
    std::vector<double> data;
};

double& AttributeVector::operator[] (const std::size_t index) {
    return data[index];
}
