#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int read_ppm(char* filename, unsigned char* buffer, int buffer_size)
{
    int size = 0;
    FILE* file = fopen(filename, "rb");

    if (file == nullptr)
        return 0;

    int c;
    int version;

    int PARSE_LENGTH = 8;
    int parse_index = 0;
    char parse_buffer[PARSE_LENGTH];
    
    int width, height, max_value;

    // Must start with 'P'
    if ((c = fgetc(file)) != EOF && c != 'P')
        return 0;
    // Version Number
    if ((c = fgetc(file)) != EOF && !isdigit((char)c))
        return 0;
    else
        version = (char)c - '0';
    // Remove whitespace
    while ((c = fgetc(file)) != EOF && isspace(c)){}

    // Width
    if (!isdigit((char)c))
        return 0;
    do {
        parse_buffer[parse_index] = (char)c;
        parse_index++;
    } while ((c = fgetc(file)) != EOF && isdigit((char)c) && parse_index < PARSE_LENGTH - 1);

    parse_buffer[parse_index] = '\0';
    width = atoi(parse_buffer);
    parse_index = 0;

    // Remove whitespace
    while ((c = fgetc(file)) != EOF && isspace(c)){}

    // Height
    if (!isdigit((char)c))
        return 0;
    do {
        parse_buffer[parse_index] = (char)c;
        parse_index++;
    } while ((c = fgetc(file)) != EOF && isdigit((char)c) && parse_index < PARSE_LENGTH - 1);
    
    parse_buffer[parse_index] = '\0';
    height = atoi(parse_buffer);
    parse_index = 0;

    // Early out if we know we can't fit at the destination
    if (width * height * 3 > buffer_size)
        return 0;

    // Remove whitespace
    while ((c = fgetc(file)) != EOF && isspace(c)){}

    // Max color value
    if (!isdigit((char)c))
        return 0;
    do {
        parse_buffer[parse_index] = (char)c;
        parse_index++;
    } while ((c = fgetc(file)) != EOF && isdigit((char)c) && parse_index < PARSE_LENGTH - 1);
    
    parse_buffer[parse_index] = '\0';
    max_value = atoi(parse_buffer);
    parse_index = 0;

    //printf("P%d\n", version);
    //printf("%d %d\n", width, height);
    //printf("%d\n", max_value);

    fpos_t pos;
    // Remove whitespace
    do
    {
        fgetpos(file, &pos);        
    } while ((c = fgetc(file)) != EOF && isspace(c));

    fsetpos(file, &pos);

    size = fread(buffer, sizeof(buffer[0]), width * height * 3, file);

    //printf("%d\n", size);

    if (size > buffer_size)
        return 0;
    
    if (ferror(file))
        return 0;

    /*
    if (feof(file))
        printf("EOF\n");
    */
  
    fclose(file);

    return size;
}

int write_ppm(char* filename, uint8_t* buffer, int height, int width)
{
    FILE* f = fopen(filename, "wb");
    fprintf(f, "P6\n%d %d\n%d\n", height, width, 255);
    fwrite(buffer, 1, height*width*3, f);
    fclose(f);

    return 1;
}
