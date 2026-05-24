#include <algorithm>
#include <random>
#include <string>
#include <vector>

class StringGenerator {
private:
    std::string alphabet;
    int minLength;
    int maxLength;
    std::mt19937 generator;
public:
    StringGenerator()
        : alphabet("ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                   "abcdefghijklmnopqrstuvwxyz"
                   "0123456789"
                   "!@#:;^&*()-."),
          minLength(10),
          maxLength(200),
          generator(std::random_device{}()) {}
    const std::string& getAlphabet() const {
        return alphabet;
    }

    std::string generateRandomString() {
        std::uniform_int_distribution<int> lengthDistribution(minLength, maxLength);
        std::uniform_int_distribution<int> charDistribution(0, static_cast<int>(alphabet.size()) - 1);
        int length = lengthDistribution(generator);
        std::string result;
        for (int i = 0; i < length; ++i) {
            result += alphabet[charDistribution(generator)];
        }
        return result;
    }

    std::vector<std::string> generateRandomArray(int n) {
        std::vector<std::string> result;
        for (int i = 0; i < n; ++i) {
            result.push_back(generateRandomString());
        }
        return result;
    }

    std::vector<std::string> generateReverseSortedArray(int n) {
        std::vector<std::string> result = generateRandomArray(n);
        std::sort(result.begin(), result.end());
        std::reverse(result.begin(), result.end());
        return result;
    }

    std::vector<std::string> generateNearlySortedArray(int n) {
        std::vector<std::string> result = generateRandomArray(n);
        std::sort(result.begin(), result.end());
        int swapsCount = std::max(1, n / 20);
        std::uniform_int_distribution<int> indexDistribution(0, n - 1);
        for (int i = 0; i < swapsCount; ++i) {
            int firstIndex = indexDistribution(generator);
            int secondIndex = indexDistribution(generator);
            std::swap(result[firstIndex], result[secondIndex]);
        }
        return result;
    }

    std::vector<std::string> generateRandomArrayWithCommonPrefixes(int n) {
        std::vector<std::string> result;
        std::vector<std::string> prefixes;
        prefixes.push_back("AAA");
        prefixes.push_back("ABC");
        prefixes.push_back("qwerty");
        std::uniform_int_distribution<int> prefixDistribution(0, static_cast<int>(prefixes.size()) - 1);
        for (int i = 0; i < n; ++i) {
            std::string s = prefixes[prefixDistribution(generator)] + generateRandomString();
            result.push_back(s);
        }
        return result;
    }
};
