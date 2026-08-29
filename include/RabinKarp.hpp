#pragma once
#include <string>

int rabinKarpDeterministic(const std::string& text, const std::string& pattern);
int rabinKarpRandomizedBase(const std::string& text, const std::string& pattern);
int rabinKarpRandomizedModulo(const std::string& text, const std::string& pattern);