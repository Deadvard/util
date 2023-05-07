
static U32 hashFunction(const char* string)
{
    // NOTE(Edvard): djb2
    U32 hash = 5381;
    for (U32 i = 0; string[i]; ++i)
    {
        hash = ((hash << 5) + hash) + string[i];
    }
    
    return hash;
}

static U32 hashFunctionLen(const U8* string, I64 length)
{
    // NOTE(Edvard): djb2
    U32 hash = 5381;
    for (I64 i = 0; i < length; ++i)
    {
        hash = ((hash << 5) + hash) + string[i];
    }
    
    return hash;
}

typedef struct HashTable {
    I64 size;
    U32* keys;
    U32* values;
} HashTable;

HashTable createHashTable(I64 size)
{
    HashTable result = {0};
    result.size = size;
    result.keys = (U32*)MemAlloc(sizeof(U32) * result.size);
    result.values = (U32*)MemAlloc(sizeof(U32) * result.size);
    return result;
}



void hashSet(HashTable* table, U32 key, U32 value)
{
    U32 n = (U32)table->size;
    U32 i = key % n;
    
    while (table->keys[i] != key && table->keys[i] != 0)
    {
        i = (i + 1) % n;
    }
    
    assert(table->keys[i] != key || table->keys[i] == 0); // NOTE(Edvard): checking collision
    
    table->keys[i] = key;
    table->values[i] = value;
}

U32 hashGet(const HashTable* table, U32 key)
{
    U32 n = (U32)table->size;
    U32 i = key % n;
    
    while (table->keys[i] != key && table->keys[i] != 0)
    {
        i = (i + 1) % n;
    }
    
    return table->keys[i] != 0 ? table->values[i] : 0;
}