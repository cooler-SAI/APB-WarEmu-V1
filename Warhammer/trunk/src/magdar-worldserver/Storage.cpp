#include "StdAfx.h"

const char RenownTablefmt[] = "ii";
const char XpTablefmt[]="iiii";
const char Itemfmt[] ="issiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiis";
const char RegionOffSetfmt[] = "iii";
const char TokInfofmt[] = "isiiiis";

const char CreatureProtofmt[] ="isiiiiiiiiiiisiiiiiiiiiiss";
const char CreatureSpawnfmt[] ="iiiiiiisiiiiiiiiiis";
const char GameObjectProtofmt[] ="isiiiiiiiiiiiiiii";
const char GameObjectSpawnfmt[] = "iiiiiiiiiiiiii";

SQLStorage sRenownTable(RenownTablefmt,"renownlevel","renowntable");
SQLStorage sXpTable(XpTablefmt,"xplevel","xptable");
SQLStorage sItemsTable(Itemfmt,"entry","items");
SQLStorage sTokTable(TokInfofmt,"tokid","tok_info");

SQLStorage sCreatureProto(CreatureProtofmt,"entry","creature_proto");
SQLStorage sCreatureSpawn(CreatureSpawnfmt,"guid","creature_spawns");
SQLStorage sGameObjectProto(GameObjectProtofmt,"entry","gameobject_proto");
SQLStorage sGameObjectSpawn(GameObjectSpawnfmt,"guid","gameobject_spawns");
SQLStorage sRegionOffset(RegionOffSetfmt,"region","region_offset");

void SQLStorage::Free ()
{
    uint32 offset=0;
    for(uint32 x=0;x<iNumFields;x++)
        if (dst_format[x]==FT_STRING)
        {
            for(uint32 y=0;y<MaxEntry;y++)
                if(pIndex[y])
                    delete [] *(char**)((char*)(pIndex[y])+offset);

            offset += sizeof(char*);
        }
        else if (dst_format[x]==FT_LOGIC)
            offset += sizeof(bool);
        else if (dst_format[x]==FT_BYTE)
            offset += sizeof(char);
        else
            offset += 4;

    delete [] pIndex;
    delete [] data;
}

void SQLStorage::Load()
{
    SQLStorageLoader loader;
    loader.Load(*this);
}
template<class T>
template<class S, class D>
void SQLStorageLoaderBase<T>::convert(uint32 /*field_pos*/, S src, D &dst)
{
    dst = D(src);
}

template<class T>
void SQLStorageLoaderBase<T>::convert_str_to_str(uint32 /*field_pos*/, char *src, char *&dst)
{
    if(!src)
    {
        dst = new char[1];
        *dst = 0;
    }
    else
    {
        uint32 l = strlen(src) + 1;
        dst = new char[l];
        memcpy(dst, src, l);
    }
}

template<class T>
template<class S>
void SQLStorageLoaderBase<T>::convert_to_str(uint32 /*field_pos*/, S /*src*/, char * & dst)
{
    dst = new char[1];
    *dst = 0;
}

template<class T>
template<class D>
void SQLStorageLoaderBase<T>::convert_from_str(uint32 /*field_pos*/, char * /*src*/, D& dst)
{
    dst = 0;
}

template<class T>
template<class V>
void SQLStorageLoaderBase<T>::storeValue(V value, SQLStorage &store, char *p, int x, uint32 &offset)
{
    T * subclass = (static_cast<T*>(this));
    switch(store.dst_format[x])
    {
        case FT_LOGIC:
            subclass->convert(x, value, *((bool*)(&p[offset])) );
            offset+=sizeof(bool);
            break;
        case FT_BYTE:
            subclass->convert(x, value, *((char*)(&p[offset])) );
            offset+=sizeof(char);
            break;
        case FT_INT:
            subclass->convert(x, value, *((uint32*)(&p[offset])) );
            offset+=sizeof(uint32);
            break;
        case FT_FLOAT:
            subclass->convert(x, value, *((float*)(&p[offset])) );
            offset+=sizeof(float);
            break;
        case FT_STRING:
            subclass->convert_to_str(x, value, *((char**)(&p[offset])) );
            offset+=sizeof(char*);
            break;
    }
}

