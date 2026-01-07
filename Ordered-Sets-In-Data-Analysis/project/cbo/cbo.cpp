#include <functional>
#include <ranges>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <span>
#include <set>
#include <algorithm>
#include <string>
#include <stdexcept>
#include <utility>

std::vector<std::string> ReadFeatureNames(std::ifstream& datafile)
{
    std::string line;
    std::getline(datafile, line);
    
    std::istringstream splitter(line);
    std::string feature;
    std::vector<std::string> features;
    while (std::getline(splitter, feature, ','))
    {
        features.emplace_back(std::move(feature));
    }
    return features;
}

std::vector<bool> ReadObject(std::string line)
{
    std::vector<bool> result;
    
    std::string value;
    std::istringstream splitter(line);
    while (std::getline(splitter, value, ','))
    {
        if (value == "1") result.push_back(true);
        else if (value == "0") result.push_back(false);
        else throw std::runtime_error("Bad input");
    }
    return result;
}

class FCA
{
public:
    FCA(std::vector<std::string> features, std::vector<std::vector<bool>> data)
        : features_(std::move(features)), data_(std::move(data)) {}

    using Objects = std::vector<std::size_t>;
    using Features = std::vector<std::size_t>;

    Features ObjectsPrime(const Objects& objects)
    {
        auto features = (
            std::views::iota(std::size_t(0), features_.size()) |
            std::views::filter([&](const std::size_t feature) {
                return std::ranges::all_of(objects, [&](const std::size_t object) { return data_[object][feature]; });
            })
        );
        Features result;
        for (const std::size_t feature : features) result.push_back(feature);
        return result;
    }
    
    Objects FeaturesPrime(const Features& features)
    {
        auto objects = (
            std::views::iota(std::size_t(0), data_.size()) |
            std::views::filter([&](const std::size_t object) {
                return std::ranges::all_of(features, [&](const std::size_t feature) { return data_[object][feature]; });
            })
        );
        Features result;
        for (const std::size_t object : objects) result.push_back(object);
        return result;
    }

    Objects ObjectsClosure(const Objects& objects)
    {
        return FeaturesPrime(ObjectsPrime(objects));
    }
    
    Features FeaturesClosure(const Features& features)
    {
        return ObjectsPrime(FeaturesPrime(features));
    }

    
    using CboCallback = std::function<void(const Objects&, const Features&)>;

    void Cbo(const CboCallback& callback, const Features& features = {}, const std::size_t add_from = 0)
    {
        const Features closure = FeaturesClosure(features);

        std::set<std::size_t> before_set(features.begin(), features.end());
        for (const auto& feature : closure)
        {
            if (before_set.contains(feature)) continue;
            if (features.size() != 0 && feature < features[features.size() - 1]) return; // non canonical
        }

        callback(FeaturesPrime(closure), closure);

        const std::set<std::size_t> closure_set(closure.begin(), closure.end());
        for (std::size_t i = add_from; i < features_.size(); ++i)
        {
            if (closure_set.contains(i)) continue;
            Features closure_cpy = closure;
            closure_cpy.push_back(i);
            Cbo(callback, closure_cpy, i + 1);
        }
    }

    const std::string& GetFeatureName(const std::size_t& feature)
    {
        return features_[feature];
    }

private:
    std::vector<std::string> features_;
    std::vector<std::vector<bool>> data_;
};


int main()
{
    std::ios::sync_with_stdio(false);

    std::ifstream datafile("../data.csv");

    std::vector<std::string> features = ReadFeatureNames(datafile);
    
    std::string line;
    std::vector<std::vector<bool>> data;
    while (std::getline(datafile, line)) data.push_back(ReadObject(line));

    FCA fca(std::move(features), std::move(data));

    std::ofstream output("concepts.txt");
    auto callback = [&fca, &output](const FCA::Objects& objects, const FCA::Features& features) {
        output << "Found formal concept: ({";
        for (std::size_t i = 0; i < objects.size(); i++)
        {
            output << objects[i] + 1;
            if (i != objects.size() - 1) output << ',';
        }
        output << "}, {";
        for (std::size_t i = 0; i < features.size(); i++)
        {
            output << fca.GetFeatureName(features[i]);
            if (i != features.size() - 1) output << ',';
        }
        output << "})\n";
    };

    fca.Cbo(callback);
}