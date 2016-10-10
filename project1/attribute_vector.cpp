#include <vector>
#include <string>

class AttributeVector {
public:
    AttributeVector (std::vector<std::string> attributes, int id) {
        class_id = id;
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

    int get_class_id () {
        return class_id;
    }

private:
    std::vector<double> data;
    int class_id;
};
