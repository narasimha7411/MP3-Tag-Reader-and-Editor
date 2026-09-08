#include "view.h"

/*
 * Validates that the provided file is a valid MP3 file
 * argv: Command-line arguments where argv[2] contains the filename
 * Returns: 0 if valid MP3 file, 1 if invalid
 */
int validate_view_arg(char *argv[]){

    char *txt = strrchr(argv[2],'.');

    if(txt == NULL || strcmp(txt,".mp3")!=0){

        printf("Error : Not an mp3 file\n");
        return 1;
    }
    return 0;
}
/*
 * Converts big-endian byte format to little-endian integer
 * size_of_tag: Pointer to 4 bytes in big-endian format
 * Returns: 32-bit unsigned integer in little-endian format
 * Note: Shifts bytes from positions [0][1][2][3] to construct the integer
 */
int conevrt_BigTOLittle_Endien(char *size_of_tag){
   

return ((unsigned int)size_of_tag[0]<<24)|
        ((unsigned int)size_of_tag[1]<<16)|
        ((unsigned int)size_of_tag[2]<<8)|
        (unsigned int)size_of_tag[3];

}
/*
 * Reads and displays all ID3 tags from an MP3 file
 * sample: Path to the MP3 file to read

 * Displays: Title, Year, Artist, Album, Genre, and Comments if present
 */
int view_tag_reader(char *sample){

    FILE *fp=fopen(sample,"r");

    if(fp==NULL){
        printf("Error : Unable to open file\n");
        return 1;
    }
    
    // Read and verify ID3 tag header (first 3 bytes should be "ID3")
    char id[4];
    fread(id,1,3,fp);
    id[3]='\0';

    if(strcmp(id,"ID3")!=0){
        printf("ID3 tag is not found\n");
        fclose(fp);
        return 1;
    }
    // Read ID3 version information
    unsigned char version_nb[2];

    fread(version_nb,1,2,fp);
    printf("\n");
    printf("----------------------------------------\n");
    printf("MP3 TAG READER AND EDITOR\n");
    printf("----------------------------------------\n");

    printf("ID3 version : 2.%d.%d\n", version_nb[0], version_nb[1]);

    // Skip flags and tag size (5 bytes)
    fseek(fp,5,SEEK_CUR);

    // Loop through all ID3 frames in the tag
    while(1){

        // Read 4-byte frame ID
        char tag_id[5];
        if(fread(tag_id,1,4,fp)!=4){
            break;
        }
        tag_id[4]='\0';
        
        // Check for null terminator (end of tags)
        if(tag_id[0]=='\0'){
            break;
        }

        // Read 4-byte frame size (big-endian format)
        unsigned char size_of_tag[4];
        fread(size_of_tag,1,4,fp);

        unsigned int size_of_frame= conevrt_BigTOLittle_Endien((char*)size_of_tag);
        
        // Skip frame flags (2 bytes)
        fseek(fp,2,SEEK_CUR);

        // Read text encoding byte
        unsigned char encoded_data;
        fread(&encoded_data,1,1,fp);

        // Decrease frame size to account for encoding byte
        size_of_frame--;

        // Allocate memory for frame data
        char *tag_data=malloc(size_of_frame + 1);

        if(tag_data==NULL){
            fclose(fp);
            return 1;
        }
        
        // Read frame data and null-terminate
        fread(tag_data,1,size_of_frame,fp);
        tag_data[size_of_frame]='\0';

        // Display tag data based on frame ID
        if(strcmp(tag_id,"TIT2")==0){
            printf("Title       : %s\n",tag_data);
        }
        if(strcmp(tag_id,"TYER")==0){
            printf("Year        : %s\n",tag_data);
        }
        if(strcmp(tag_id,"TPE1")==0){
            printf("Artist      : %s\n",tag_data);
        }
        if(strcmp(tag_id,"TALB")==0){
            printf("Album       : %s\n",tag_data);
        }
        if(strcmp(tag_id,"TCON")==0){
            printf("Genre       : %s\n",tag_data);
        }
        if(strcmp(tag_id,"COMM")==0){
            printf("Comments    : %s\n",tag_data);
        }
    
        // Free allocated memory
        free(tag_data);
    }
    printf("----------------------------------------\n");
    fclose(fp);
    return 0;
}
