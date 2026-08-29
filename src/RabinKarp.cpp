#include "RabinKarp.hpp"
#include <vector>
#include <random>

using namespace std;

int rabinKarpDeterministic(const string& text, const string& pattern) {
    int N = text.length();
    int M = pattern.length();
    if (M > N) return -1;

    long long q = 1000000007; 
    
    // THE VULNERABILITY: A fixed, deterministic base
    long long B = 31; 

    long long B_M_minus_1 = 1;
    for (int i = 0; i < M - 1; i++) B_M_minus_1 = (B_M_minus_1 * B) % q;

    long long h_pattern = 0;
    long long h_text = 0;
    for (int i = 0; i < M; i++) {
        h_pattern = (h_pattern * B + pattern[i]) % q;
        h_text = (h_text * B + text[i]) % q;
    }

    for (int i = 0; i <= N - M; i++) {
        if (h_pattern == h_text) {
            bool match = true;
            for (int j = 0; j < M; j++) {
                if (text[i + j] != pattern[j]) {
                    match = false;
                    break;
                }
            }
            if (match) return i; 
        }
        if (i < N - M) {
            long long removed = (text[i] * B_M_minus_1) % q;
            h_text = (h_text - removed + q) % q; 
            h_text = (h_text * B + text[i + M]) % q;
        }
    }
    return -1;
}

int rabinKarpRandomizedBase(const string& text, const string& pattern) {
    int N = text.length();
    int M = pattern.length();
    if (M > N) return -1;

    long long q = 1000000007; 

    // THE SOLUTION: A randomly selected base at runtime
    static thread_local mt19937 rng(random_device{}());
    uniform_int_distribution<long long> dist(256, q - 1);
    long long B = dist(rng); 

    long long B_M_minus_1 = 1;
    for (int i = 0; i < M - 1; i++) B_M_minus_1 = (B_M_minus_1 * B) % q;

    long long h_pattern = 0;
    long long h_text = 0;
    for (int i = 0; i < M; i++) {
        h_pattern = (h_pattern * B + pattern[i]) % q;
        h_text = (h_text * B + text[i]) % q;
    }

    for (int i = 0; i <= N - M; i++) {
        if (h_pattern == h_text) {
            bool match = true;
            for (int j = 0; j < M; j++) {
                if (text[i + j] != pattern[j]) {
                    match = false;
                    break;
                }
            }
            if (match) return i; 
        }
        if (i < N - M) {
            long long removed = (text[i] * B_M_minus_1) % q;
            h_text = (h_text - removed + q) % q; 
            h_text = (h_text * B + text[i + M]) % q;
        }
    }
    return -1;
}

int rabinKarpRandomizedModulo(const string& text, const string& pattern) {
    int N = text.length();
    int M = pattern.length();
    if (M > N) return -1;

    // THE VULNERABILITY FIXED: A pool of large prime numbers.
    // Instead of always using 1000000007, we pick one randomly.
    const vector<long long> PRIME_POOL = {
        1000000007, 1000000009, 1000000021, 1000000033, 1000000087,
        1000000093, 1000000097, 1000000103, 1000000123, 1000000181,
        1000000207, 1000000223, 1000000241, 1000000271, 1000000289,
        1000000297, 1000000321, 1000000349, 1000000363, 1000000403
    };

    // RANDOMIZATION: Select a prime modulo at runtime
    static thread_local mt19937 rng(random_device{}());
    uniform_int_distribution<int> dist(0, PRIME_POOL.size() - 1);
    
    long long q = PRIME_POOL[dist(rng)]; 
    
    // In this version, the base is fixed to the alphabet size (256 for ASCII)
    long long B = 256; 

    // Precompute B^(M-1) % q
    long long B_M_minus_1 = 1;
    for (int i = 0; i < M - 1; i++) {
        B_M_minus_1 = (B_M_minus_1 * B) % q;
    }

    // Compute initial hashes for the pattern and the first text window
    long long h_pattern = 0;
    long long h_text = 0;
    for (int i = 0; i < M; i++) {
        h_pattern = (h_pattern * B + pattern[i]) % q;
        h_text = (h_text * B + text[i]) % q;
    }

    // Slide the window
    for (int i = 0; i <= N - M; i++) {
        
        // If hashes match, verify character by character (Las Vegas guarantee)
        if (h_pattern == h_text) {
            bool match = true;
            for (int j = 0; j < M; j++) {
                if (text[i + j] != pattern[j]) {
                    match = false;
                    break;
                }
            }
            if (match) return i; 
        }

        // Roll the hash forward
        if (i < N - M) {
            long long removed = (text[i] * B_M_minus_1) % q;
            h_text = (h_text - removed + q) % q; 
            h_text = (h_text * B + text[i + M]) % q;
        }
    }
    return -1;
}