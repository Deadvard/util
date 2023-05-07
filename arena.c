
static void copy(void* destination, const void* source, I64 length);


typedef struct Arena {
    U8* content;
    I64 count;
    I64 capacity;
} Arena;

static B32 IsPowerOfTwo(I64 x) {
	return (x & (x - 1)) == 0;
}

static I64 AlignForward(I64 ptr, I64 align) {
    assert(IsPowerOfTwo(align));
    
    I64 p = ptr;
    I64 al = align;
    I64 modulo = p & (al - 1);
    
    if (modulo != 0) {
        p += (al - modulo);
    }
    
    return p;
}


static Arena Arena_Create(U8* memory, I64 size) {
    Arena arena = {0};
    arena.content = memory;
    arena.capacity = size;
    return arena;
}

static U8* arAllocAlign(Arena* arena, I64 size, I64 align) {
    I64 currPtr = (I64)(arena->content + arena->count);
    I64 offset = AlignForward(currPtr, align);
    offset -= (I64)arena->content;
    
    U8* result = 0;
    if ((offset + size) <= arena->capacity) {
		result = arena->content + offset;
        arena->count = (offset + size);
        
        // clear?
	}
    
    return result;
}

static void* arAlloc(Arena* arena, I64 size) {
    return arAllocAlign(arena, size, 2 * sizeof(void*));
}

static void* arenaRealloc(Arena* arena, void* ptr, I64 oldSize, I64 newSize) {
    void* newPtr = arAlloc(arena, newSize);
    copy(newPtr, ptr, oldSize);
    return newPtr;
}

static void arFreeAll(Arena* arena) {
    arena->count = 0;
}
