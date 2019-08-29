#include <stdio.h>
#include <string.h>

int main() {
    char str_a[20]; //a 20 element array
    char *pointer; // a pointer, meant for a character array
    char *pointer2; //another one

    strcpy(str_a, "Hello, world!\n");
    pointer = str_a; // set the first pointer to the start of the array
    printf(pointer); // should print a memory address

    pointer2 = pointer + 2; // set the second one 2 bytes further in
    printf(pointer2);
    strcpy(pointer2, "y you guys!\n"); //write to that spot
    printf(pointer); //print again
}