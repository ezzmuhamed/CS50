#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define Block_Size 512
typedef uint8_t BYTE ;

int main (int argc , char *argv[])
{
    if(argc != 2 )
    {
        fprintf(stderr,"Usage : recover file \n ");
        return 1 ;
    }

    char * infile = argv[1];
    FILE * inptr = fopen(infile,"r");
    if(inptr == NULL)
    {
        fprintf(stderr,"Can't open a file %s \n",infile);
        return 2 ;
    }

    BYTE Buffer[512];
    int imagecount = 0 ;
    char filename[8];
    FILE * outptr = NULL ;

    while(true)
    {
        size_t ByteReads = fread(Buffer,sizeof(BYTE),Block_Size,inptr);

        if(ByteReads == 0 && feof(inptr))
        {
            break;
        }

        bool containsJpegHeader = Buffer[0] == 0xff && Buffer[1] == 0xd8 && Buffer[2] == 0xff && (Buffer[3] & 0xf0) == 0xe0;
        if (containsJpegHeader && outptr != NULL)
        {
            fclose(outptr);
            imagecount++;
        }
        if(containsJpegHeader)
        {
            sprintf(filename ,"%3i.jpg",imagecount);
            outptr = fopen(filename,"w");
        }
        if (outptr != NULL)
        {
            fwrite(Buffer, sizeof(BYTE), ByteReads, outptr);
        }
    }

    // close last jpeg file
    fclose(outptr);

    // close infile
    fclose(inptr);

    // success
    return 0;
}
