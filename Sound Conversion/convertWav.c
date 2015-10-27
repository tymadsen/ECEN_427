#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]){
	if(argc != 3){
		printf("Invalid input, got: %d\n expected: 'convertWav <fileName> <newFileName>\n", argc);
		return 0;
	}

	FILE *ifp, *ofp;
	int size = strlen(argv[2])+2;
	char ofpName[size];
	int i;
	for(i=0;i<size;i++){
		if(i < (size-2))
			ofpName[i] = argv[2][i];
		else
			if(i == size-2)
				ofpName[i] = '.';
			else
				ofpName[i] = 'c';
	}
	printf("output file name: %s\n", ofpName);
	//Get file pointer, reading file
	ifp = fopen(argv[1], "r");
	ofp = fopen(ofpName, "w");

	int numSamples = 0;
	int sampleRate = 0;
	int data;
	short dat;
	char chars[2], chunkDesc[13], fmtSubChunk[13], sampleInfo[13], dataSubchunk[9];
	chunkDesc[12] = 0x20;
	fmtSubChunk[12] = 0x20;
	sampleInfo[12] = 0x20;
	dataSubchunk[8] = 0x20;

	fscanf(ifp, "%c%c%c%c%c%c%c%c%c%c%c%c", chunkDesc, chunkDesc+1, chunkDesc+2, chunkDesc+3, chunkDesc+4, chunkDesc+5, chunkDesc+6, chunkDesc+7, chunkDesc+8, chunkDesc+9, chunkDesc+10, chunkDesc+11);
	fscanf(ifp, "%c%c%c%c%c%c%c%c%c%c%c%c", fmtSubChunk, fmtSubChunk+1, fmtSubChunk+2, fmtSubChunk+3, fmtSubChunk+4, fmtSubChunk+5, fmtSubChunk+6, fmtSubChunk+7, fmtSubChunk+8, fmtSubChunk+9, fmtSubChunk+10, fmtSubChunk+11);
	fscanf(ifp, "%c%c%c%c%c%c%c%c%c%c%c%c", sampleInfo, sampleInfo+1, sampleInfo+2, sampleInfo+3, sampleInfo+4, sampleInfo+5, sampleInfo+6, sampleInfo+7, sampleInfo+8, sampleInfo+9, sampleInfo+10, sampleInfo+11);
	fscanf(ifp, "%c%c%c%c%c%c%c%c", dataSubchunk, dataSubchunk+1, dataSubchunk+2, dataSubchunk+3, dataSubchunk+4, dataSubchunk+5, dataSubchunk+6, dataSubchunk+7);

	printf("chunk %s\n", chunkDesc);
	printf("fmt %s\n", fmtSubChunk);
	printf("sample %s\n", sampleInfo);
	printf("data %s\n", dataSubchunk);


	//File is valid
	if(ifp){
		// Write to file
		// Start writing sound data array
		fprintf(ofp, "int %s_soundData[] = {", argv[2]);

		// Jump to data section
		// Write data
		while(fscanf(ifp, "%c%c", chars, chars+1) != EOF) {
			printf("read: %hi%hi\n", (short)chars[0], (short)chars[1]);
		  fprintf(ofp, "%hi%hi, ", (short)chars[0], (short)chars[1]);
		  numSamples++;
		}
		// Finish sound data array
		fprintf(ofp, "};\n");

		// Write num samples
		fprintf(ofp, "int %s_numberOfSamples = %d;\n", argv[2], numSamples);
		
		// Write sample rate
		fprintf(ofp, "int %s_sampleRate = %d;\n", argv[2], sampleRate);

		// Close files
		fclose(ifp);
		fclose(ofp);
	}else{//Alert and exit
		printf("File is invalid. please try a different file.\n");
		return 0;
	}

return 0;

}