#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define SEGMENT 102 //approximate target size of small file

long file_size(char *name);//function definition below

int split(char *largeFileName)
{
    int segments=0, i, len, accum;
    FILE *fp1, *fp2;
    //char filename[260]={"/home/ijaju/Documents/uni-shit/pdc/LZW-master/para-compress/smallFileName_"};//base name for small files.
    //char largeFileName[]={"/home/ijaju/Documents/uni-shit/pdc/LZW-master/compress/test.txt"};//change to your path
    char filename[260];
    char smallFileName[260];
    char line[1080];
    
    long sizeFile = file_size(largeFileName);
    //segments = sizeFile/SEGMENT + 1;//ensure end of file
    segments = 4;

    fp1 = fopen(largeFileName, "r");
    if(fp1)
    {
        for(i=0;i<segments;i++)
        {
            accum = 0;
            sprintf(smallFileName, "%s%d.txt", filename, i);
            fp2 = fopen(smallFileName, "w");
            if(fp2)
            {
                while(fgets(line, 1080, fp1) && accum <= SEGMENT)
                {
                    accum += strlen(line);//track size of growing file
                    fputs(line, fp2);
                }
                fclose(fp2);
            }
        }
        fclose(fp1);
    }
    return 0;
}

/*long file_size(char *name);//function definition below
int main(void)
{
    int segments=0, i, len, accum;
    FILE *fp1, *fp2;
    char filename[260]={"/home/ijaju/Documents/uni-shit/pdc/LZW-master/compress/smallFileName_"};//base name for small files.
    char largeFileName[]={"/home/ijaju/Documents/uni-shit/pdc/LZW-master/compress/test.txt"};//change to your path
    char smallFileName[260];
    char line[1080];
    
    long sizeFile = file_size(largeFileName);
    
    printf("%ld\n",sizeFile/SEGMENT + 1);
}*/


long file_size(char *name)
{
    FILE *fp = fopen(name, "rb"); //must be binary read to get bytes

    long size=-1;
    if(fp)
    {
        fseek (fp, 0, SEEK_END);
        size = ftell(fp)+1;
        fclose(fp);
    }
    return size;
}
