#include <stdint.h>


void encrypterino(uint8_t *in, uint8_t *out);
void decrypterino(uint8_t *in, uint8_t *out);

void AES_set_key(uint32_t *key);
void AES_EBC_enc(uint8_t *data, size_t len, uint8_t **out, size_t *outLen);
void AES_EBC_dec(uint8_t *data, size_t len, uint8_t **out, size_t *outLen);
void AES_CTR_enc(uint8_t *data, size_t len, uint8_t **out, size_t *outLen, uint8_t* IV);
void AES_CTR_dec(uint8_t *data, size_t len, uint8_t **out, size_t *outLen, uint8_t* IV);

