#include <vector>
#include <string>
#include <algorithm>

int charAt(const std::string& s, int index) {
    if (index >= static_cast<int>(s.size())) {
        return -1;
    }
    return static_cast<unsigned char>(s[index]);
}

void stringQuickSortRecursive(std::vector<std::string>& a, int l, int r, int d, long long& charComparisons) {
    if (l >= r) {
        return;
    }
    int mid = l + (r - l) / 2;
    std::swap(a[l], a[mid]);
    int less = l;
    int greater = r;
    int pivot = charAt(a[l], d);
    int i = l + 1;
    while (i <= greater) {
        int cur = charAt(a[i], d);
        ++charComparisons;
        if (cur < pivot) {
            std::swap(a[less], a[i]);
            ++less;
            ++i;
        } else {
            ++charComparisons;
            if (cur > pivot) {
                std::swap(a[i], a[greater]);
                --greater;
            } else {
                ++i;
            }
        }
    }
    stringQuickSortRecursive(a, l, less - 1, d, charComparisons);
    if (pivot >= 0) {
        stringQuickSortRecursive(a, less, greater, d + 1, charComparisons);
    }
    stringQuickSortRecursive(a, greater + 1, r, d, charComparisons);
}

void stringQuickSort(std::vector<std::string>& a, long long& charComparisons) {
    charComparisons = 0;
    if (a.empty()) {
        return;
    }
    stringQuickSortRecursive(a, 0, static_cast<int>(a.size()) - 1, 0, charComparisons);
}