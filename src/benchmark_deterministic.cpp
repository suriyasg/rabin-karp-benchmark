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
    // Load the text
    ifstream textFile("../data/sample.txt");
    if (!textFile.is_open()) {
        cerr << "Error: Could not open data/sample.txt\n";
        return 1;
    }
    stringstream buffer;
    buffer << textFile.rdbuf();
    string text = buffer.str();
    
    // Load the dataset (queries)
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

    int errorsDet = 0;

    for (const auto& test : tests) {
        auto start = high_resolution_clock::now();
        int resDet = rabinKarpDeterministic(text, test.word);
        timeIntervels.push_back(duration<double>(high_resolution_clock::now() - start).count());
        if (resDet != test.expected_index) errorsDet++;
    }

    double totalTime = std::accumulate(timeIntervels.begin(), timeIntervels.end(), 0.0);

    // Output Results
    cout << "--- Benchmark Results (" << tests.size() << " queries) ---\n";
    
    cout << "[Deterministic Fixed-Base]\n";
    cout << "Time:   " << totalTime << " seconds\n";
    cout << "Errors: " << errorsDet << "\n\n";

    // Save individual times to a CSV file
    ofstream resultsFile("../result/benchmark_deterministic.csv");
    if (resultsFile.is_open()) {
        resultsFile << "TestNumber,TimeTaken_Seconds\n"; // CSV Header
        for (size_t i = 0; i < timeIntervels.size(); ++i) {
            // i + 1 gives us 1-based test numbering
            resultsFile << (i + 1) << "," << timeIntervels[i] << "\n";
        }
        resultsFile.close();
        cout << "Individual test times saved to result/benchmark_deterministic.csv\n";
    } else {
        cerr << "Error: Could not create result/benchmark_deterministic.csv\n";
    }

    return 0;
}