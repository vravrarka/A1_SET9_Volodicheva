#include <iostream>
#include <vector>
#include <string>

struct LcpCompareRes {
    int order;
    int lcp;
    LcpCompareRes() : order(0), lcp(0) {}
    LcpCompareRes(int orderValue, int lcpValue)
        : order(orderValue), lcp(lcpValue) {}
};

struct LcpString {
    std::string value;
    int lcpWithPrev;
    LcpString() : value(""), lcpWithPrev(0) {}
    LcpString(const std::string& valueValue, int lcpValue)
        : value(valueValue), lcpWithPrev(lcpValue) {}
};

LcpCompareRes lcpCompare(const std::string& first, const std::string& second, int knownPrefixLength, long long& charComparisons) {
    int i = knownPrefixLength;
    while (i < static_cast<int>(first.size()) &&
           i < static_cast<int>(second.size())) {
        ++charComparisons;
        if (first[i] != second[i]) {
            break;
        }
        ++i;
    }
    if (i == static_cast<int>(first.size()) &&
        i == static_cast<int>(second.size())) {
        return LcpCompareRes(0, i);
    }
    if (i == static_cast<int>(first.size())) {
        return LcpCompareRes(-1, i);
    }
    if (i == static_cast<int>(second.size())) {
        return LcpCompareRes(1, i);
    }
    if (first[i] < second[i]) {
        return LcpCompareRes(-1, i);
    }
    return LcpCompareRes(1, i);
}

std::vector<LcpString> mergeParts(const std::vector<LcpString>& firstPart,
                                  const std::vector<LcpString>& secondPart,
                                  long long& charComparisons) {
    std::vector<LcpString> left = firstPart;
    std::vector<LcpString> right = secondPart;
    std::vector<LcpString> result;
    int i = 0;
    int j = 0;
    while (i < static_cast<int>(left.size()) &&
           j < static_cast<int>(right.size())) {
        int leftLcp = left[i].lcpWithPrev;
        int rightLcp = right[j].lcpWithPrev;
        if (leftLcp > rightLcp) {
            result.push_back(left[i]);
            ++i;
        } else if (leftLcp < rightLcp) {
            result.push_back(right[j]);
            ++j;
        } else {
            LcpCompareRes cmp = lcpCompare(left[i].value, right[j].value, leftLcp, charComparisons);
            if (cmp.order <= 0) {
                result.push_back(left[i]);
                ++i;
                if (j < static_cast<int>(right.size())) {
                    right[j].lcpWithPrev = cmp.lcp;
                }
            } else {
                result.push_back(right[j]);
                ++j;
                if (i < static_cast<int>(left.size())) {
                    left[i].lcpWithPrev = cmp.lcp;
                }
            }
        }
    }
    while (i < static_cast<int>(left.size())) {
        result.push_back(left[i]);
        ++i;
    }
    while (j < static_cast<int>(right.size())) {
        result.push_back(right[j]);
        ++j;
    }
    if (!result.empty()) {
        result[0].lcpWithPrev = 0;
    }
    return result;
}

std::vector<LcpString> stringMergeSortRecursive(const std::vector<std::string>& a, int l, int r, long long& charComparisons) {
    if (l == r) {
        std::vector<LcpString> result;
        result.push_back(LcpString(a[l], 0));
        return result;
    }
    int mid = l + (r - l) / 2;
    std::vector<LcpString> firstPart = stringMergeSortRecursive(a, l, mid, charComparisons);
    std::vector<LcpString> secondPart = stringMergeSortRecursive(a, mid + 1, r, charComparisons);
    return mergeParts(firstPart, secondPart, charComparisons);
}

void stringMergeSort(std::vector<std::string>& a, long long& charComparisons) {
    charComparisons = 0;
    if (a.empty()) {
        return;
    }
    std::vector<LcpString> sorted = stringMergeSortRecursive(a, 0, static_cast<int>(a.size()) - 1, charComparisons);
    for (int i = 0; i < static_cast<int>(sorted.size()); ++i) {
        a[i] = sorted[i].value;
    }
}