#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
    string outputFileText = "../data/malicious_sample.txt";
    string outputFileCsv = "../data/malicious_dataset.csv";

    // 1. Generate the Malicious Text
    // We create a block of 10,000 'A's ending in "Fe"
    string block = string(10000, 'A') + "Fe";
    
    string text = "";
    cout << "Generating massive malicious text (this may take a second)..." << endl;
    
    // Repeat the block 1,000 times (creates a ~10MB file)
    for (int i = 0; i < 1000; i++) { 
        text += block;
    }
    
    // Append the actual target at the very end so it must scan the whole file
    string target_pattern = string(10000, 'A') + "GF";
    text += target_pattern; 

    ofstream outText(outputFileText);
    outText << text;
    outText.close();

    // 2. Generate the CSV Query
    ofstream outCsv(outputFileCsv);
    outCsv << "word,expected_index\n";
    
    // The expected index is exactly where we appended the target pattern
    size_t expected_index = 1000 * block.length();
    outCsv << target_pattern << "," << expected_index << "\n";
    outCsv.close();

    cout << "Malicious dataset created in ../data/!" << endl;
    return 0;
}