#pragma once
#include<string>
#include<vector>
#include<unordered_map>
#include<fstream>

std::unordered_multimap<std::string, std::vector<std::string>> build_dictiorany(std::ifstream &, std::vector<int>, char);

std::vector<int> create_wanted_key(std::string);
