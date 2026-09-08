#include "edit.h"

/*
 * Validates edit arguments
 * argv: Command-line arguments where argv[4] contains the filename
 * Returns: 0 if valid (MP3 file), 1 if invalid
 */
int validate_edit_arg(char *argv[]){

    char *txt = strrchr(argv[4],'.');

    if(txt == NULL || strcmp(txt,".mp3")!=0){

        printf("Error : Not an mp3 file\n");
        return 1;
    }
    return 0;
    
}
/*
 * Converts big-endian byte format to little-endian unsigned integer
 * tag_size: Pointer to 4 bytes in big-endian format
 * Returns: 32-bit unsigned integer value
 * Note: Combines bytes: [0]<<24 | [1]<<16 | [2]<<8 | [3]
 */
int big_to_littel_endien(unsigned char *tag_size){

    return (unsigned int)tag_size[0]<<24 |
            (unsigned int)tag_size[1]<<16|
            (unsigned int)tag_size[2]<<8|
            (unsigned int)tag_size[3];
}

/*
 * Converts an unsigned integer to big-endian byte format (4 bytes)
 * new_tag_size: Unsigned 32-bit integer to convert
 * new_size_int: Output array of 4 bytes in big-endian format
 * Note: Splits integer into bytes and stores in big-endian order
 */
void convert_integer_bigEndien( unsigned int new_tag_size,unsigned char *new_size_int){

    new_size_int[0]=(new_tag_size>>24) & 0xFF;  // Most significant byte
    new_size_int[1]=(new_tag_size>>16) & 0xFF;
    new_size_int[2]=(new_tag_size>>8 ) & 0xFF;
    new_size_int[3]=(new_tag_size    ) & 0xFF;  // Least significant byte
}
int edit_tag(char *option,char *new_name,char *filename){

    // Open source MP3 file for reading
    FILE *src=fopen(filename,"r");
    if(src == NULL){
        perror("");
        return 1;
    }

    // Create temporary file for writing modified content FILE *temp=fopen("tempFile.mp3","w");
    FILE *temp=fopen("tempFile.mp3","w");
    if(temp==NULL){
        fclose(src);
        return 1;
    }

    // Map command-line option to ID3 frame ID
    char tag_to_edit[5];

    if(strcmp(option,"-t")==0){
        strcpy(tag_to_edit,"TIT2");  // Title
    }
    else if(strcmp(option,"-y")==0){
        strcpy(tag_to_edit,"TYER");  // Year
    }
    else if(strcmp(option,"-A")==0){
        strcpy(tag_to_edit,"TALB");  // Album
    }
    else if(strcmp(option,"-a")==0){
        strcpy(tag_to_edit,"TPE1");  // Artist
    }
    else if(strcmp(option,"-g")==0){
        strcpy(tag_to_edit,"TCON");  // Genre
    }
    else if(strcmp(option,"-c")==0){
        strcpy(tag_to_edit,"COMM");  // Comment
    }
    else{
        printf("Edit is not supported for this option\n");
        fclose(src);
        fclose(temp);
        return 1;
    }
    
    // Copy ID3 header (first 10 bytes)
    char hearder[10];
    fread(hearder,1,10,src);
    fwrite(hearder,1,10,temp);
    
    // Process all ID3 frames
    char tag_id[5];
    while(1){
        // Read 4-byte frame ID
        fread(tag_id,1,4,src);
        tag_id[4]='\0';

        // Check for end of frames (null frame ID)
        if(tag_id[0]=='\0'){
            break;
        }

        // Read frame size (big-endian, 4 bytes)
        unsigned char tag_size[4];

        fread(tag_size,1,4,src);

        unsigned int frame_size = big_to_littel_endien(tag_size);

        // Read frame flags (2 bytes)
        char flag[2];
        fread(flag,1,2,src);

        // Read text encoding byte
        unsigned char encoded;
        fread(&encoded,1,1,src);
        
        // Decrease frame size for encoding byte already read
        if(frame_size>0){
            frame_size--;
        }    
        
        // Allocate and read frame data
        char *data=malloc(frame_size+1);
        if(data==NULL){
            fclose(src);
            fclose(temp);
            remove("temp.mp3");
            return 1;
        }
        fread(data,1,frame_size,src);
        data[frame_size]='\0';

        // Check if this is the frame we want to edit
        if(strcmp(tag_id,tag_to_edit)==0){
            // Write frame ID
            fwrite(tag_id,1,4,temp);

            // Calculate new frame size (new value + 1 for encoding byte)
            unsigned int new_tag_size=strlen(new_name)+1;

            // Convert new size to big-endian format
            unsigned char new_size_int[4];
            convert_integer_bigEndien(new_tag_size,new_size_int);
            
            // Write new size, flags, and encoding byte
            fwrite(new_size_int,1,4,temp);
            fwrite(flag,1,2,temp);

            // Write encoding (0 = ISO-8859-1)
            encoded=0;
            fwrite(&encoded,1,1,temp);
            
            // Write new tag value
            fwrite(new_name,1,strlen(new_name),temp);

        }
        else{
            // Frame not being edited - copy as-is
            fwrite(tag_id,1,4,temp);
            fwrite(tag_size,1,4,temp);
            fwrite(flag,1,2,temp);
            fwrite(&encoded,1,1,temp);
            fwrite(data,1,frame_size,temp);
        }
        free(data);

        // End loop at last frame marker
        if(strcmp(tag_id,"COMM")==0){ break;
        }
    }

    // Copy remaining file content (audio frames) byte by byte
    int ch;

    while ((ch=fgetc(src))!=EOF)
    {
        fputc(ch,temp);
    }
    
    // Close both files
    fclose(src);
    fclose(temp);

    // Replace original file with modified temporary file
    remove(filename);

    rename("tempFile.mp3",filename);

    printf("Tag Edited Successufly\n");
    return 0;
}
