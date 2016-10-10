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

    double &operator[] (const std::size_t);

    std::size_t size() const { return data.size(); }

    int get_class_id () { return class_id; }

private:
    int class_id;
    std::vector<double> data;
};

double& AttributeVector::operator[] (const std::size_t index) {
    return data[index];
}
