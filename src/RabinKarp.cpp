#include "RabinKarp.hpp"
#include <vector>
#include <random>
#include <chrono>

using namespace std;

int rabinKarpDeterministic(const string& text, const string& pattern, int* collisions) {
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

    int local_collisions = 0;
    
    for (int i = 0; i <= N - M; i++) {
        if (h_pattern == h_text) {
            bool match = true;
            for (int j = 0; j < M; j++) {
                if (text[i + j] != pattern[j]) {
                    match = false;
                    break;
                }
            }
            if (match) {
                if (collisions) *collisions += local_collisions;
                return i; 
            } else {
                local_collisions++; // Spurious match / false positive
            }
        }
        if (i < N - M) {
            long long removed = (text[i] * B_M_minus_1) % q;
            h_text = (h_text - removed + q) % q; 
            h_text = (h_text * B + text[i + M]) % q;
        }
    }
    if (collisions) *collisions += local_collisions;
    return -1;
}

int rabinKarpRandomizedBase(const string& text, const string& pattern, int* collisions) {
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

    int local_collisions = 0;
    for (int i = 0; i <= N - M; i++) {
        if (h_pattern == h_text) {
            bool match = true;
            for (int j = 0; j < M; j++) {
                if (text[i + j] != pattern[j]) {
                    match = false;
                    break;
                }
            }
            if (match) {
                if (collisions) *collisions += local_collisions;
                return i; 
            } else {
                local_collisions++; // Spurious match / false positive
            }
        }
        if (i < N - M) {
            long long removed = (text[i] * B_M_minus_1) % q;
            h_text = (h_text - removed + q) % q; 
            h_text = (h_text * B + text[i + M]) % q;
        }
    }
    return -1;
}

int rabinKarpRandomizedModulo(const string& text, const string& pattern, int* collisions) {
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

    int local_collisions = 0;
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
            if (match) {
                if (collisions) *collisions += local_collisions;
                return i; 
            } else {
                local_collisions++; // Spurious match / false positive
            }
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


int rabinKarpMonteCarlo(const string& text, const string& pattern, int* collisions) {
    int N = text.length();
    int M = pattern.length();
    if (M > N) return -1;
    if (M == 0) return 0;

    // Use two different large primes
    long long q1 = 1000000007; 
    long long q2 = 1000000009;

    // Randomly select two bases at runtime
    static thread_local mt19937 rng(random_device{}());
    uniform_int_distribution<long long> dist1(256, q1 - 1);
    uniform_int_distribution<long long> dist2(256, q2 - 1);
    
    long long B1 = dist1(rng); 
    long long B2 = dist2(rng); 

    // Precompute B1^(M-1) % q1 and B2^(M-1) % q2
    long long B1_M_minus_1 = 1;
    long long B2_M_minus_1 = 1;
    for (int i = 0; i < M - 1; i++) {
        B1_M_minus_1 = (B1_M_minus_1 * B1) % q1;
        B2_M_minus_1 = (B2_M_minus_1 * B2) % q2;
    }

    // Initial hashes for pattern and the first window of text
    long long h1_pattern = 0, h2_pattern = 0;
    long long h1_text = 0, h2_text = 0;
    for (int i = 0; i < M; i++) {
        h1_pattern = (h1_pattern * B1 + pattern[i]) % q1;
        h2_pattern = (h2_pattern * B2 + pattern[i]) % q2;
        
        h1_text = (h1_text * B1 + text[i]) % q1;
        h2_text = (h2_text * B2 + text[i]) % q2;
    }

    // Slide the window
    for (int i = 0; i <= N - M; i++) {
        // MONTE CARLO: If both hashes match, assume strings match without checking
        if (h1_pattern == h1_text && h2_pattern == h2_text) {
            return i; 
        }
        
        if (i < N - M) {
            // Update first hash
            long long removed1 = (text[i] * B1_M_minus_1) % q1;
            h1_text = (h1_text - removed1 + q1) % q1; 
            h1_text = (h1_text * B1 + text[i + M]) % q1;
            
            // Update second hash
            long long removed2 = (text[i] * B2_M_minus_1) % q2;
            h2_text = (h2_text - removed2 + q2) % q2; 
            h2_text = (h2_text * B2 + text[i + M]) % q2;
        }
    }
    return -1;
}



// Experiment function for detecting time taken to detect hash collision
int rabinKarpDeterministic_Timed(const string& text, const string& pattern, double* time_spent_verifying) {
    int N = text.length();
    int M = pattern.length();
    if (M > N) return -1;

    long long q = 1000000007; 
    long long B = 31; 

    long long B_M_minus_1 = 1;
    for (int i = 0; i < M - 1; i++) B_M_minus_1 = (B_M_minus_1 * B) % q;

    long long h_pattern = 0;
    long long h_text = 0;
    for (int i = 0; i < M; i++) {
        h_pattern = (h_pattern * B + pattern[i]) % q;
        h_text = (h_text * B + text[i]) % q;
    }

    double local_verification_time = 0.0;
    
    for (int i = 0; i <= N - M; i++) {
        if (h_pattern == h_text) {
            // START TIMING: Hash collision detected
            auto start = std::chrono::high_resolution_clock::now();
            
            bool match = true;
            for (int j = 0; j < M; j++) {
                if (text[i + j] != pattern[j]) {
                    match = false;
                    break;
                }
            }
            
            // STOP TIMING: Verification completed or failed
            auto end = std::chrono::high_resolution_clock::now();
            local_verification_time += std::chrono::duration<double>(end - start).count();

            if (match) {
                if (time_spent_verifying) *time_spent_verifying += local_verification_time;
                return i; 
            }
        }
        if (i < N - M) {
            long long removed = (text[i] * B_M_minus_1) % q;
            h_text = (h_text - removed + q) % q; 
            h_text = (h_text * B + text[i + M]) % q;
        }
    }
    
    if (time_spent_verifying) *time_spent_verifying += local_verification_time;
    return -1;
}