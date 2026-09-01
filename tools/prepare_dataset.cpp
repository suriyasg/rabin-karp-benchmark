#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <sstream>

using namespace std;

int main(int argc, char* argv[]) {
    // Default parameters
    string inputFile = "../data/sample.txt";
    string outputFile = "../data/dataset.csv";
    int patternLength = 100;
    int numCases = 100;

    // CLI parser: --input <path> --output <path> --length <n> --count <n>
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "--input" && i + 1 < argc) inputFile = argv[++i];
        else if (arg == "--output" && i + 1 < argc) outputFile = argv[++i];
        else if (arg == "--length" && i + 1 < argc) patternLength = stoi(argv[++i]);
        else if (arg == "--count" && i + 1 < argc) numCases = stoi(argv[++i]);
    }

    // 1. Load the text file
    ifstream inFile(inputFile);
    if (!inFile.is_open()) {
        cerr << "Error: Could not open " << inputFile << "\n";
        return 1;
    }

    stringstream buffer;
    buffer << inFile.rdbuf();
    string text = buffer.str();
    inFile.close();

    if (text.length() < patternLength) {
        cerr << "Error: Text length (" << text.length() << ") is shorter than requested pattern length (" << patternLength << ").\n";
        return 1;
    }

    // 2. Setup Random Number Generator
    random_device rd;
    mt19937 rng(rd());
    // Max valid starting index to avoid going out of bounds
    uniform_int_distribution<size_t> dist(0, text.length() - patternLength);

    // 3. Open CSV for writing
    ofstream outFile(outputFile);
    outFile << "word,expected_index\n";

    int count = 0;
    int attempts = 0;
    int maxAttempts = numCases * 100; // Failsafe to prevent infinite loops

    cout << "Extracting " << numCases << " patterns of length " << patternLength << "...\n";

    // 4. Extract random substrings
    while (count < numCases && attempts < maxAttempts) {
        attempts++;
        
        size_t randomIdx = dist(rng);
        string substring = text.substr(randomIdx, patternLength);

        // Reject strings with commas or newlines to prevent breaking the CSV parser
        if (substring.find(',') != string::npos || 
            substring.find('\n') != string::npos || 
            substring.find('\r') != string::npos) {
            continue; 
        }

        // Even though we extracted from randomIdx, it might exist earlier in the text.
        // We must use text.find() to get the true FIRST occurrence index, as that is
        // what the benchmark algorithms will return.
        size_t expected_index = text.find(substring);

        if (expected_index != string::npos) {
            outFile << substring << "," << expected_index << "\n";
            count++;
        }
    }

    outFile.close();

    if (count < numCases) {
        cout << "Warning: Only found " << count << " valid sequences without commas/newlines after " << attempts << " attempts.\n";
    } else {
        cout << "Successfully generated " << count << " test cases in " << outputFile << ".\n";
    }

    return 0;
}