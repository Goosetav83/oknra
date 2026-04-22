#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>


size_t normalize_string(char* p, size_t n){
    //Function:
    //Removes chars outside 32-126 range
    //Converts sequences of whitespaces into single space
    //changes all chars to lowercase
    //converts interpunction to comas
    //converts repeatings of the same word into one occurance eg. hello hello hello world -> hello world

    size_t i = 0, j = 0, curr_word_size = 0;
    unsigned char c;

    char* last_word_buf = (char*)malloc(n + 1);
    size_t last_word_len = 0;

    while (i < n) {
        c = (unsigned char)p[i];

        if (isspace(c)) {
            if (j > 0 && p[j-1] != ' ') {
                if (curr_word_size > 0 && curr_word_size == last_word_len &&
                    memcmp(p + j - curr_word_size, last_word_buf, curr_word_size) == 0) {
                    j -= curr_word_size;
                } else {
                    memcpy(last_word_buf, p + j - curr_word_size, curr_word_size);
                    last_word_len = curr_word_size;
                    p[j++] = ' '; 
                }
                curr_word_size = 0;
            }
            i++;
            continue;
        }

        if (c < 32 || c > 126) { i++; continue; }
        if (ispunct(c)) c = ',';

        p[j++] = (char)tolower(c);
        curr_word_size++;
        i++;
    }

    if (curr_word_size > 0 && curr_word_size == last_word_len &&
        memcmp(p + j - curr_word_size, last_word_buf, curr_word_size) == 0) {
        j -= curr_word_size;
    }

    free(last_word_buf);
    p[j] = '\0';
    return j;
}



int main(int argc, char *argv[]){
    const char* filename = argc > 1 ? argv[1] : "testfile1.txt";
    int iterations = argc > 1 ? 400 : 1;

    FILE* file = fopen(filename, "r");
    if(!file){
        fprintf(stderr, "Error: cannot open %s\n", filename);
        return 1;
    }

    fseek(file, 0, SEEK_END);
    size_t file_size = (size_t)ftell(file);
    fseek(file, 0, SEEK_SET);

    char* original = (char*)malloc(file_size + 1);
    size_t read_size = fread(original, 1, file_size, file);
    fclose(file);
    size_t original_size = read_size;
    original[original_size] = '\0';

    char* input = (char*)malloc(original_size + 1);
    size_t input_size;
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    for (int it = 0; it < iterations; it++) {
        memcpy(input, original, original_size + 1);
        input_size = normalize_string(input, original_size);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
    printf("%.6f\n", elapsed);

    //size_t start_pos = input_size > 1000 ? input_size - 1000 : 0;
    //printf("%.*s\n", (int)(input_size - start_pos), input + start_pos);

    free(original);
    free(input);
    return 0;
}