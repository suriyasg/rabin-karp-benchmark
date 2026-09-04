#pragma once
#include <string>

int rabinKarpDeterministic(const std::string& text, const std::string& pattern, int* collisions = nullptr);
int rabinKarpRandomizedBase(const std::string& text, const std::string& pattern, int* collisions = nullptr);
int rabinKarpRandomizedModulo(const std::string& text, const std::string& pattern, int* collisions = nullptr);
int rabinKarpMonteCarlo(const std::string& text, const std::string& pattern, int* collisions = nullptr);
