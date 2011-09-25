#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <string>
#include <iostream>

#include "Bytebuffer.h"

class DBuffer : public ByteBuffer
{
 public:

        DBuffer()                                       : ByteBuffer(0)
        {
        }
        explicit DBuffer(size_t res=200) : ByteBuffer(res) { }

        DBuffer(const DBuffer &ibuff)              : ByteBuffer(ibuff)
        {
        }

        void Initialize(size_t newres=200)
        {
            clear();
            _storage.reserve(newres);
        }

		size_t GetRemain() { return size()-_rpos; };
};

struct SpellHeader // 33 bytes
{

};
struct SpellStart // 90 bytes
{

};
struct SpellData // 101 bytes
{

};
struct SpellEnder // 40 bytes
{

};

struct Header // 33
{
	uint8 unk1; //01

	uint32 unk2; // 13
	uint32 unk3; // 01
	uint32 unk4; // 09
	uint32 unk5; // 00
	uint32 unk6; // 00
	uint32 unk7; // 02
	uint32 unk8; // 0
	uint32 unk9; // 0

};

int main()
{
  FILE * pFile;
  long lSize;
  char * buffer;
  size_t result;

  pFile = fopen("abilityexport2.bin" , "rb");

  if(pFile==NULL) 
  {
	  fputs("Erreur de lecture",stderr); 
	  exit(1);
  }

  fseek (pFile , 0, SEEK_END);
  lSize = ftell(pFile);
  rewind (pFile);

  buffer = (char*)malloc(sizeof(char)*lSize);

  if(buffer == NULL)
  {
	  fputs("Impossible d'allouer la memoire",stderr);
	  exit (2);
  }

  result = fread(buffer, 1, lSize, pFile);

  if (result != lSize)
  {
	  fputs("Erreur de lecture",stderr);
	  exit(3);
  }

  DBuffer dat(99999);

  for(unsigned int i = 0; i < sizeof(char)*lSize; i++)
	dat << (uint8)buffer[i];

	FILE * fp = fopen("abilityexport2.txt","ab+");

	uint8 a,d;
	uint32 b;
	uint32 c;

	//dat >>  a >> b >> d >> c ;

	uint16 s;
	uint8 h,e;
	uint32 t;
  while(dat.GetRemain())
  {
	  dat >> h;

	  if(h == 0x68)
	  {
		  dat >> e;
		  if(e == 0x01)
		  {
			  fprintf(fp,"\n");
			  continue;
		  }
		  else {fprintf(fp,"%02X %02X ",h,e);continue;}
	  }

	fprintf(fp,"%02X ",h);
  }

  system("pause");
/*
  printf("Entrez le SPELLID\n");
  scanf("%d", &dspellID);

  printf("Entrez l'entree DATA que vous visez\n");
  scanf("%d", &dataVal);

  for(int i = 0; i <= dataVal; i++)
    dat >> Garbage;

  uint16 spellID = 0;

  FILE *fp = fopen("spells.txt","ab+");

  while(dat.size() > 1)
  {
	fprintf(fp,"SPELLID %d :",spellID);
	uint16  bDat;
	uint16  dDat;

	dat >> bDat;

	for(int i = 0; i <= 89; i++)
	{
		dat >> dDat;
		fprintf(fp,"%d | ",dDat);
	}

	if(tCompteur == dspellID)
	{
	  printf("SPELLID : %d DATA : %d VALUE : %d\n", spellID, dataVal, bDat);
	  system("pause");
	  fclose(pFile);
      free(buffer);
      return 0;
	}

	spellID++;
	tCompteur++;

	fprintf(fp,"\n");
  }
  */
  fclose(pFile);
  free(buffer);
  return 0;
}