#include "StringGenerator.cpp"
#include "QuickSort.cpp"
#include "MergeSort.cpp"
#include "StringMergeSort.cpp"
#include "StringQuickSort.cpp"
#include "MSDRadixSort1.cpp"
#include "MSDRadixSort2.cpp"
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <chrono>
#include <functional>
#include <algorithm>
#include <random>

struct SortStatistics {
    long long timeMicroseconds;
    long long charComparisons;
    SortStatistics() : timeMicroseconds(0), charComparisons(0) {}
    SortStatistics(long long timeValue, long long comparisonsValue)
        : timeMicroseconds(timeValue), charComparisons(comparisonsValue) {}
};

class StringSortTester {
private:
    StringGenerator generator;
    int maxArraySize;
    int step;
    int repeats;

    typedef std::function<void(std::vector<std::string>&, long long&)> SortFunction;

    bool isSortedCorrectly(const std::vector<std::string>& a) {
        for (int i = 1; i < static_cast<int>(a.size()); ++i) {
            if (a[i - 1] > a[i]) {
                return false;
            }
        }
        return true;
    }

    std::vector<std::string> getSubArray(const std::vector<std::string>& source, int n) {
        std::vector<std::string> result;
        for (int i = 0; i < n; ++i) {
            result.push_back(source[i]);
        }
        return result;
    }

    SortStatistics measureSort(const std::vector<std::string>& originalArray,
                               SortFunction sortFunction) {
        long long totalTime = 0;
        long long totalComparisons = 0;
        for (int repeat = 0; repeat < repeats; ++repeat) {
            std::vector<std::string> currentArray = originalArray;
            long long charComparisons = 0;
            std::chrono::high_resolution_clock::time_point start =
                std::chrono::high_resolution_clock::now();
            sortFunction(currentArray, charComparisons);
            std::chrono::high_resolution_clock::time_point finish =
                std::chrono::high_resolution_clock::now();
            long long duration =
                std::chrono::duration_cast<std::chrono::microseconds>(finish - start).count();
            if (!isSortedCorrectly(currentArray)) {
                std::cout << "Ошибка: массив отсортирован неправильно." << std::endl;
            }
            totalTime += duration;
            totalComparisons += charComparisons;
        }
        return SortStatistics(totalTime / repeats, totalComparisons / repeats);
    }

    void writeNumberCell(std::ofstream& file, long long value) {
        file << "<Cell><Data ss:Type=\"Number\">" << value << "</Data></Cell>\n";
    }

    void writeStringCell(std::ofstream& file, const std::string& value, const std::string& styleId) {
        file << "<Cell ss:StyleID=\"" << styleId << "\"><Data ss:Type=\"String\">" << value << "</Data></Cell>\n";
    }

    void writeMergedStringCell(std::ofstream& file,
                               const std::string& value,
                               int mergeAcross,
                               const std::string& styleId) {
        file << "<Cell ss:MergeAcross=\"" << mergeAcross << "\" ss:StyleID=\"" << styleId << "\">";
        file << "<Data ss:Type=\"String\">" << value << "</Data></Cell>\n";
    }

    void writeExcelFile(const std::string& fileName,
                        const std::string& sortName,
                        const std::vector<int>& sizes,
                        const std::vector<SortStatistics>& randomStatistics,
                        const std::vector<SortStatistics>& reverseStatistics,
                        const std::vector<SortStatistics>& nearlySortedStatistics) {
        std::ofstream file(fileName.c_str(), std::ios::binary);
        file << "\xEF\xBB\xBF";
        file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
        file << "<?mso-application progid=\"Excel.Sheet\"?>\n";
        file << "<Workbook xmlns=\"urn:schemas-microsoft-com:office:spreadsheet\"\n";
        file << " xmlns:o=\"urn:schemas-microsoft-com:office:office\"\n";
        file << " xmlns:x=\"urn:schemas-microsoft-com:office:excel\"\n";
        file << " xmlns:ss=\"urn:schemas-microsoft-com:office:spreadsheet\">\n";
        file << "<Styles>\n";
        file << "<Style ss:ID=\"Title\">\n";
        file << "<Font ss:Bold=\"1\" ss:Size=\"12\"/>\n";
        file << "<Alignment ss:Horizontal=\"Center\"/>\n";
        file << "</Style>\n";
        file << "<Style ss:ID=\"Header\">\n";
        file << "<Font ss:Bold=\"1\"/>\n";
        file << "<Interior ss:Color=\"#D9EAF7\" ss:Pattern=\"Solid\"/>\n";
        file << "<Alignment ss:Horizontal=\"Center\"/>\n";
        file << "<Borders>\n";
        file << "<Border ss:Position=\"Bottom\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>\n";
        file << "<Border ss:Position=\"Left\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>\n";
        file << "<Border ss:Position=\"Right\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>\n";
        file << "<Border ss:Position=\"Top\" ss:LineStyle=\"Continuous\" ss:Weight=\"1\"/>\n";
        file << "</Borders>\n";
        file << "</Style>\n";
        file << "<Style ss:ID=\"Number\">\n";
        file << "<Alignment ss:Horizontal=\"Right\"/>\n";
        file << "</Style>\n";
        file << "</Styles>\n";
        file << "<Worksheet ss:Name=\"" << sortName << "\">\n";
        file << "<Table>\n";
        file << "<Column ss:Width=\"90\"/>\n";
        file << "<Column ss:Width=\"90\"/>\n";
        file << "<Column ss:Width=\"210\"/>\n";
        file << "<Column ss:Width=\"90\"/>\n";
        file << "<Column ss:Width=\"210\"/>\n";
        file << "<Column ss:Width=\"90\"/>\n";
        file << "<Column ss:Width=\"210\"/>\n";
        file << "<Row>\n";
        file << "<Cell/>\n";
        writeMergedStringCell(file, "Случайные массивы", 1, "Title");
        writeMergedStringCell(file, "Обратно отсортированные", 1, "Title");
        writeMergedStringCell(file, "Почти отсортированные", 1, "Title");
        file << "</Row>\n";
        file << "<Row>\n";
        writeStringCell(file, "n", "Header");
        writeStringCell(file, "t, мкс", "Header");
        writeStringCell(file, "Количество посимвольных сравнений", "Header");
        writeStringCell(file, "t, мкс", "Header");
        writeStringCell(file, "Количество посимвольных сравнений", "Header");
        writeStringCell(file, "t, мкс", "Header");
        writeStringCell(file, "Количество посимвольных сравнений", "Header");
        file << "</Row>\n";
        for (int i = 0; i < static_cast<int>(sizes.size()); ++i) {
            file << "<Row>\n";
            writeNumberCell(file, sizes[i]);
            writeNumberCell(file, randomStatistics[i].timeMicroseconds);
            writeNumberCell(file, randomStatistics[i].charComparisons);
            writeNumberCell(file, reverseStatistics[i].timeMicroseconds);
            writeNumberCell(file, reverseStatistics[i].charComparisons);
            writeNumberCell(file, nearlySortedStatistics[i].timeMicroseconds);
            writeNumberCell(file, nearlySortedStatistics[i].charComparisons);
            file << "</Row>\n";
        }
        file << "</Table>\n";
        file << "</Worksheet>\n";
        file << "</Workbook>\n";
        file.close();
    }

public:
    StringSortTester()
        : maxArraySize(3000),
          step(100),
          repeats(5) {}

