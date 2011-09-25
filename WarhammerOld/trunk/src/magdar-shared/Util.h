#ifndef _UTIL_H
#define _UTIL_H

#include "Common.h"
#include <string.h>
#include <vector>
#include <google/dense_hash_map>
#include <openssl/sha.h>
#include <openssl/evp.h>

///////////////////////////////////////////////////////////////////////////////
// String Functions ///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
std::vector<std::string> StrSplit(const std::string &src, const std::string &sep);

// This HAS to be called outside the threads __try / __except block!
void SetThreadName(const char* format, ...);
time_t convTimePeriod ( uint32 dLength, char dType);

const char         fillchar = '=';
                       // 00000000001111111111222222
                       // 01234567890123456789012345
static std::string cvt = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                       // 22223333333333444444444455
                       // 67890123456789012345678901
                         "abcdefghijklmnopqrstuvwxyz"
                       // 555555556666
                       // 234567890123
                         "0123456789+/";
                       
std::string Encode64(std::string data);
std::string Decode64(std::string data);

#ifdef WIN32

	typedef struct tagTHREADNAME_INFO
	{
		DWORD dwType; // must be 0x1000
		LPCSTR szName; // pointer to name (in user addr space)
		DWORD dwThreadID; // thread ID (-1=caller thread)
		DWORD dwFlags; // reserved for future use, must be zero
	} THREADNAME_INFO;

#endif

	namespace Utils
{
inline std::vector<std::string> StrSplit(const std::string &src, const std::string &sep)
{
    std::vector<std::string> r;
    std::string s;
    for (std::string::const_iterator i = src.begin(); i != src.end(); ++i)
    {
        if (sep.find(*i) != std::string::npos)
        {
            if (s.length())
                r.push_back(s);
            s = "";
        }
        else
        {
            s += *i;
        }
    }
    if (s.length())
        r.push_back(s);
    return r;
}

inline void hexdump(void *pAddressIn, long  lSize)
{
    std::ostringstream stm;
    stm << std::endl;
    char szBuf[100];
    long lIndent = 1;
    long lOutLen, lIndex, lIndex2, lOutLen2;
    long lRelPos;
    struct
    {
        char *pData;
        unsigned long lSize;
    } buf;
    unsigned char *pTmp,ucTmp;
    unsigned char *pAddress = (unsigned char *)pAddressIn;

    buf.pData   = (char *)pAddress;
    buf.lSize   = lSize;

    while (buf.lSize > 0)
    {
        pTmp     = (unsigned char *)buf.pData;
        lOutLen  = (int)buf.lSize;
        if (lOutLen > 16)
            lOutLen = 16;

        // create a 64-character formatted output line:
        sprintf(szBuf, " >                            "
                "                      "
                "    %08lX", (long unsigned int)(pTmp-pAddress));
        lOutLen2 = lOutLen;

        for (lIndex = 1+lIndent, lIndex2 = 53-15+lIndent, lRelPos = 0;
                lOutLen2;
                lOutLen2--, lIndex += 2, lIndex2++
            )
        {
            ucTmp = *pTmp++;

            sprintf(szBuf + lIndex,"%02X ", (unsigned short)ucTmp);
            if (!isprint(ucTmp))
                ucTmp = '.'; // nonprintable char
            szBuf[lIndex2] = ucTmp;

            if (!(++lRelPos & 3))     // extra blank after 4 bytes
            {
                lIndex++;
                szBuf[lIndex+2] = ' ';
            }
        }

        if (!(lRelPos & 3))
            lIndex--;

        szBuf[lIndex  ]   = '<';
        szBuf[lIndex+1]   = ' ';

        stm << szBuf << std::endl;


        buf.pData   += lOutLen;
        buf.lSize   -= lOutLen;
    }
	printf("%s",stm.str().c_str());
}
inline std::string strToLowerCase(std::string inString)
{
    std::transform(inString.begin(), inString.end(), inString.begin(),static_cast < int(*)(int) > (tolower));
    return inString;
}
inline std::string strToUpperCase(std::string inString)
{
    std::transform(inString.begin(), inString.end(), inString.begin(),static_cast < int(*)(int) > (toupper));
    return inString;
}

/// \todo Re-write this HORRID piece of code. Written in a hurry.
inline std::string byteArrayToAsciiString(const unsigned char *inArray, const int inLen)
{
    std::string outString;
    char tmp[255];
    for (int i=0; i< inLen; ++i)
    {
        sprintf(&tmp[0], "%02X", inArray[i]);
        outString.push_back(tmp[0]);
        outString.push_back(tmp[1]);
    }
    return outString;
}
inline void asciiHexStringtoByteArray(std::string inString, unsigned char* outArray)
{
    int length = inString.length();
    // make sure the input string has an even digit numbers
    if (length%2 == 1)
    {
        return;
    }

    int size = length/2;

    std::stringstream sstr(inString);
    for (int i=0; i < size; ++i)
    {
        char ch1, ch2;
        sstr >> ch1 >> ch2;
        int dig1, dig2;
        if (isdigit(ch1))
            dig1 = ch1 - '0';
        else
            if (ch1>='A' && ch1<='F')
                dig1 = ch1 - 'A' + 10;
            else
                if (ch1>='a' && ch1<='f')
                    dig1 = ch1 - 'a' + 10;
        if (isdigit(ch2))
            dig2 = ch2 - '0';
        else
            if (ch2>='A' && ch2<='F')
                dig2 = ch2 - 'A' + 10;
            else
                if (ch2>='a' && ch2<='f')
                    dig2 = ch2 - 'a' + 10;
				else	//this is a failure case, if we get here we've been given a malformed Hex string!
					dig2 = 0;
        outArray[i] = dig1*16 + dig2;
    }
}

struct strHash
{

    std::size_t operator()(const std::string& s) const
    {
        return SPARSEHASH_HASH<const char*>()(s.c_str());
    }
};

inline std::string makeSalt(std::string noun)
{
    uint8 salt[8];
    memset(salt,0,8);
    snprintf((char*)salt,8,"%s",noun.c_str());
    return byteArrayToAsciiString(salt,8);
}

inline std::string makeHash(const std::string& inA, const std::string& inB, const std::string& inHashC)
{
    char A[8], B[8];
    uint8 hashC[SHA256_DIGEST_LENGTH];
    char cryptIN[256];
    char outHash[SHA256_DIGEST_LENGTH];

    asciiHexStringtoByteArray(inA, (uint8*)&A);
    asciiHexStringtoByteArray(inB, (uint8*)&B);
    asciiHexStringtoByteArray(inHashC, (uint8*)&hashC);

    memset(cryptIN,0,256);
    memcpy(cryptIN,A,8);
    memcpy(&cryptIN[8],B,8);
    memcpy(&cryptIN[16],hashC,32);
    EVP_Digest(cryptIN,8+8+SHA256_DIGEST_LENGTH,(uint8*)&outHash,NULL,EVP_sha256(),NULL);

    return byteArrayToAsciiString((unsigned char*)&outHash,SHA256_DIGEST_LENGTH);
}

}//namespace Utils
#endif
