#pragma once

#include <algorithm>
#include <float.h>
#include <vector>

#include <iostream>

#include "attribute_vector.cpp"
#include "csv_parser.cpp"

class DataSet {
public:
    std::vector<AttributeVector> data;
    // std::vector<AttributeVector> unscaled_data;
    std::unordered_map<std::string, int> classifications;

    DataSet (CSVParser& p, int attribs) {
        num_attr = attribs;

        p.parse();

        for (auto& item: p.parsed_data)
            data.push_back(
                AttributeVector(item.get_data(), item.get_class_id())
            );

        // unscaled_data = std::vector<AttributeVector>(p.parsed_data);
        classifications = p.classifications;
    }

    void scale () {
        // Loop through each column in the data set.
        std::vector<double> minimums;
        std::vector<double> maximums;
        for (int i = 0; i < num_attr; i++) {
            double min_val = DBL_MAX;
            double max_val = -DBL_MAX;

            // Find the minimum and maximum values in the given column.
            for (auto& row: data) {
                min_val = std::min(min_val, row[i]);
                max_val = std::max(max_val, row[i]);
            }

            std::cout << i << ". min=" << min_val << " max=" << max_val << std::endl;
            minimums.push_back(min_val);
            maximums.push_back(max_val);
        }

        // for (auto row: data) {
        for (int j = 0; j < data.size(); j++) {
            std::vector<double> a;
            // std::cout << "test" << std::endl;

            for (int i = 0; i < num_attr; i++) {
                double min_val = minimums[i];
                double max_val = maximums[i];
                double denom = max_val - min_val;
                if (denom == 0)
                    denom = 1;

                data[j][i] = (data[j][i] - min_val) / denom;

                // a.push_back((row[i] - min_val) / denom);
            }

            // scaled_data.push_back(
            //     AttributeVector(a, row.get_class_id())
            // );
        }

            // Scale the values to doubles in the interval [0, 1];
            // double denom = max_val - min_val;

            // If denom is 0, then the max_val == min_val, so row[i] is the same
            // for all i. Therefore, it should have no influence in
            // classification. `row[i] - min_val` will equal 0, so setting denom
            // to one prevents an error from occurring.
            // if (denom == 0) denom = 1;

            // for (int j = 0; j < unscaled_data.size(); j++)
                // scaled_data[j][i] = 0;
                // unscaled_data[j].set(i, 0);
            // for (auto& row: unscaled_data) {
                // row[0] = 0;
            // }
            // data[j][i] = (data[j][i] - min_val) / denom;
        // }
    }
private:
    int num_attr;
};
