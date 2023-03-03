#include <stdint.h>
#include <stdlib.h>

/*
#include <stdio.h>


static void printState(uint8_t state[4][4]) {
	for(int i = 0; i < 4; ++i) {
		for(int j = 0; j < 4; ++j) {
			printf("%x ", state[i][j]);
		}
		printf("\n");
	}
} 
*/

static const uint8_t sbox[256] = {
  0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
  0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
  0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
  0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
  0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
  0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
  0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
  0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
  0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
  0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
  0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
  0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
  0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
  0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
  0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
  0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };

static const uint8_t invsbox[256] = {
  0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
  0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
  0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
  0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
  0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
  0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
  0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
  0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
  0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
  0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
  0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
  0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
  0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
  0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
  0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
  0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d };

static const uint8_t Rcon[11] = {
  0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36 };




/**
 * Kopirano s necijeg githuba jer nisam planirao sam izmisljati mnozenje u GF(2^8)
*/
static inline uint8_t xtime(uint8_t x) {
	return ((x<<1) ^ (((x>>7) & 1) * 0x1b));
}


static uint8_t Multiply(uint8_t a, uint8_t b) {
	int i;

	uint8_t c = 0;
	uint8_t d = b;

	for (uint8_t i = 0; i < 8 ; ++i) {
		if(a % 2 == 1) {
			c ^= d;
		}
		a /= 2;
		d = xtime(d);
	}
	return c;
}



static void ShiftRows(uint8_t state[4][4]) {
	uint8_t temp;

	// drugi row
	temp = state[1][0];
	state[1][0] = state[1][1];
	state[1][1] = state[1][2];
	state[1][2] = state[1][3];
	state[1][3] = temp;

	// treci row
	temp = state[2][0];
	state[2][0] = state[2][2];
	state[2][2] = temp;
	temp = state[2][1];
	state[2][1] = state[2][3];
	state[2][3] = temp;

	// cetvrti row
	temp = state[3][3];
	state[3][3] = state[3][2];
	state[3][2] = state[3][1];
	state[3][1] = state[3][0];
	state[3][0] = temp;
}

static void InvShiftRows(uint8_t state[4][4]) {
	uint8_t temp;

	// drugi row
	temp = state[1][3];
	state[1][3] = state[1][2];
	state[1][2] = state[1][1];
	state[1][1] = state[1][0];
	state[1][0] = temp;

	//treci row
	temp = state[2][0];
	state[2][0] = state[2][2];
	state[2][2] = temp;
	temp = state[2][1];
	state[2][1] = state[2][3];
	state[2][3] = temp;

	// cetvrti row
	temp = state[3][0];
	state[3][0] = state[3][1];
	state[3][1] = state[3][2];
	state[3][2] = state[3][3];
	state[3][3] = temp;
}

static void SubBytes(uint8_t state[4][4]) {
	for(uint8_t i = 0; i < 4; ++i) {
		for(uint8_t j = 0; j < 4; ++j) {
			state[i][j] = sbox[state[i][j]];
		}
	}
}

static void InvSubBytes(uint8_t state[4][4]) {
	for(uint8_t i = 0; i < 4; ++i) {
		for(uint8_t j = 0; j < 4; ++j) {
			state[i][j] = invsbox[state[i][j]];
		}
	}
}

static void MixColumns(uint8_t state[4][4]) {
	uint8_t newCol[4];
	for(uint8_t c = 0; c < 4; ++c) {
		newCol[0] = Multiply(0x02, state[0][c]) ^ Multiply(0x03, state[1][c]) ^ state[2][c] ^ state[3][c];
		newCol[1] = state[0][c] ^ Multiply(0x02, state[1][c]) ^ Multiply(0x03, state[2][c]) ^ state[3][c];
		newCol[2] = state[0][c] ^ state[1][c] ^ Multiply(0x02, state[2][c]) ^ Multiply(0x03, state[3][c]);
		newCol[3] = Multiply(0x03, state[0][c]) ^ state[1][c] ^ state[2][c] ^ Multiply(0x02, state[3][c]);
		
		// mozda brze od for petlje? (vjerojatno isto zbog optimizacije kompajlera)
		state[0][c] = newCol[0];
		state[1][c] = newCol[1];
		state[2][c] = newCol[2];
		state[3][c] = newCol[3];
		
	}
}

