#ifndef DORCRYPT_H
#define DORCRYPT_H


#include <Common.h>
#include <openssl/rand.h>
#include <openssl/rsa.h>
namespace DORCrypt
{

void encryptWARRC4Packet(unsigned char *inArray, int inLen, const unsigned char *RC4Key, const int RC4KeyLen);

void decryptWARRC4Packet(unsigned char *inArray, int inLen, const unsigned char *RC4Key, const int RC4KeyLen);

void generateRSAKey();

void decryptWARRSAPacket(const unsigned char *inArray, int inLen, unsigned char *outArray, int *outLen);

void exportRSAPublicKey(char *outKey, int *outLen);

void cleanUp();

extern RSA *rsaKey;

extern bool bInitialised;
}

#endif //DORCRYPT_H
