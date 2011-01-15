#include "Crypt.h"
#include <openssl/bn.h>
#include <Util.h>

RSA *DORCrypt::rsaKey;
bool DORCrypt::bInitialised = false;
#define STORE32L(x, y)                                                                     \
     { (y)[3] = (unsigned char)(((x)>>24)&255); (y)[2] = (unsigned char)(((x)>>16)&255);   \
       (y)[1] = (unsigned char)(((x)>>8)&255); (y)[0] = (unsigned char)((x)&255); }
#define OUTPUT_BIGNUM(num, buf2, y, z)         \
{                                              \
      z = (unsigned long)BN_num_bytes(num);  \
      STORE32L(z, buf2+y);                     \
      y += 4;                                  \
      (void)BN_bn2bin(num, buf2+y);   \
      y += z;                                  \
}

void DORCrypt::decryptWARRC4Packet(unsigned char *inArray, int inLen, const unsigned char *RC4Key, const int RC4KeyLen)
{
    int x, y, midpoint, pos;
    unsigned char *s, tmp, *tmp_sym_sbox;
    tmp_sym_sbox = (unsigned char*)malloc(RC4KeyLen * sizeof(unsigned char));
	if(tmp_sym_sbox == NULL)
	{
		printf("decryptWARRC4Packet - malloc failed to allocate tmp_sym_sbox!");
	}
	else
	{
		memcpy(tmp_sym_sbox,RC4Key,RC4KeyLen);
		x = 0;
		y = 0;
		s = tmp_sym_sbox;
		/* it is not standard RC4 practice to break a block in half, but packets
		 from mythic's client have a sequence number at the beginning which
		 would be easily guessable */
		midpoint = inLen / 2;

		for (pos=midpoint; pos<inLen; pos++)
		{
			x = (x + 1) & 255;
			y = (y + s[x]) & 255;
			tmp = s[x];
			s[x] = s[y];
			s[y] = tmp;
			tmp = (s[x] + s[y]) & 255;
			inArray[pos] ^= s[tmp];
			y = (y + inArray[pos]) & 255;  // this is not standard RC4 here
		}
		for (pos=0; pos<midpoint; pos++)
		{
			x = (x + 1) & 255;
			y = (y + s[x]) & 255;
			tmp = s[x];
			s[x] = s[y];
			s[y] = tmp;
			tmp = (s[x] + s[y]) & 255;
			inArray[pos] ^= s[tmp];
			y = (y + inArray[pos]) & 255;  // this is not standard RC4 here
		}
	}
	
	free(tmp_sym_sbox);
}
void DORCrypt::encryptWARRC4Packet(unsigned char *inArray, int inLen, const unsigned char *RC4Key, const int RC4KeyLen)
{
    int x, y, midpoint, pos;
    unsigned char *s, tmp, *tmp_sym_sbox;
    tmp_sym_sbox = (unsigned char*)malloc(RC4KeyLen * sizeof(unsigned char));
	if(tmp_sym_sbox == NULL)
	{
		printf("encryptWARRC4Packet - malloc failed to allocate tmp_sym_sbox!");
		/// \todo Kill server here... something is fundamentally wrong!
	}
	else
	{
		memcpy(tmp_sym_sbox,RC4Key,RC4KeyLen);

		x = 0;
		y = 0;
		s = tmp_sym_sbox;
		/* it is not standard RC4 practice to break a block in half, but packets
		 from mythic's client have a sequence number at the beginning which
		 would be easily guessable */
		midpoint = inLen / 2;

		for (pos=midpoint; pos<inLen; pos++)
		{
			x = (x + 1) & 255;
			y = (y + s[x]) & 255;
			tmp = s[x];
			s[x] = s[y];
			s[y] = tmp;
			tmp = (s[x] + s[y]) & 255;
			y = (y + inArray[pos]) & 255;  // this is not standard RC4 here
			inArray[pos] ^= s[tmp];
		}
		for (pos=0; pos<midpoint; pos++)
		{
			x = (x + 1) & 255;
			y = (y + s[x]) & 255;
			tmp = s[x];
			s[x] = s[y];
			s[y] = tmp;
			tmp = (s[x] + s[y]) & 255;
			y = (y + inArray[pos]) & 255;  // this is not standard RC4 here
			inArray[pos] ^= s[tmp];
		}
	}
	free(tmp_sym_sbox);
}

void DORCrypt::generateRSAKey()
{
    const char rnd_seed[] = "DoR Rocks! dOr rOCKS¡";
    RAND_seed(rnd_seed, sizeof rnd_seed);

    rsaKey = RSA_generate_key(1536, 65537, NULL, NULL);
    bInitialised = true;
}
void DORCrypt::exportRSAPublicKey(char *outKey, int *outLen)
{
    unsigned char buf2[5120];
    unsigned long y, z;
    STORE32L(0x91, buf2);
    y=4;
    buf2[y++] = 0x01;
    OUTPUT_BIGNUM(rsaKey->n, buf2, y, z);
    OUTPUT_BIGNUM(rsaKey->e, buf2, y, z);

    memcpy(outKey,buf2,y);
    *outLen = y;
}
void DORCrypt::decryptWARRSAPacket(const unsigned char *inArray, int inLen, unsigned char *outArray, int *outLen)
{
    *outLen = RSA_private_decrypt(inLen,inArray, outArray,rsaKey,RSA_NO_PADDING);
}

void DORCrypt::cleanUp()
{
    RSA_free(rsaKey);
}
