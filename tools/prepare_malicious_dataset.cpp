#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    // Default parameters
    string textFile = "../data/malicious_sample.txt";
    string csvFile = "../data/malicious_dataset.csv";
    size_t repeatCount = 100000;

    // CLI parser: --text <path> --csv <path> --repeats <n>
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "--text" && i + 1 < argc) textFile = argv[++i];
        else if (arg == "--csv" && i + 1 < argc) csvFile = argv[++i];
        else if (arg == "--repeats" && i + 1 < argc) repeatCount = stoull(argv[++i]);
    }

    // 1. Generate the text file data
    // Creates the "GFFeaGFFea..." pattern and appends "FeGF" at the end
    string collisionPattern = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAGFFe";
    string suffix = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAFeGF";
    
    // Pre-allocate memory for performance
    string fullText;
    fullText.reserve((collisionPattern.length() * repeatCount) + suffix.length());
    
    for (size_t i = 0; i < repeatCount; ++i) {
        fullText += collisionPattern;
    }
    
    size_t indexOfMatchingString = fullText.length();
    fullText += suffix;

    // Write to text file
    ofstream txtOut(textFile);
    if (!txtOut.is_open()) {
        cerr << "Error: Could not open " << textFile << " for writing.\n";
        return 1;
    }
    txtOut << fullText;
    txtOut.close();
    
    cout << "Created " << textFile << " with length " << fullText.length() << " characters.\n";

    // 2. Generate the CSV file
    // Sets up the headers and places FeGF first, followed by its expected index
    ofstream csvOut(csvFile);
    if (!csvOut.is_open()) {
        cerr << "Error: Could not open " << csvFile << " for writing.\n";
        return 1;
    }
    
    csvOut << "word,expected_index\n";
    csvOut << "FeGF," << indexOfMatchingString << "\n";
    csvOut.close();
    
    cout << "Created " << csvFile << " successfully.\n";

    return 0;
}