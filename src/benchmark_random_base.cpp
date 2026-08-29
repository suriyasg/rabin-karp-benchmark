#include "RabinKarp.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <chrono>
#include <numeric>

using namespace std;
using namespace std::chrono;

struct TestCase {
    string word;
    int expected_index;
};

int main() {
    // 1. Load the text
    ifstream textFile("../data/sample.txt");
    if (!textFile.is_open()) {
        cerr << "Error: Could not open data/sample.txt\n";
        return 1;
    }
    stringstream buffer;
    buffer << textFile.rdbuf();
    string text = buffer.str();
    
    // 2. Load the dataset (queries)
    ifstream dataFile("../data/dataset.csv");
    if (!dataFile.is_open()) {
        cerr << "Error: Could not open data/dataset.csv. Run prepare_dataset first.\n";
        return 1;
    }

    vector<TestCase> tests;
    string line, word, indexStr;
    getline(dataFile, line); // Skip CSV header
    while (getline(dataFile, line)) {
        stringstream ss(line);
        getline(ss, word, ',');
        getline(ss, indexStr, ',');
        tests.push_back({word, stoi(indexStr)});
    }

    cout << "Loaded text length: " << text.length() << " characters.\n";
    cout << "Loaded " << tests.size() << " test queries.\n\n";

    // Run Benchmarks
    std::vector<double> timeIntervels;
    int errorsRandB = 0;

    for (const auto& test : tests) {
        // Randomized Base
        auto start = high_resolution_clock::now();
        int resRandB = rabinKarpRandomizedBase(text, test.word);
        timeIntervels.push_back(duration<double>(high_resolution_clock::now() - start).count());
        if (resRandB != test.expected_index) errorsRandB++;
    }
    
    double totalTime = std::accumulate(timeIntervels.begin(), timeIntervels.end(), 0.0);

    // Output Results
    cout << "--- Benchmark Results (" << tests.size() << " queries) ---\n";

    cout << "[Randomized Base (Las Vegas)]\n";
    cout << "Time:   " << totalTime << " seconds\n";
    cout << "Errors: " << errorsRandB << "\n\n";

    // Save individual times to a CSV file
    ofstream resultsFile("../result/benchmark_random_base.csv");
    if (resultsFile.is_open()) {
        resultsFile << "TestNumber,TimeTaken_Seconds\n"; // CSV Header
        for (size_t i = 0; i < timeIntervels.size(); ++i) {
            // i + 1 gives us 1-based test numbering
            resultsFile << (i + 1) << "," << timeIntervels[i] << "\n";
        }
        resultsFile.close();
        cout << "Individual test times saved to result/benchmark_random_base.csv\n";
    } else {
        cerr << "Error: Could not create result/benchmark_random_base.csv\n";
    }

    return 0;
}