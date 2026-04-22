#include <stdio.h>
#include <ctype.h>
#include <string>
#include <fstream>
#include <sstream>
#include <time.h>
#include <algorithm>
#include <string_view>


void normalize_string(std::string &input){
    //Function:
    //Removes chars outside 32-126 range
    //Converts sequences of whitespaces into single space
    //changes all chars to lowercase
    //converts interpunction to comas
    //converts repeatings of the same word into one occurance eg. hello hello hello world -> hello world

    // Remove chars outside printable ASCII range (32-126)

    // Convert whitespace -> space, punctuation -> comma, letters -> lowercase
    std::transform(input.begin(), input.end(), input.begin(),
        [](unsigned char c) -> char {
            if (isspace(c))  return ' ';
            if (ispunct(c))  return ',';
            return (char)tolower(c);
        });
    
    input.resize(
        std::remove_if(input.begin(), input.end(),
            [](unsigned char c){ return c < 32 || c > 126; })
        - input.begin());

    input.resize(
        std::unique(input.begin(), input.end(),
            [](char a, char b){ return a == ' ' && b == ' '; })
        - input.begin());

    // Remove consecutive duplicate words in-place using std::find / std::copy
    auto word_begin = input.begin();
    auto write_end  = input.begin();
    std::string_view last_word;

    while (word_begin != input.end()) {
        auto word_end = std::find(word_begin, input.end(), ' ');
        std::string_view current(&*word_begin, (size_t)(word_end - word_begin));

        if (current != last_word) {
            if (write_end != input.begin()) *write_end++ = ' ';
            auto new_word_start = write_end;
            write_end = std::copy(word_begin, word_end, write_end);
            last_word = std::string_view(&*new_word_start, current.size());
        }

        word_begin = word_end;
        if (word_begin != input.end()) ++word_begin; // skip space
    }
    input.erase(write_end, input.end());
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
        normalize_string(input);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("%.6f\n", elapsed);

    //size_t start_pos = input.size() > 1000 ? input.size() - 1000 : 0;
    //printf("%s\n", input.substr(start_pos).c_str());
    return 0;

}