static void InvMixColumns(uint8_t state[4][4]) {
	uint8_t newCol[4];
	for(uint8_t c = 0; c < 4; ++c) {
		newCol[0] = Multiply(0x0e, state[0][c]) ^ Multiply(0x0b, state[1][c]) ^ Multiply(0x0d, state[2][c]) ^ Multiply(0x09, state[3][c]);
		newCol[1] = Multiply(0x09, state[0][c]) ^ Multiply(0x0e, state[1][c]) ^ Multiply(0x0b, state[2][c]) ^ Multiply(0x0d, state[3][c]);
		newCol[2] = Multiply(0x0d, state[0][c]) ^ Multiply(0x09, state[1][c]) ^ Multiply(0x0e, state[2][c]) ^ Multiply(0x0b, state[3][c]);
		newCol[3] = Multiply(0x0b, state[0][c]) ^ Multiply(0x0d, state[1][c]) ^ Multiply(0x09, state[2][c]) ^ Multiply(0x0e, state[3][c]);
				
		state[0][c] = newCol[0];
		state[1][c] = newCol[1];
		state[2][c] = newCol[2];
		state[3][c] = newCol[3];	
	}
}

static void AddRoundKey(uint8_t state[4][4], uint32_t *rk) {
	for(uint8_t i = 0; i < 4; ++i) {
		for(uint8_t j = 0; j < 4; ++j) {
			state[j][i] ^= ((uint8_t*)rk)[i * 4 + 3 - j];
		}
	}
}

static inline uint32_t SubWord(uint32_t word) {
	uint8_t ret[4];
	*((uint32_t*) ret) = word;
	ret[0] = sbox[ret[0]];
	ret[1] = sbox[ret[1]];
	ret[2] = sbox[ret[2]];
	ret[3] = sbox[ret[3]];

	return *((uint32_t*) ret);
}

static inline uint32_t RotWord(uint32_t word) {
	uint8_t temp;
	uint8_t ret[4];
	*((uint32_t*) ret) = word;
	temp = ret[3];
	ret[3] = ret[2];
	ret[2] = ret[1];
	ret[1] = ret[0];
	ret[0] = temp;

	return *((uint32_t*) ret);
}
static void KeyExpansion(uint32_t *key, uint32_t *word) {
	uint8_t i;

	for(i = 0; i < 4; ++i) {
		word[i] = key[i];
	}

	// 11 kljuceva za runde, 4 worda po rundi
	for(; i < 44; ++i) {
		uint32_t temp = word[i - 1];
		if(i % 4 == 0) {
			temp = SubWord(RotWord(temp));
			((uint8_t*) &temp)[3] ^= Rcon[i / 4];
		}
		word[i] = word[i - 4] ^ temp;
	}
}


static void Cipher(uint8_t *in, uint8_t *out, uint32_t *word) {
	uint8_t state[4][4];

	for(uint8_t i  = 0; i < 4; ++i) {
		for(uint8_t j = 0; j < 4; ++j) {
			state[j][i] = in[i * 4 + j]; 
		}
	}

	AddRoundKey(state, word);

	for(uint8_t i = 0; i < 9; ++i) {
		SubBytes(state);
		ShiftRows(state);
		MixColumns(state);
		AddRoundKey(state, word + (i + 1) * 4);
	}

	SubBytes(state);
	ShiftRows(state);
	AddRoundKey(state, word + 40);

	for(uint8_t i  = 0; i < 4; ++i) {
		for(uint8_t j = 0; j < 4; ++j) {
			out[i * 4 + j] = state[j][i]; 
		}
	}
}

