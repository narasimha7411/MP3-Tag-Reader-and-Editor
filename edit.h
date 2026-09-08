#ifndef EDIT_H
#define EDIT_H

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

/* Validates that the provided file is an MP3 file and arguments are correct */
int validate_edit_arg(char *argv[]);

/* Edits a specific ID3 tag with the new value in the MP3 file */
int edit_tag(char *option,char *new_name,char *filename);

/* Converts big-endian byte format to little-endian unsigned integer */
int big_to_littel_endien(unsigned char *tag_size);

/* Converts an unsigned integer to big-endian byte format (4 bytes) */
void convert_integer_bigEndien( unsigned int new_tag_size,unsigned char *new_size_int);
#endif
