#if !defined(DATABASEENV_H)
#define DATABASEENV_H

//! Other libs we depend on.
#include "Common.h"
#include "Singleton.h"
#include "Log.h"

//! Our own includes.
#include "Database/Field.h"
#include "Database/Database.h"

enum
{
    FT_NA='x',                                              //not used or unknown, 4 byte size
    FT_NA_BYTE='X',                                         //not used or unknown, byte
    FT_STRING='s',                                          //char*
    FT_FLOAT='f',                                           //float
    FT_INT='i',                                             //uint32
    FT_BYTE='b',                                            //uint8
    FT_SORT='d',                                            //sorted by this field, field is not included
    FT_IND='n',                                             //the same,but parsed to data
    FT_LOGIC='l'                                            //Logical (boolean)
};

#endif
