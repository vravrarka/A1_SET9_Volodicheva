#include <string>
#include <vector>

class QuickSort {
private:
    static int compareStrings(const std::string& first, const std::string& second, long long& charComparisons) {
        int i = 0;
        while (i < static_cast<int>(first.size()) && i < static_cast<int>(second.size())) {
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

    static void quickSortRecursive(std::vector<std::string>& a, int left, int right,long long& charComparisons) {
        if (left >= right) {
            return;
        }
        std::string pivot = a[left + (right - left) / 2];
        int i = left;
        int j = right;
        while (i <= j) {
            while (compareStrings(a[i], pivot, charComparisons) < 0) {
                ++i;
            }
            while (compareStrings(a[j], pivot, charComparisons) > 0) {
                --j;
            }
            if (i <= j) {
                std::swap(a[i], a[j]);
                ++i;
                --j;
            }
        }
        if (left < j) {
            quickSortRecursive(a, left, j, charComparisons);
        }
        if (i < right) {
            quickSortRecursive(a, i, right, charComparisons);
        }
    }

public:
    static void sort(std::vector<std::string>& a, long long& charComparisons) {
        if (a.empty()) {
            return;
        }
        quickSortRecursive(a, 0, static_cast<int>(a.size()) - 1, charComparisons);
    }
};