template<class T>
void SQLStorageLoaderBase<T>::storeValue(char * value, SQLStorage &store, char *p, int x, uint32 &offset)
{
    T * subclass = (static_cast<T*>(this));
    switch(store.dst_format[x])
    {
        case FT_LOGIC:
            subclass->convert_from_str(x, value, *((bool*)(&p[offset])) );
            offset+=sizeof(bool);
            break;
        case FT_BYTE:
            subclass->convert_from_str(x, value, *((char*)(&p[offset])) );
            offset+=sizeof(char);
            break;
        case FT_INT:
            subclass->convert_from_str(x, value, *((uint32*)(&p[offset])) );
            offset+=sizeof(uint32);
            break;
        case FT_FLOAT:
            subclass->convert_from_str(x, value, *((float*)(&p[offset])) );
            offset+=sizeof(float);
            break;
        case FT_STRING:
            subclass->convert_str_to_str(x, value, *((char**)(&p[offset])) );
            offset+=sizeof(char*);
            break;
    }
}

template<class T>
void SQLStorageLoaderBase<T>::Load(SQLStorage &store)
{
    uint32 maxi;
    Field *fields;
    QueryResult *result  = WorldDatabase.Query("SELECT MAX(%s) FROM %s", store.entry_field, store.table);
    if(!result)
    {
        sLog.outError("Error loading %s table (not exist?)\n", store.table);
        exit(1);                                            // Stop server at loading non exited table or not accessable table
    }

	maxi = (*result).Fetch()[0].GetUInt32()+1;
    delete result;

    result = WorldDatabase.Query("SELECT COUNT(*) FROM %s", store.table);
    if(result)
    {
        fields = result->Fetch();
        store.RecordCount = fields[0].GetUInt32();
        delete result;
    }
    else
        store.RecordCount = 0;

    result = WorldDatabase.Query("SELECT * FROM %s", store.table);

    if(!result)
    {
        sLog.outError("%s table is empty!\n", store.table);
        store.RecordCount = 0;
        return;
    }

    uint32 recordsize = 0;
    uint32 offset = 0;

    if(store.iNumFields != result->GetFieldCount())
    {
        store.RecordCount = 0;
        sLog.outError("Error in %s table, probably sql file format was updated (there should be %d fields in sql).\n", store.table, store.iNumFields);
        delete result;
        exit(1);                                            // Stop server at loading broken or non-compatible table.
    }

    //get struct size
    uint32 sc=0;
    uint32 bo=0;
    uint32 bb=0;
    for(uint32 x=0; x< store.iNumFields; x++)
        if(store.dst_format[x]==FT_STRING)
            ++sc;
        else if (store.dst_format[x]==FT_LOGIC)
            ++bo;
        else if (store.dst_format[x]==FT_BYTE)
            ++bb;
    recordsize=(store.iNumFields-sc-bo-bb)*4+sc*sizeof(char*)+bo*sizeof(bool)+bb*sizeof(char);

    char** newIndex=new char*[maxi];
    memset(newIndex,0,maxi*sizeof(char*));

    char * _data= new char[store.RecordCount *recordsize];
    uint32 count=0;
    barGoLink bar( store.RecordCount );
    do
    {
        fields = result->Fetch();
		++bar;
        char *p=(char*)&_data[recordsize*count];
        newIndex[fields[0].GetUInt32()]=p;

        offset=0;
        for(uint32 x = 0; x < store.iNumFields; x++)
            switch(store.src_format[x])
            {
                case FT_LOGIC:
                    storeValue((bool)(fields[x].GetUInt32() > 0), store, p, x, offset); break;
                case FT_BYTE:
                    storeValue((char)fields[x].GetUInt8(), store, p, x, offset); break;
                case FT_INT:
                    storeValue((uint32)fields[x].GetUInt32(), store, p, x, offset); break;
                case FT_FLOAT:
                    storeValue((float)fields[x].GetFloat(), store, p, x, offset); break;
                case FT_STRING:
                    storeValue((char*)fields[x].GetString(), store, p, x, offset); break;
            }
        ++count;
    }while( result->NextRow() );

    delete result;

    store.pIndex = newIndex;
    store.MaxEntry = maxi;
    store.data = _data;
}