#include <stdio.h>
//#include <unordered_set>
#include <ctype.h>
#include <string>
#include <fstream>
#include <sstream>
#include <time.h>


std::string normalize_string(std::string input){
    //Function:
    //Removes chars outside 32-126 range
    //Converts sequences of whitespaces into single space
    //changes all chars to lowercase
    //converts interpunction to comas
    //converts repeatings of the same world into one occurance eg. hello hello hello world -> hello world
    //isispace()
    std::string result = "";
    std::string current_word = "";
    std::string last_word = "";
    unsigned int n = input.size(), i = 0;
    unsigned char ascii_val;
    while(i<n){
        ascii_val = (unsigned char)input[i];
        if(isspace(ascii_val)){
            //printf("current word: %s last word: %s\n", current_word.c_str(), last_word.c_str());
            if(!result.empty() && result.back() != ' '){
                if(!current_word.empty() && current_word == last_word){
                    result.resize(result.size() - current_word.size());
                } else {
                    result += ' ';
                }
                last_word = current_word;
                current_word = "";
            }
            i++;
            continue;
        }

        if(ascii_val < 32 || ascii_val >126){
            i++; continue;
        }else if(ispunct(ascii_val)){
            ascii_val = ',';
        }
        current_word += tolower(ascii_val);
        result += tolower(ascii_val);
        i++;
        
    }
    if(!current_word.empty() && current_word == last_word){
            result.resize(result.size() - current_word.size());
        }
        
    return result;
}



int main(int argc, char *argv[]){
    const char* filename = argc > 1 ? argv[1] : "testfile1.txt";
    int iterations = argc > 1 ? 400 : 1;

    std::ifstream file(filename);
    if(!file.is_open()){
        fprintf(stderr, "Error: cannot open %s\n", filename);
        return 1;
    }
    std::ostringstream ss;
    ss << file.rdbuf();
    std::string original = ss.str();

    std::string input;
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int it = 0; it < iterations; it++) {
        input = original;
        input = normalize_string(input);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("%.6f\n", elapsed);

    //size_t start_pos = input.size() > 1000 ? input.size() - 1000 : 0;
    //printf("%s\n", input.substr(start_pos).c_str());
    return 0;

}