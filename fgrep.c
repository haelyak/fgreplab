/*
 * Fgrep (lab 6).
 *
 * Team members:
 *
 *      Kayleah Tsai <ktsai@cs.hmc.edu>
 *      Eric Chen <erchen@cs.hmc.edu>
 */

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int iFlag;
int lFlag;
int nFlag;
int qFlag;
bool hasFiles;
char* pattern;


int  main(int argc, char *argv[]);
int fgrep(FILE* stream, char* pattern, char* fileName, size_t printName);
char* get_full_line(char* buf, int nchars, int currentSize, FILE* stream, int* end);


int main(int argc, char *argv[])
{
    int i;
    int status=1;
    int error = 0;
    size_t printName;
    i = 1;

    // checking for flags
    while (argv[i][0] == '-') {
        char flag = argv[i][1];
        
        switch(flag) {
            case 'i':
                iFlag = 1;
                break;
            case 'l':
                lFlag = 1;
                break;
            case 'n':
                nFlag = 1;
                break;
            case 'q':
                qFlag = 1;
                break;
            default :
                // If the flag doesn't exist
                fprintf(stderr, "Usage: fgrep [-i] [-l] [-n] [-q] pattern [files]\n");
                error = 2;
                return error;
        }
        i++;
    }

    // getting pattern
    if (i >= argc) {
        fprintf(stderr, "Usage: fgrep [-i] [-l] [-n] [-q] pattern [files]\n");
        error = 2;
        return error;
    }

    pattern = argv[i];
    i++;

    // searching in each file
    printName = !(argc-i == 1);

    while (i < argc) {
        hasFiles = true; 
        FILE* stream = fopen(argv[i], "r");
        if (stream == NULL){
            fprintf(stderr, "Couldn't open '%s': No such file or directory\n", argv[i]);
            error = 1;
        } else{
            status = fgrep(stream, pattern, argv[i], printName) && status;
        }
        i++;
    }
    
    if (!hasFiles) {
        status = fgrep(stdin, pattern, argv[i], printName) && status;
    }

    return status || error;
}
  
// searches in the file stream for a given pattern
int fgrep(FILE* stream, char* pattern, char* fileName, size_t printName) {
    char *buf;
    size_t n;
    int compare;
    int lineNumber = 0;
    int status = 1;
    size_t currentSize = 2;
    int end;
    buf = malloc(currentSize);

    if (buf == NULL) {
        printf("Out of memory!");
        return status;
    }

    n = strlen(pattern);
    end = 0;

    while(1) {
        
        lineNumber++;
        free(buf);
        buf = malloc(currentSize);
        buf = get_full_line(buf, 0, currentSize, stream, &end);

        for (int i=0; i<strlen(buf) - n; i++) {
            // searching; if the -i flag is on, then caps don't matter
            if (iFlag ==1) {
               compare = strncasecmp(&buf[i], pattern, n);
            } 
            else if (iFlag == 0){
                compare = strncmp(&buf[i], pattern, n);
            }

            // printing our result based on flags
            status = status && compare;
            if (compare == 0) {
                // if the -q flag is on, don't print anything
                if(qFlag == 1){
                    return status;
                }

                // if the -l flag is on, then only need to print the file name
                else if (lFlag == 1){
                    if (fileName == NULL){
                        fileName = "(standard input)";
                    }
                    fprintf(stdout, "%s\n", fileName);
                    return status;
                }
                else{
                    // if the -n flag is on, then need to print line number
                    if (printName && fileName) {
                        fprintf(stdout, "%s:", fileName);
                    }
                    if (nFlag == 1){
                        printf("%d:",lineNumber);
                    }
                    fputs(buf, stdout);
                    fflush(stdout);
                    break;
                }

            }
        } 
        if (end == 1){
            break;
        }

    }
    return status;
}

// reads in a full line until either the new line character or EOF
char* get_full_line(char* buf, int nchars, int currentSize, FILE* stream, int* end) {
    char* result = fgets(buf+nchars, currentSize-nchars, stream);
    if (result == NULL) {
        *end = 1;
        return buf;
    }

    nchars = strlen(buf);

    if (buf[nchars-1] == '\n') {
        return buf;
    }
    else {
        // expanding buf if we run out of space.
        currentSize = 2*currentSize;
        buf = realloc(buf, currentSize);
        return get_full_line(buf, nchars, currentSize, stream, end);
    }
}
