#include <iostream>
#include <vector>
#include <string>

int charAt2(const std::string& s, int d) {
    if (d >= static_cast<int>(s.size())) {
        return -1;
    }
    return static_cast<unsigned char>(s[d]);
}

int getCode2(const std::string& s, int d) {
    if (d >= static_cast<int>(s.size())) {
        return 0;
    }
    return static_cast<unsigned char>(s[d]) + 1;
}

void stringQuickSortRecursive2(std::vector<std::string>& a, int l, int r, int d, long long& charComparisons) {
    if (l >= r) {
        return;
    }
    int mid = l + (r - l) / 2;
    std::swap(a[l], a[mid]);
    int less = l;
    int greater = r;
    int pivot = charAt2(a[l], d);
    int i = l + 1;
    while (i <= greater) {
        int cur = charAt2(a[i], d);
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
    stringQuickSortRecursive2(a, l, less - 1, d, charComparisons);
    if (pivot >= 0) {
        stringQuickSortRecursive2(a, less, greater, d + 1, charComparisons);
    }
    stringQuickSortRecursive2(a, greater + 1, r, d, charComparisons);
}

void msdRadixSortRecursive2(std::vector<std::string>& a, std::vector<std::string>& buffer, int l, int r, int d, long long& charComparisons) {
    if (l >= r) {
        return;
    }
    const int switchLimit = 74;
    if (r - l + 1 < switchLimit) {
        stringQuickSortRecursive2(a, l, r, d, charComparisons);
        return;
    }
    const int alphabetSize = 256;
    const int codesCount = alphabetSize + 1;
    std::vector<int> count(codesCount + 1, 0);
    for (int i = l; i <= r; ++i) {
        int code = getCode2(a[i], d);
        ++charComparisons;
        ++count[code + 1];
    }
    for (int i = 0; i < codesCount; ++i) {
        count[i + 1] += count[i];
    }
    std::vector<int> start = count;
    for (int i = l; i <= r; ++i) {
        int code = getCode2(a[i], d);
        ++charComparisons;
        buffer[l + count[code]] = a[i];
        ++count[code];
    }
    for (int i = l; i <= r; ++i) {
        a[i] = buffer[i];
    }
    for (int code = 1; code < codesCount; ++code) {
        int left = l + start[code];
        int right = l + start[code + 1] - 1;
        if (left < right) {
            msdRadixSortRecursive2(a, buffer, left, right, d + 1, charComparisons);
        }
    }
}

void msdRadixSort2(std::vector<std::string>& a, long long& charComparisons) {
    charComparisons = 0;
    if (a.empty()) {
        return;
    }
    std::vector<std::string> buffer(a.size());
    msdRadixSortRecursive2(a, buffer, 0, static_cast<int>(a.size()) - 1, 0, charComparisons);
}