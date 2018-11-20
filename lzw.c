#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "algorithms.c" // LZW compression
#include "split.c"
#include "mpi.h"

FILE *inputFile;
FILE *outputFile;

char *fileName;
int outputArray[500];


int main(int argc, char** argv) { 

    if (argc > 2) {
        if (strcmp(argv[1], "c") == 0) { // compression 
            inputFile = fopen(argv[2], "r");
      		
      		fileName = argv[2];
            long q = file_size(fileName);
            
            char * buffer = 0;
            buffer = (char*)malloc(q);
            if (buffer) {
            	fread(buffer,1,q,inputFile);
        
            }
            //fclose(fileName);
            
            int rank,size;
            MPI_Init(&argc,&argv);
            MPI_Comm_rank(MPI_COMM_WORLD,&rank);
            MPI_Comm_size(MPI_COMM_WORLD,&size);
            
            char inputString[q/size];
            int output[q];
            
            
            MPI_Scatter(buffer,q/size,MPI_BYTE,inputString,q/size,MPI_BYTE,0,MPI_COMM_WORLD);
            //printf("rank=%d\tbuffer=%s,inputString=%s\n",rank,buffer,inputString);
            
            /*outputFile = fopen(strcat(argv[2], ".lzw"), "w+b"); // binary write to output file 
            
            if (outputFile == NULL || inputFile == NULL) {
                printf("Can't open files\n'"); return 0;
            }*/
            
            compress(inputString, outputArray);
                
                        
            MPI_Gather(outputArray,q/size,MPI_INT,output,q/size,MPI_INT,0,MPI_COMM_WORLD);
            
            if(rank == 0) {
            	int len=0;
            	printf("q=%ld\n",q);
		        int e;
		        for(e=0;e<q;e++) 
		        	if (output[e] != 0 && output[e] < 4096) {
		        		len++;
			        	printf("e=%d rank=%d\t,output = %d\n\n",e,rank,output[e]);
			        	}
			    
			    printf("len = %d\n",len);
			    
		        outputFile = fopen(strcat(argv[2], ".lzw"), "w+b");
			    for(e=0;e<len;e++)
			    	fputc(output[e],outputFile);
		        
		        
            }
            
            MPI_Finalize();
            
        }
        
        fclose(inputFile); fclose(outputFile);
    } else {
        printf("LZW 0.7   (c) 2009 Radek Stepan   03 Dec 2009\n\n");
        printf("Usage:    lzw <command> <input file>\n\n");
        printf("<Commands>\n  c       Compress\n  d       Decompress\n");
    }
    
    return 0;
}
