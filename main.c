#include<stdio.h>
#include<string.h>
#include "view.h"
#include "edit.h"

/*
 * Main function - Entry point for the program
 * Argc: Number of command-line arguments
 * Argv: Array of command-line arguments
 * Returns: 0 on success, 1 on error
 * 
 * Usage:
 *   -v flag: View MP3 tags
 *   -e flag: Edit MP3 tags (requires option, value, and filename)
 */
int main(int argc,char *argv[]){

    // Handle view mode: -v flag with 2 arguments (program name and filename)
    if(argc==3){

        if(strcmp(argv[1],"-v")==0){

            // Validate that the file is an MP3 file
            if(validate_view_arg(argv)==1){
                printf("Invalid Input\n");
                return 1;
            }
            // Read and display all ID3 tags from the MP3 file
            view_tag_reader(argv[2]);
        }
        else{
            printf("Use This Command To View Content :\n");
            printf("./a.out -v sample.mp3\n\n");
        }

    }
    // Handle edit mode: -e flag with 4 arguments (program, flag, option, value, filename)
    else if(argc==5){

        if(strcmp(argv[1],"-e")==0){
            // Validate edit arguments
            if(validate_edit_arg(argv)==1){
                printf("Invalid Input\n");
                return 1;
            }
            // Edit the specified ID3 tag with the new value
            edit_tag(argv[2],argv[3],argv[4]);
        }
        else{
            printf("Use : ./a.out -e <tag option> <new name> sample.mp3");
        }

    }
    else{
        printf("\nUse This commands : \n");
        printf("For Displaying Content :\n");
        printf("./a.out -v sample.mp3\n\n");
        printf("For Edit :\n");
        printf("./a.out -e -t \"Enter_title\" sample.mp3\n");
        printf("./a.out -e -y \"Enter_year\" sample.mp3\n");
        printf("./a.out -e -a \"Enter_artist\" sample.mp3\n");
        printf("./a.out -e -A \"Enter_album\" sample.mp3\n");
        printf("./a.out -e -g \"Enter_content\" sample.mp3\n");
        printf("./a.out -e -c \"Enter_comments\" sample.mp3\n");
    }
    
    return 0;
}
