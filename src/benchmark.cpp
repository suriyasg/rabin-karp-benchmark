#include "RabinKarp.hpp"
#include <benchmark/benchmark.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

struct TestCase {
    string word;
    int expected_index;
};

// Global variables so the benchmark macros can access the data
string global_text;
vector<TestCase> global_testCases;

// New global variables for loop control
int global_offset = 0;
int global_count = -1; // -1 means "process everything until the end"

// Helper to load the datasets into memory once
void LoadData(const string& textPath, const string& csvPath) {
    ifstream textFile(textPath);
    if (textFile) {
        stringstream buffer;
        buffer << textFile.rdbuf();
        global_text = buffer.str();
    } else {
        cerr << "Failed to open " << textPath << "\n";
        exit(1);
    }

    ifstream dataFile(csvPath);
    if (dataFile) {
        string line, word, idxStr;
        getline(dataFile, line); // Skip header
        while (getline(dataFile, line)) {
            stringstream ss(line);
            getline(ss, word, ',');
            getline(ss, idxStr, ',');
            global_testCases.push_back({word, stoi(idxStr)});
        }
    } else {
        cerr << "Failed to open " << csvPath << "\n";
        exit(1);
    }
}

// ---------------------------------------------------------
// GOOGLE BENCHMARK DEFINITIONS
// ---------------------------------------------------------

static void BM_Deterministic(benchmark::State& state) {
    // Calculate safe bounds OUTSIDE the timed loop to avoid benchmarking the math
    int total_cases = global_testCases.size();
    int start_idx = min(max(0, global_offset), total_cases);
    int elements = (global_count < 0) ? (total_cases - start_idx) : global_count;
    int end_idx = min(start_idx + elements, total_cases);

    for (auto _ : state) {
        for (int i = start_idx; i < end_idx; ++i) {
            int res = rabinKarpDeterministic(global_text, global_testCases[i].word);
            benchmark::DoNotOptimize(res); 
        }
    }
}
BENCHMARK(BM_Deterministic)->Unit(benchmark::kMillisecond);

static void BM_RandomizedBase(benchmark::State& state) {
    int total_cases = global_testCases.size();
    int start_idx = min(max(0, global_offset), total_cases);
    int elements = (global_count < 0) ? (total_cases - start_idx) : global_count;
    int end_idx = min(start_idx + elements, total_cases);

    for (auto _ : state) {
        for (int i = start_idx; i < end_idx; ++i) {
            int res = rabinKarpRandomizedBase(global_text, global_testCases[i].word);
            benchmark::DoNotOptimize(res);
        }
    }
}
BENCHMARK(BM_RandomizedBase)->Unit(benchmark::kMillisecond);

static void BM_RandomizedModulo(benchmark::State& state) {
    int total_cases = global_testCases.size();
    int start_idx = min(max(0, global_offset), total_cases);
    int elements = (global_count < 0) ? (total_cases - start_idx) : global_count;
    int end_idx = min(start_idx + elements, total_cases);

    for (auto _ : state) {
        for (int i = start_idx; i < end_idx; ++i) {
            int res = rabinKarpRandomizedModulo(global_text, global_testCases[i].word);
            benchmark::DoNotOptimize(res);
        }
    }
}
BENCHMARK(BM_RandomizedModulo)->Unit(benchmark::kMillisecond);

// ---------------------------------------------------------
// CUSTOM MAIN FUNCTION
// ---------------------------------------------------------
int main(int argc, char** argv) {
    string textPath = "../data/sample.txt";
    string csvPath = "../data/dataset.csv";

    // 1. Intercept custom dataset arguments and remove them from argv
    int new_argc = 1; 
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "--text" && i + 1 < argc) {
            textPath = argv[++i]; 
        } else if (arg == "--csv" && i + 1 < argc) {
            csvPath = argv[++i];  
        } else if (arg == "--offset" && i + 1 < argc) {
            global_offset = stoi(argv[++i]); 
        } else if (arg == "--count" && i + 1 < argc) {
            global_count = stoi(argv[++i]);  
        } else {
            argv[new_argc++] = argv[i];
        }
    }
    
    argc = new_argc;
    argv[argc] = nullptr; 

    cout << "Loading dataset into memory...\n";
    LoadData(textPath, csvPath);
    
    // Calculate actual bounds for the console output
    int start = min(max(0, global_offset), (int)global_testCases.size());
    int end = min(start + (global_count < 0 ? (int)global_testCases.size() - start : global_count), (int)global_testCases.size());
    
    cout << "Ready! File size: " << global_text.length() << " chars.\n";
    cout << "Total Queries Available: " << global_testCases.size() << "\n";
    cout << "Executing subset: Index " << start << " to " << (end - 1) << " (" << (end - start) << " queries)\n\n";

    // 2. Pass the filtered arguments to Google Benchmark
    ::benchmark::Initialize(&argc, argv);
    if (::benchmark::ReportUnrecognizedArguments(argc, argv)) return 1;
    
    // 3. Run the suite
    ::benchmark::RunSpecifiedBenchmarks();
    ::benchmark::Shutdown();
    
    return 0;
}