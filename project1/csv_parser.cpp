#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <stdlib.h>

class AttributeVector {
public:
    AttributeVector (std::vector<std::string> attributes, bool c) {
        is_class = c;
        for (auto const& value: attributes) {
            data.push_back(std::atof(value.c_str()));
        }
    }
    double const& operator [] (std::size_t index) const {
        return data[index];
    }
    std::size_t size() const {
        return data.size();
    }
private:
    std::vector<double> data;
    bool is_class;
};

class CSVParser {
public:
    CSVParser (std::string file_name, int attrib) {
        csv_data.open(file_name);
        attributes = attrib;
    }

    std::vector<AttributeVector> parse (std::string class_name) {
        std::string line;
        std::vector<AttributeVector> parsed_data;

        while (std::getline(csv_data, line)) {
            std::vector<std::string> line_data;
            std::stringstream line_stream(line);
            std::string cell;

            for (int i = 0; i < attributes; i++) {
                std::getline(line_stream, cell, ',');
                line_data.push_back(cell);
            }

            // check whether the last entry is the correct class
            std::getline(line_stream, cell, ',');

            AttributeVector v (line_data, 0 == class_name.compare(cell));

            parsed_data.push_back(v);
        }

        return parsed_data;
    }
private:
    std::ifstream csv_data;
    int attributes;
};
