#include <iostream>
#include <fstream>
#include <string>
#include <random>
#include <vector>

using namespace std;

int main(int argc, char* argv[]) {
    // Default parameters
    string outputFile = "../data/generated_sample.txt";
    size_t targetSize = 1000000; // 1 million characters
    string alphabetType = "alpha"; 

    // CLI parser: --output <path> --size <n> --alphabet <alpha|dna|binary>
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "--output" && i + 1 < argc) outputFile = argv[++i];
        else if (arg == "--size" && i + 1 < argc) targetSize = stoull(argv[++i]);
        else if (arg == "--alphabet" && i + 1 < argc) alphabetType = argv[++i];
    }

    // Define safe alphabets (strictly NO commas or newlines)
    string alphabet;
    if (alphabetType == "dna") {
        alphabet = "ACGT"; // High collision rate, great for stress testing
    } else if (alphabetType == "binary") {
        alphabet = "01";   // Extreme collision rate
    } else {
        // Default alphanumeric + space
        alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 ";
    }

    cout << "Generating " << targetSize << " chars using '" << alphabetType << "' alphabet...\n";

    // Setup Random Number Generator
    random_device rd;
    mt19937 rng(rd());
    uniform_int_distribution<size_t> dist(0, alphabet.length() - 1);

    // Open file for writing
    ofstream outFile(outputFile);
    if (!outFile.is_open()) {
        cerr << "Error: Could not open " << outputFile << " for writing.\n";
        return 1;
    }

    // Buffer for fast I/O (8 KB chunks)
    const size_t CHUNK_SIZE = 8192;
    string buffer(CHUNK_SIZE, ' ');

    size_t charsWritten = 0;
    while (charsWritten < targetSize) {
        size_t charsToWrite = min(CHUNK_SIZE, targetSize - charsWritten);
        
        // Fill the buffer with random characters
        for (size_t i = 0; i < charsToWrite; ++i) {
            buffer[i] = alphabet[dist(rng)];
        }

        // Write chunk to file
        outFile.write(buffer.data(), charsToWrite);
        charsWritten += charsToWrite;
    }

    outFile.close();
    cout << "Successfully generated " << targetSize << " safe characters into " << outputFile << ".\n";

    return 0;
}