static void InvCipher(uint8_t *in, uint8_t *out, uint32_t *word) {
	uint8_t state[4][4];

	for(uint8_t i  = 0; i < 4; ++i) {
		for(uint8_t j = 0; j < 4; ++j) {
			state[j][i] = in[i * 4 + j]; 
		}
	}

	AddRoundKey(state, word + 40);

	for(uint8_t i = 0; i < 9; ++i) {
		InvShiftRows(state);
		InvSubBytes(state);
		AddRoundKey(state, word + 36 - (i * 4));
		InvMixColumns(state);
	}

	InvShiftRows(state);
	InvSubBytes(state);
	AddRoundKey(state, word);

	for(uint8_t i  = 0; i < 4; ++i) {
		for(uint8_t j = 0; j < 4; ++j) {
			out[i * 4 + j] = state[j][i]; 
		}
	}
}

static uint32_t Key[4];

void AES_set_key(uint32_t *key) {
	for(uint8_t i = 0; i < 4; ++i) {
		Key[i] = key[i];
	}
}

void encrypterino(uint8_t *in, uint8_t *out) {
	uint32_t word[44];
	KeyExpansion(Key, word);
	Cipher(in, out, word);
}

void decrypterino(uint8_t *in, uint8_t *out) {
	uint32_t word[44];
	KeyExpansion(Key, word);
	InvCipher(in, out, word);
}

#include <stdio.h>

void incrementIV(uint8_t* IV) {
	for(size_t i = 16; i > 0; --i) {
		*(IV + i - 1) += 1;
		if(*(IV + i - 1) != 0) {
			break;
		}
	}
}

static void prepare(uint8_t *data, size_t len, uint8_t **out, size_t *outLen, uint8_t* extra, uint8_t paddedBlock[16]) {
	*extra = len % 16;
	uint8_t paddingLen = 16 - *extra;
	*outLen = len + paddingLen;
	*out = (uint8_t*) malloc((len + paddingLen) * sizeof(uint8_t));
	
	for(size_t i = len - *extra, j = 0; i < len; ++i, ++j) {
		paddedBlock[j] = data[i];
	}

	for(size_t i = *extra; i < 16; ++i) {
		paddedBlock[i] = paddingLen;
	}
}

void AES_EBC_enc(uint8_t *data, size_t len, uint8_t **out, size_t *outLen) {
	uint8_t extra;
	uint8_t paddedBlock[16];
	prepare(data, len, out, outLen, &extra, paddedBlock);
	
	uint32_t word[44];
	KeyExpansion(Key, word);
	for(size_t i = 0; i < len - extra; i += 16) {
		Cipher(data + i, *(out) + i, word);
	}

	Cipher(paddedBlock, *(out) + (len - extra), word);
}

void AES_EBC_dec(uint8_t *data, size_t len, uint8_t **out, size_t *outLen) {
	*out = (uint8_t*) malloc(len * sizeof(uint8_t));
	
	uint32_t word[44];
	KeyExpansion(Key, word);
	for(size_t i = 0; i < len; i +=16) {
		InvCipher(data + i, *(out) + i, word);
	}

	uint8_t padLen = *(*(out) + len - 1);
	*outLen = len - padLen;
}

void AES_CTR_enc(uint8_t *data, size_t len, uint8_t **out, size_t *outLen, uint8_t* IV) {
	*out = (uint8_t*) malloc((len) * sizeof(uint8_t));
	*outLen = len;

	for(size_t i = 0; i < len; ++i) {
		*((*out) + i) = *(data + i);
	}

	uint8_t IV_cipher[16];

	uint32_t word[44];
	size_t j = 0;
	KeyExpansion(Key, word);
	for(size_t i = 0; i < *outLen; i += 16) {
		Cipher(IV, IV_cipher, word);
		//for(size_t a = 0; a < 16; ++a) {printf("%x ", IV_cipher[a]);} printf("\n");
		for(; j < i + 16 && j < len; ++j) {
			*((*out) + j) ^= IV_cipher[j % 16];
		}
		incrementIV(IV); 
	}

}

void AES_CTR_dec(uint8_t *data, size_t len, uint8_t **out, size_t *outLen, uint8_t* IV) {
	AES_CTR_enc(data, len, out, outLen, IV);
}

