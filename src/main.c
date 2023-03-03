#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "aes128.h"

char options[2][3] = {"enc", "dec"};
char modes[2][3] = { "ecb", "ctr"};

void printError(char *name) {
	printf("Invalid usage. Try:\n\t%s [enc/dec] [mode:ecb/ctr] [input file] [output filen] [key file(text file with hex representation)] [optional:IV file for ctr mode (same format as key)]\n", name);
}

void parseArgs(int argc, char **argv, unsigned *option, unsigned *mode, char *inFilename, char *outFilename, char *keyFilename, char *ivFilename) {
	if(argc < 6 ||
		(strcmp(argv[1], "enc") != 0 && strcmp(argv[1], "dec") != 0) ||
		(strcmp(argv[2], "ecb") != 0 && strcmp(argv[2], "ctr") != 0)) {
		printError(argv[0]);
		exit(-1);
	}

	*option = 0;
	*mode = 0;

	if(strcmp(argv[1], "dec") == 0) {
		*option = 1;
	}

	if(strcmp(argv[2], "ctr") == 0) {
		*mode = 1;
		if(argc < 7) {
			printError(argv[0]);
			printf("You did not supply an IV.\n");
			exit(-1);
		}
	}
}

void textTo32Int(uint8_t *text, uint32_t arr[4]) {
	char buff[8];
	for(size_t i = 0; i < 4; ++i) {
		for(size_t j = 0; j < 8; ++j) {
			buff[j] = text[i * 8 + j];
		}
		arr[i] = (uint32_t) strtol(buff, NULL, 16);
	}
}

int main(int argc, char **argv) {

	unsigned mode;
	unsigned option;
	char *inFilename;
	char *outFilename;
	char *keyFilename;
	char *ivFilename;


	uint32_t key[4];/* = {0x2b7e1516, 0x28aed2a6, 0xabf71588, 0x09cf4f3c}; */
	uint32_t iv[4];

	FILE *fileptr;
	uint8_t* fileIn;
	uint8_t* fileOut;
	uint8_t* fileKey;
	uint8_t* fileIV;
	long fileInLen;
	long fileOutLen;
	long fileKeyLen;
	long fileIVLen;

	parseArgs(argc, argv, &option, &mode, inFilename, outFilename, keyFilename, ivFilename);
	inFilename = argv[3];
	outFilename = argv[4];
	keyFilename = argv[5];

	// read input file
	fileptr = fopen(inFilename, "rb");
	if(fileptr == NULL) {
		printf("Could not open file %s\n", inFilename);
		exit(-1);
	}
	fseek(fileptr, 0, SEEK_END);
	fileInLen = ftell(fileptr);
	rewind(fileptr);

	fileIn = (uint8_t *) malloc(fileInLen * sizeof(uint8_t));
	fread(fileIn, fileInLen, 1, fileptr);
	fclose(fileptr);


	
	// read key file
	fileptr = fopen(keyFilename, "rb");
	if(fileptr == NULL) {
		printf("Could not open file %s\n", keyFilename);
		exit(-1);
	}
	if(fileptr == NULL) {
		printf("Could not open file %s\n", keyFilename);
		exit(-1);
	}
	fseek(fileptr, 0, SEEK_END);
	fileKeyLen = ftell(fileptr);
	if(fileKeyLen < 32) {
		printf("Key too short.\n");
		return -1;
	}
	rewind(fileptr);

	fileKey = (uint8_t *) malloc(fileKeyLen * sizeof(uint8_t));
	fread(fileKey, fileKeyLen, 1, fileptr);
	fclose(fileptr);

	textTo32Int(fileKey, key);

	if(mode == 1) {
		ivFilename = argv[6];
		// read key file
		fileptr = fopen(ivFilename, "rb");
		if(fileptr == NULL) {
			printf("Could not open file %s\n", ivFilename);
			exit(-1);
		}
		if(fileptr == NULL) {
			printf("Could not open file %s\n", ivFilename);
			exit(-1);
		}
		fseek(fileptr, 0, SEEK_END);
		fileIVLen = ftell(fileptr);
		if(fileIVLen < 32) {
			printf("IV too short.\n");
			return -1;
		}
		rewind(fileptr);

		fileIV = (uint8_t *) malloc(fileIVLen * sizeof(uint8_t));
		fread(fileIV, fileIVLen, 1, fileptr);
		fclose(fileptr);

		textTo32Int(fileIV, iv);
		for(size_t i = 0; i < 4; ++i) {
			for(size_t j = 0; j < 2; ++j) {
				uint8_t temp = *((uint8_t*)(iv + i) + j);
				*((uint8_t*)(iv + i) + j) = *((uint8_t*)(iv + i) + 3 - j);
				*((uint8_t*)(iv + i) + 3 - j) = temp;
			}
		}
	}


	AES_set_key(key);
	if(mode == 0) {		// ECB
		if(option == 0) {	// ENC
			AES_EBC_enc(fileIn, fileInLen, &fileOut, &fileOutLen);
		} else {			// DEC
			AES_EBC_dec(fileIn, fileInLen, &fileOut, &fileOutLen);
		}
	} else {			// CTR
		if(option == 0) {	// ENC
			AES_CTR_enc(fileIn, fileInLen, &fileOut, &fileOutLen, (uint8_t*) iv);
		} else {			// DEC
			AES_CTR_dec(fileIn, fileInLen, &fileOut, &fileOutLen, (uint8_t*) iv);
		}
	}

	fileptr = fopen(outFilename, "wb");
	fwrite(fileOut, 1, fileOutLen, fileptr);
	fclose(fileptr);


	
	return 0;
}
