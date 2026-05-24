#include <vector>
#include <string>

int getCode(const std::string& s, int d) {
    if (d >= static_cast<int>(s.size())) {
        return 0;
    }
    return static_cast<unsigned char>(s[d]) + 1;
}

void msdRadixSortRecursive(std::vector<std::string>& a, std::vector<std::string>& buffer, int l, int r, int d, long long& charComparisons) {
    if (l >= r) {
        return;
    }
    const int alphabetSize = 256;
    const int codesCount = alphabetSize + 1;
    std::vector<int> count(codesCount + 1, 0);
    for (int i = l; i <= r; ++i) {
        int code = getCode(a[i], d);
        ++charComparisons;
        ++count[code + 1];
    }
    for (int i = 0; i < codesCount; ++i) {
        count[i + 1] += count[i];
    }
    std::vector<int> start = count;
    for (int i = l; i <= r; ++i) {
        int code = getCode(a[i], d);
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
            msdRadixSortRecursive(a, buffer, left, right, d + 1, charComparisons);
        }
    }
}

void msdRadixSort1(std::vector<std::string>& a, long long& charComparisons) {
    charComparisons = 0;
    if (a.empty()) {
        return;
    }
    std::vector<std::string> buffer(a.size());
    msdRadixSortRecursive(a, buffer,0, static_cast<int>(a.size()) - 1, 0, charComparisons);
}