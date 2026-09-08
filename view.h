#ifndef VIEW_H
#define VIEW_H

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

/* Validates that the provided file is a valid MP3 file */
int validate_view_arg(char *argv[]);

/* Reads and displays all ID3 tags from an MP3 file */
int view_tag_reader(char *sample);

/* Converts big-endian byte format to little-endian integer */
int conevrt_BigTOLittle_Endien(char *size_of_tag);
#endif
