#include"create_dictionary.h"
#include<string>
#include<vector>
#include<unordered_map>
#include<fstream>
#include<ranges>

// This function construct a multimap
// As imput it takes an ifstream (the file to read) and the colum_key (the int of the column to be used as key)
// It returns a multimap with a string as key, and a vector as result.
// The vector last element is the entire line.
// Example:
//
// from this file:
//      123 Rambo Soldier
//      555 Jim Scientist
//      333 Jim CLover
//
// You get this: 
// 
// {"Rambo", ["123", "Rambo", "Soldier", "123 Rambo Soldier"]},
// {"Jim", ["555", "Jim", "Scientist, "Jim 555 Scientist"]}
// {"Jim", ["333", "Jim", "CLover, "Jim 555 CLover"]}
//std::unordered_multimap<std::string, std::vector<std::string>> build_dictiorany(std::ifstream &input_file1, int colum_key){
std::unordered_multimap<std::string, std::vector<std::string>> build_dictiorany(std::ifstream &input_file1, std::vector<int>columns_for_key, char separator){
    std::string line;
    std::unordered_multimap<std::string, std::vector<std::string>>key_values;

    while (std::getline(input_file1, line)) {
        if (line[0] == '#')
            continue;
        std::vector<std::string>substrings;
        for (auto part : line | std::views::split(separator)) {
            // emplace_back insert element to a vector like push_back but a little bit faster as 
            // the string is constructed inside the vector. 
            // https://www.geeksforgeeks.org/cpp/vectoremplace_back-c-stl/
            substrings.emplace_back(part.begin(), part.end());
        }
        substrings.emplace_back(line);

        std::string key;
        for (auto const& i: columns_for_key){
            key = key + substrings[i];
        }
        key_values.insert({key, substrings});
    }

    return key_values;
}

std::vector<int> create_wanted_key(std::string s){
    std::vector<std::string>wanted_key;
    std::vector<int>wanted_int_key;
    for (auto part : s | std::views::split(',')) {
        wanted_key.emplace_back(part.begin(), part.end());
    }

    for (const std::string& s : wanted_key) {
        wanted_int_key.push_back(std::stoi(s));
    }

    return wanted_int_key;
}


