#include<iostream>
#include<fstream>
#include<getopt.h>
#include<string>
#include<vector>
#include<unordered_map>
#include<ranges>
#include<algorithm>
#include "create_dictionary.h"

int main(int argc, char *argv[]){
    char c;
    int processes_to_use = 1;
    std::string key_field = "";
    std::string key_field2 = "";
    std::string separator1 = "\t";
    std::string separator2 = "\t";
    std::string separator_user_wants = "\t";
    std::vector<int> bed_fields = {0,1,2};
    bool is_bed = false;

    const char help[] =
            "Usage: reads_count++ [OPTIONS]... file1 file2\n"
            "\n"
            "Options:\n"
            "  -h        show help options\n"
            "  -b        the first input file is a BED\n"
            "  -f <n>    field from first file to be used as key\n"
            "  -s <n>    field from second file to be used as key\n"
            "  -d <c>    The field separator string in the first file argument (default tab)\n"
            "  -e <c>    The field separator string in the second file argument (default tab)\n"
            "  -o <c>    The field separator the user wants in the output (default tab)\n"
            "  -p <n>    set processors (default 1)\n";

    while ((c = getopt (argc, argv, "hbf:s:d:p:e:o:")) != -1){
        switch (c) {
            case 'h':
                puts(help);
                return 1;
            case 'b':
                is_bed = true;
                break;
            case 'f':
                key_field = optarg;
                break;
            case 's':
                key_field2 = optarg;
                break;
            case 'p':
                processes_to_use = atoi(optarg);
                if (processes_to_use < 1) processes_to_use = 1;
                break;
            case 'd':
                if (optarg != NULL)
                    separator1 = optarg;
                break;
            case 'e':
                if (optarg != NULL)
                    separator2 = optarg;
                break;
            case 'o':
                if (optarg != NULL)
                    separator_user_wants = optarg;
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

    char separator_as_char1 = separator1.empty() ? '\t' : separator1[0];
    char separator_as_char2 = separator2.empty() ? '\t' : separator2[0];
    char separator_user_wants_as_char = separator_user_wants.empty() ? '\t' : separator_user_wants[0];


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

    std::vector<int> columns_for_key = create_wanted_key(key_field);
    std::vector<int> columns_for_key2 = create_wanted_key(key_field2);

    if (is_bed == true){
        bed_fields.insert(bed_fields.end(), columns_for_key.begin(), columns_for_key.end());
        columns_for_key = bed_fields;
    }

    std::unordered_multimap<std::string, std::vector<std::string>> multi_map = build_dictiorany(input_file1, columns_for_key, separator_as_char1);

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
        for (auto part : line | std::views::split(separator_as_char2)) {
            substrings.emplace_back(part.begin(), part.end());
        }
        // Loop that constructs the key using the -g parameter after it is parsed by the create_wanted_key function
        std::string second_file_key = "";
        for (auto i: columns_for_key2) {
            second_file_key = second_file_key + substrings[i];
        }

        // Equal_range returns std::pair
        // in this case the key and the value
        auto range = multi_map.equal_range(second_file_key);
        // To see all the std::pair
        for (auto it = range.first; it != range.second; ++it){
            if (separator_as_char1 != '\t'){
                std::replace(it->second.back().begin(), it->second.back().end(), separator_as_char1, separator_user_wants_as_char);
            }
            if (separator_as_char2 != '\t')
                std::replace(line.begin(), line.end(), separator_as_char2, separator_user_wants_as_char);

            std::cout << it->second.back() << separator_user_wants_as_char << line << std::endl;
        }
    }

    return 0;
}
