#pragma once

#include <algorithm>
#include <float.h>
#include <random>
#include <vector>

#include "attribute_vector.cpp"
#include "csv_parser.cpp"

class DataSet {
public:
    std::vector<AttributeVector*> data;
    std::unordered_map<std::string, int> classifications;

    DataSet (CSVParser& p, int attribs) {
        p.parse();

        num_attr = attribs;
        data = p.parsed_data;
        classifications = p.classifications;
        has_scaled = false;
    }

    ~DataSet () {
        for (int i = 0; i < data.size(); i++)
            delete data[i];
        data.clear();
    }

    void scale () {
        // Only scale once.
        if (has_scaled) return;

        has_scaled = true;

        // Loop through each column in the data set.
        for (int i = 0; i < num_attr; i++) {
            double min_val = DBL_MAX;
            double max_val = -DBL_MAX;

            // Find the minimum and maximum values in the given column.
            for (auto row: data) {
                min_val = std::min(min_val, (*row)[i]);
                max_val = std::max(max_val, (*row)[i]);
            }

            // Scale the values to doubles in the interval [0, 1];
            double denom = max_val - min_val;

            // If denom is 0, then the max_val == min_val, so row[i] is the same
            // for all i. Therefore, it should have no influence in
            // classification. `row[i] - min_val` will equal 0, so setting denom
            // to one prevents an error from occurring.
            if (denom == 0) denom = 1;

            // scaled_value = (value - min_val) / (max_val - min_val)
            for (int j = 0; j < data.size(); j++) {
                (*data[j])[i] = ((*data[j])[i] - min_val) / denom;
            }
        }
    }

    // Knuth-Yates Unbiased Shuffle
    void shuffle () {
        std::random_device rd;
        int len = data.size(), t, i;

        while (len > 0) {
            // Pick an index from 0 to len - 1. Post-fix decrement len so that
            // it can be used as an array index for the last element. Also,
            // decrementing is required for maintenance, so that the algorithm
            // only picks an element once.
            int pick_index = rd() % len--;

            // Swap the last element with the element at `pick_index`
            auto target = data[pick_index];
            data[pick_index] = data[len];
            data[len] = target;
        }
    }

private:
    bool has_scaled;
    int num_attr;
};