    void runSort(const std::string& sortName, SortFunction sortFunction) {
        std::cout << "Запуск сортировки: " << sortName << std::endl;
        std::vector<std::string> randomMaxArray =
            generator.generateRandomArray(maxArraySize);
        std::vector<std::string> reverseSortedMaxArray = randomMaxArray;
        std::sort(reverseSortedMaxArray.begin(), reverseSortedMaxArray.end());
        std::reverse(reverseSortedMaxArray.begin(), reverseSortedMaxArray.end());
        std::vector<std::string> nearlySortedMaxArray = randomMaxArray;
        std::sort(nearlySortedMaxArray.begin(), nearlySortedMaxArray.end());
        std::mt19937 localGenerator(std::random_device{}());
        std::uniform_int_distribution<int> indexDistribution(0, maxArraySize - 1);
        int swapsCount = std::max(1, maxArraySize / 20);
        for (int i = 0; i < swapsCount; ++i) {
            int firstIndex = indexDistribution(localGenerator);
            int secondIndex = indexDistribution(localGenerator);
            std::swap(nearlySortedMaxArray[firstIndex], nearlySortedMaxArray[secondIndex]);
        }
        std::vector<int> sizes;
        std::vector<SortStatistics> randomStatistics;
        std::vector<SortStatistics> reverseStatistics;
        std::vector<SortStatistics> nearlySortedStatistics;
        for (int n = step; n <= maxArraySize; n += step) {
            sizes.push_back(n);
            std::vector<std::string> randomArray =
                getSubArray(randomMaxArray, n);
            std::vector<std::string> reverseArray =
                getSubArray(reverseSortedMaxArray, n);
            std::vector<std::string> nearlySortedArray =
                getSubArray(nearlySortedMaxArray, n);
            randomStatistics.push_back(measureSort(randomArray, sortFunction));
            reverseStatistics.push_back(measureSort(reverseArray, sortFunction));
            nearlySortedStatistics.push_back(measureSort(nearlySortedArray, sortFunction));
            std::cout << "n = " << n << " готово" << std::endl;
        }
        std::string fileName = "result_" + sortName + ".xls";
        writeExcelFile(fileName,
                       sortName,
                       sizes,
                       randomStatistics,
                       reverseStatistics,
                       nearlySortedStatistics);
        std::cout << "Файл создан: " << fileName << std::endl;
    }

    void runAllSorts() {
        runSort("QuickSort",
                [](std::vector<std::string>& a, long long& comparisons) {
                    QuickSort::sort(a, comparisons);
                });

        runSort("MergeSort",
                [](std::vector<std::string>& a, long long& comparisons) {
                    MergeSort::sort(a, comparisons);
                });

        runSort("StringQuickSort",
                [](std::vector<std::string>& a, long long& comparisons) {
                    stringQuickSort(a, comparisons);
                });

        runSort("StringMergeSort",
                [](std::vector<std::string>& a, long long& comparisons) {
                    stringMergeSort(a, comparisons);
                });

        runSort("MSDRadixSort1",
                [](std::vector<std::string>& a, long long& comparisons) {
                    msdRadixSort1(a, comparisons);
                });

        runSort("MSDRadixSort2",
                [](std::vector<std::string>& a, long long& comparisons) {
                    msdRadixSort2(a, comparisons);
                });
    }
};