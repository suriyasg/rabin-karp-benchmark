#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <sstream>
#include <algorithm>

using namespace std;

// Helper to clean punctuation and ensure standard words
string cleanWord(string word) {
    string cleaned;
    for (char c : word) {
        if (isalnum(c)) cleaned += c;
    }
    return cleaned;
}

int main(int argc, char* argv[]) {
    string inputFile = "../data/sample.txt";
    string outputFile = "../data/dataset.csv";
    int numWordsToExtract = 100; // Number of random test cases to generate

    ifstream inFile(inputFile);
    if (!inFile.is_open()) {
        cerr << "Error: Could not open " << inputFile << ". Please create it in the data/ folder.\n";
        return 1;
    }

    // Read entire file into string
    stringstream buffer;
    buffer << inFile.rdbuf();
    string text = buffer.str();
    inFile.close();

    // Extract all valid words
    vector<string> allWords;
    stringstream textStream(text);
    string token;
    while (textStream >> token) {
        string cleaned = cleanWord(token);
        // Only keep words of a decent length to make the benchmark meaningful
        if (cleaned.length() > 4) {
            allWords.push_back(cleaned);
        }
    }

    if (allWords.empty()) {
        cerr << "Error: No valid words found in the text.\n";
        return 1;
    }

    // Shuffle and pick random words
    random_device rd;
    mt19937 g(rd());
    shuffle(allWords.begin(), allWords.end(), g);

    ofstream outFile(outputFile);
    outFile << "word,expected_index\n"; // CSV Header

    int count = 0;
    for (const string& word : allWords) {
        if (count >= numWordsToExtract) break;
        
        // Find ground truth index using standard library
        size_t idx = text.find(word); 
        if (idx != string::npos) {
            outFile << word << "," << idx << "\n";
            count++;
        }
    }

    outFile.close();
    cout << "Successfully generated " << count << " test cases in " << outputFile << ".\n";
    return 0;
}