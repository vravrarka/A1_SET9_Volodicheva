#include <string>
#include <vector>

class MergeSort {
private:
    static int compareStrings(const std::string& first, const std::string& second, long long& charComparisons) {
        int i = 0;
        while (i < static_cast<int>(first.size()) &&
               i < static_cast<int>(second.size())) {
            ++charComparisons;
            if (first[i] < second[i]) {
                return -1;
            }
            if (first[i] > second[i]) {
                return 1;
            }
            ++i;
        }
        if (first.size() < second.size()) {
            return -1;
        }
        if (first.size() > second.size()) {
            return 1;
        }
        return 0;
    }

    static void mergeSortRecursive(std::vector<std::string>& a, std::vector<std::string>& buffer, int left, int right, long long& charComparisons) {
        if (left >= right) {
            return;
        }
        int middle = left + (right - left) / 2;
        mergeSortRecursive(a, buffer, left, middle, charComparisons);
        mergeSortRecursive(a, buffer, middle + 1, right, charComparisons);
        int i = left;
        int j = middle + 1;
        int k = left;
        while (i <= middle && j <= right) {
            if (compareStrings(a[i], a[j], charComparisons) <= 0) {
                buffer[k] = a[i];
                ++i;
            } else {
                buffer[k] = a[j];
                ++j;
            }
            ++k;
        }
        while (i <= middle) {
            buffer[k] = a[i];
            ++i;
            ++k;
        }
        while (j <= right) {
            buffer[k] = a[j];
            ++j;
            ++k;
        }
        for (int index = left; index <= right; ++index) {
            a[index] = buffer[index];
        }
    }

public:
    static void sort(std::vector<std::string>& a, long long& charComparisons) {
        if (a.empty()) {
            return;
        }
        std::vector<std::string> buffer(a.size());
        mergeSortRecursive(a, buffer, 0, static_cast<int>(a.size()) - 1, charComparisons);
    }
};