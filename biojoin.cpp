#include<iostream>
#include<fstream>
#include<getopt.h>
#include<list>
#include<string>
#include<vector>
#include<map>
#include<string_view>
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
std::multimap<std::string, std::vector<std::string>> build_dictiorany(std::ifstream &input_file1, int colum_key){
    std::string line;
    std::multimap<std::string, std::vector<std::string>>key_values;

    while (std::getline(input_file1, line)) {
        if (line[0] == '#')
            continue;
        std::vector<std::string>substrings;
        for (auto part : line | std::views::split('\t')) {
            // emplace_back insert element to a vector like push_back but a little bit faster as 
            // the string is constructed inside the vector. 
            // https://www.geeksforgeeks.org/cpp/vectoremplace_back-c-stl/
            substrings.emplace_back(part.begin(), part.end());
        }
        substrings.emplace_back(line);
        std::string key =  substrings[colum_key];
        key_values.insert({key, substrings});
    }

    return key_values;
}

int main(int argc, char *argv[]){
    char c;
    int processes_to_use = 1;
    int key_field = 0;
    int key_field2 = 0;
    int hflag = 0;

    const char help[] =
            "Usage: reads_count++ [OPTIONS]... BAM_file GFF3_file\n"
            "\n"
            "Options:\n"
            "  -h        show help options\n"
            "  -f <n>    field from first file to be used as key\n"
            "  -g <n>    field from second file to be used as key\n"
            "  -p <n>    set processors (default 1)\n";

    while ((c = getopt (argc, argv, "hf:g:p:")) != -1){
        switch (c) {
            case 'h':
                hflag = 1;
                puts(help);
                return 1;
            case 'f':
                key_field = atoi(optarg);
                break;
            case 'g':
                key_field2 = atoi(optarg);
                break;
            case 'p':
                processes_to_use = atoi(optarg);
                if (processes_to_use < 1) processes_to_use = 1;
                break;
            case '?':
                if (isprint(optopt))
                    fprintf(stderr, "Unknown option `-%c'.\n", optopt);
                else
                    fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
                return 1;
            default:
                abort();
        }
    }

    argc -= optind;
    argv += optind;

    /* Check if there is a BAM and a GFF3 file */
    if (argc < 2) {
        if (argc == 0)
            std::cout << "Not foud first file for join\n";
        if (argc == 1)
            std::cout << "Not foud second file for join\n";
        return -1;
    }

    // Check if can open the first file
    std::ifstream input_file1(argv[0]);
    if (!input_file1.is_open()) {
        std::cerr << "Error: cannot open file: " << argv[0] << std::endl;
        return 1;
    }

    std::multimap<std::string, std::vector<std::string>> multi_map = build_dictiorany(input_file1, key_field);

    // Check if can open the second file
    std::ifstream input_file2(argv[1]);
    if (!input_file2.is_open()) {
        std::cerr << "Error: cannot open file: " << argv[1] << std::endl;
        return 1;
    }

    std::string line;
    while (std::getline(input_file2, line)) {
        if (line[0] == '#')
            continue;
        std::vector<std::string>substrings;
        for (auto part : line | std::views::split('\t')) {
            substrings.emplace_back(part.begin(), part.end());
        }
        std::string key =  substrings[key_field2];

        // Equal_range returns std::pair
        // in this case the key and the value
        auto range = multi_map.equal_range(key);
        // To see all the std::pair
        for (auto it = range.first; it != range.second; ++it){
            std::cout << it->second.back() <<  "\t" << line << std::endl;
        }
    }

    return 0;
}
