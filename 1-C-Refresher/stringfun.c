#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
//add additional prototypes here
int  count_words(char *, int, int);
int print_words(char *, int, int);
void reverse_string(char *, int);

/**
* setup_buff - Processes the user input string and stores it in the buffer.
*
* buff: Buffer where the processed string will be stored.
* user_str: User-provided input string that will be processed.
* len: Length of the buffer.
*
* This function processes the input string by removing consecutive spaces,
* replacing them with a single space, and then ensuring that the buffer is
* filled with either the processed string or padding if the string
* is shorter than the buffer length.
*
* Return: The number of characters written to the buffer, or -1 if the buffer is too small to fit the processed string.
*/
int setup_buff(char *buff, char *user_str, int len){
    //TODO: #4:  Implement the setup buff as per the directions
    char *input_pointer = user_str;
    char *buffer_pointer = buff;
    int char_counter = 0;
    int is_whitespace = 1;

    while (*input_pointer != '\0') {
        if (char_counter >= len) {
            return -1;
        }

        if (*input_pointer == ' ' || *input_pointer == '\t') {
            if (!is_whitespace) {
                *buffer_pointer++ = ' ';
                char_counter++;
                is_whitespace = 1;
            }
        } else {
            *buffer_pointer++ = *input_pointer;
            char_counter++;
            is_whitespace = 0;
        }
        input_pointer++;
    }

    while (char_counter < len) {
        *buffer_pointer++ = '.';
        char_counter++;
    }
    return char_counter;
}

/**
* print_buff - Prints the characters in the buffer.
*
* buff: Pointer to the buffer.
* len: Length of the buffer.
*
* This function iterates over the entire buffer, printing each character one by one.
*/
void print_buff(char *buff, int len){
    printf("Buffer:  ");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    putchar('\n');
}

/**
* usage - Prints the usage information.
*
* exename: The name of the executable to be printed.
*
* This function prints a help message explaining how to use the program. 
*/
void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);
}

/**
* count_words - Counts the number of words in a given buffer.
*
* buff: Buffer containing the string.
* len: Length of the buffer.
* str_len: Length of the string in the buffer.
*
* This function iterates through the buffer and counts the number of words.
*
* Return: The total number of words in the buffer.
*/
int count_words(char *buff, int len, int str_len){
    int word_counter = 0;
    int word = 0;

    for (int i = 0; i < str_len; i++) {
        if (*(buff + i) != ' ') {
            if (!word) {
                word_counter++;
                word = 1;
            }
        } else {
            word = 0;
        }
    }
    return word_counter;
}

/**
* print_words - Prints words in the buffer along with their character counts.
*
* buff: Buffer containing the string.
* len: Length of the buffer.
* str_len: Length of the actual string.
*
* This function iterates through the buffer and prints each word on a new line, 
* followed by the number of characters in that word.
*
* Return: The total number of words printed.
*/
int print_words(char *buff, int len, int str_len) {
    int word_counter = 0;
    int char_counter = 0;
    int word = 0;

    printf("Word Print\n----------\n");

    for (int i = 0; i < str_len; i++) {
        char c = *(buff + i);

        if (c == '.') {
            break;
        }

        if (c != ' ') {
            if (!word) {
                word_counter++;
                printf("%d. ", word_counter);
                word = 1;
            }
            putchar(c);
            char_counter++;
        } else {
            if (word) {
                printf(" (%d)\n", char_counter);
                char_counter = 0;
                word = 0;
            }
        }
    }

    if (word) {
        printf(" (%d)\n", char_counter);
    }
    return word_counter;
}

/**
* reverse_string - Prints the reverse of the string given.
*
* buff: Pointer to the buffer that contains the string.
* str_len: Length of the string.
*
* Given a string, this function prints the reversed version. It skips over padding characters.
*/
void reverse_string(char *buff, int str_len) {
    printf("Reversed String: ");
    
    for (int i = str_len - 1; i >= 0; i--) {
        if (buff[i] != '.') {
            putchar(buff[i]);
        }
    }
    putchar('\n');
}

//ADD OTHER HELPER FUNCTIONS HERE FOR OTHER REQUIRED PROGRAM OPTIONS

int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    //ANSWER: This is safe because this block makes sure the program receives two arguments. Since it's checking for (argc < 2), it avoids a segmentation fault if argv[1] doesn't exist. If both of these conditions fail, usage instructions will be provided and the program will exit.
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    //      PLACE A COMMENT BLOCK HERE EXPLAINING
    //ANSWER: The purpose of the if statement is to check if there is a string after the option flag. If it's less than 3 arguements, it will provide usage instructions and exit. It also prevents accessing argv[2] if it doesn't exist.
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    // CODE GOES HERE FOR #3
    buff = (char *)malloc(BUFFER_SZ * sizeof(char));
    if (buff == NULL) {
        exit(99);
    }

    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d", user_str_len);
        exit(2);
    }

    switch (opt){
        case 'c':
            rc = count_words(buff, BUFFER_SZ, user_str_len);  //you need to implement
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;
        case 'r':
            reverse_string(buff, user_str_len);
            break;
        case 'w':
            print_words(buff, BUFFER_SZ, user_str_len);
            break;

        //TODO:  #5 Implement the other cases for 'r' and 'w' by extending
        //       the case statement options
        default:
            usage(argv[0]);
            exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    print_buff(buff,BUFFER_SZ);
    free(buff);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          PLACE YOUR ANSWER HERE
/*
ANSWER: It's good practice because providing the pointer allows the functions to know 
how many available characters are left regardless of actual buffer size which prevents buffer overflows.
The buffer size could also change in the future, so providing both will make sure the function remains correct regardless of buffer size change.
Overall, it allows the code to be more reusable and adaptable to different buffer sizes. 
*/