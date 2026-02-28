#include<iostream>
#include<fstream>
#include<getopt.h>
#include<string>
#include<vector>
#include<unordered_map>
#include<ranges>
#include<algorithm>
#include "create_dictionary.h"

#define GTF 1
#define NO_GTF 0

void run_biojoin(int argc, char *argv[]) {
    char c;
    int processes_to_use = 1;
    std::string key_field = "";
    std::string key_field2 = "";
    std::string separator1 = "\t";
    std::string separator2 = "\t";
    std::string separator_user_wants = "\t";
    std::vector<int> bed_fields = {0,1,2};
    std::vector<int> bed_fields2 = {0,1,2};
    std::vector<int> gtf_fields = {0,3,4};
    std::vector<int> gtf_fields2 = {0,3,4};
    bool is_bed = false;
    bool is_bed2 = false;
    bool is_gtf = false;
    bool is_gtf2 = false;
    std::vector<std::string> joins = {"INNER", "LEFT_OUTER", "RIGHT_OUTER", "FULL"};
    std::string join = "INNER";

    const char help[] =
            "Usage: reads_count++ [OPTIONS]... file1 file2\n"
            "\n"
            "Options:\n"
            "  -h            show help options\n"
            "  -b            the first input file is a BED\n"
            "  -B            the second input file is a BED\n"
            "  -g            the first input file is a GTF\n"
            "  -G            the second input file is a GTF\n"
            "  -f <n>        field from first file to be used as key\n"
            "  -s <n>        field from second file to be used as key\n"
            "  -d <c>        The field separator string in the first file argument (default tab)\n"
            "  -e <c>        The field separator string in the second file argument (default tab)\n"
            "  -j <string>   The type of join to do: INNER, LEFT (OUTER), RIGHT (OUTER), FULL\n"
            "  -o <c>        The field separator the user wants in the output (default tab)\n"
            "  -p <n>        set processors (default 1)\n";

    while ((c = getopt (argc, argv, "hbBgGf:s:d:p:e:j:o:")) != -1){
        switch (c) {
            case 'h':
                puts(help);
                return;
            case 'b':
                is_bed = true;
                break;
            case 'B':
                is_bed2 = true;
                break;
            case 'g':
                is_gtf = true;
                break;
            case 'G':
                is_gtf2 = true;
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
            case 'j':
                join = optarg;
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
                return;
            default:
                abort();
        }
    }

    argc -= optind;
    argv += optind;

    // Warning in the case of wrong join (-j)
    if (!std::ranges::contains(joins, join)){
        throw std::invalid_argument("The choosen JOIN using -j is not among these: INNER, LEFT (OUTER), RIGHT (OUTER), FULL");
    }

    char separator_as_char1 = separator1.empty() ? '\t' : separator1[0];
    char separator_as_char2 = separator2.empty() ? '\t' : separator2[0];
    char separator_user_wants_as_char = separator_user_wants.empty() ? '\t' : separator_user_wants[0];
    
    // Warnings in the case of the first file being a BED
    if (is_bed == true && key_field != "" )
        throw std::invalid_argument("If first input file is tagged as BED with -b the -f, field from first file to be used as key, should not be used");

    // Warnings in the case of the second file being a BED
    if (is_bed2 == true && key_field2 != "" )
        throw std::invalid_argument("If second input file is tagged as BED with -B the -s, field from first file to be used as key, should not be used");


    // Warnings in the case of the first file being a GTF
    if (is_gtf == true && key_field != "" )
        throw std::invalid_argument("If first input file is tagged as GTF with -g the -f, field from first file to be used as key, should not be used");

    // Warnings in the case of the second file being a GTF
    if (is_gtf2 == true && key_field != "" )
        throw std::invalid_argument("If second input file is tagged as GTF with -G the -f, field from first file to be used as key, should not be used");

    // Check if there are the 2 input files
    if (argc < 2) {
        if (argc == 0)
            std::cout << "Not foud first file for join\n";
        if (argc == 1)
            std::cout << "Not foud second file for join\n";
        throw std::invalid_argument("Two input files are required");
    }

    // Check if can open the first file
    std::ifstream input_file1(argv[0]);
    if (!input_file1.is_open()) {
        std::string error =  "cannot open file: ";
        std::string path = argv[0];
        error.append(path);
        throw std::runtime_error(error);
    }

    std::vector<int> columns_for_key = create_wanted_key(key_field);
    std::vector<int> columns_for_key2 = create_wanted_key(key_field2);

    if (is_bed == true){
        bed_fields.insert(bed_fields.end(), columns_for_key.begin(), columns_for_key.end());
        columns_for_key = bed_fields;
    }

    if (is_bed2 == true){
        bed_fields2.insert(bed_fields2.end(), columns_for_key2.begin(), columns_for_key2.end());
        columns_for_key2 = bed_fields2;
    }

    if (is_gtf == true){
        gtf_fields.insert(gtf_fields.end(), columns_for_key.begin(), columns_for_key.end());
        columns_for_key = gtf_fields;
    }

    if (is_gtf2 == true){
        gtf_fields2.insert(gtf_fields2.end(), columns_for_key2.begin(), columns_for_key2.end());
        columns_for_key2 = gtf_fields2;
    }

    std::unordered_multimap<std::string, std::vector<std::string>> multi_map = build_dictiorany(input_file1, columns_for_key, separator_as_char1, is_gtf ? true : false);

    // The unordered map that will contain the matched elements
    std::unordered_map<std::string, bool> matched;
    for (const auto& pair : multi_map) {
            matched[pair.first] = false;
    }

    // Check if can open the second file
    std::ifstream input_file2(argv[1]);
    if (!input_file2.is_open()) {
        std::string error =  "cannot open file: ";
        std::string path = argv[1];
        error.append(path);
        throw std::runtime_error(error);
    }

    std::string line;
    while (std::getline(input_file2, line)) {
        if (line[0] == '#')
            continue;

        // Split the line using the separator (\t the default)
        // substrings contains the elemente of the string
        std::vector<std::string>substrings;
        for (auto part : line | std::views::split(separator_as_char2)) {
            substrings.emplace_back(part.begin(), part.end());
        }
        // Loop that constructs the key using the -g parameter after it is parsed by the create_wanted_key function
        std::string second_file_key = "";
        for (auto i: columns_for_key2) {
            if (i < 0 || i >= static_cast<int>(substrings.size())) {
                throw std::out_of_range("Column index out of range in second file (check -e or -s)");
            }
            if (is_gtf2 == true && i == 3 && is_gtf == true){
                auto as_integer = std::to_string(stoi(substrings[i]) - 1);
                second_file_key = second_file_key + as_integer;
            }
            else if (is_gtf2 == true && i == 3 && is_gtf == false){
                second_file_key = second_file_key + substrings[i];
            }
            else if (is_gtf2 == true && i != 3){
                second_file_key = second_file_key + substrings[i];
            }
            else if (is_gtf2 == false) {
                second_file_key = second_file_key + substrings[i];
            }
        }

        // Equal_range returns std::pair
        // in this case the key and the value
        auto range = multi_map.equal_range(second_file_key);

        if (join == "LEFT_OUTER"){
            if (range.first != range.second){
                for (auto it = range.first; it != range.second; ++it) {
                    matched[it->first] = true;
                    // Exclusive LEFT JOIN are the default
                    //std::cout << it->second.back() 
                    //<< separator_user_wants_as_char
                    //<< line
                    //<< std::endl;
                }
            }
        }
        else{
            // To see all the std::pair
            for (auto it = range.first; it != range.second; ++it){

                std::replace(it->second.back().begin(), it->second.back().end(), separator_as_char1, separator_user_wants_as_char);

                std::replace(line.begin(), line.end(), separator_as_char2, separator_user_wants_as_char);

                std::cout << it->second.back() << separator_user_wants_as_char << line << std::endl;
            }

        }

    }

    // Exclusive LEFT JOIN are the default
    if (join == "LEFT_OUTER") {
        for (auto& pair : multi_map)
            if (!matched[pair.first]) {
                std::replace(pair.second.back().begin(), pair.second.back().end(), separator_as_char1, separator_user_wants_as_char);
                std::cout << pair.second.back() << std::endl;

            }
        }
    //}
}

int main(int argc, char *argv[]){
    try {
        run_biojoin(argc, argv);
    }
    catch (const std::exception& e){
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
}
