
typedef struct String {
    const U8* content;
    I64 count;
} String;

typedef struct Buffer {
    U8* content;
    I64 capacity;
} Buffer;

typedef struct StringBuilder {
    U8* content;
    I64 count;
    I64 capacity;
    Arena* arena;
    
} StringBuilder;

typedef struct StringFormat {
    StringBuilder builder;
    String format;
    String sub;
} StringFormat;


String String_Create(const U8* content, I64 count) {
    String self;
    self.content = content;
    self.count = count;
    return self;
}

Buffer Buffer_Create(U8* content, I64 capacity) {
    Buffer self;
    self.content = content;
    self.capacity = capacity;
    return self;
}

#define Str(string) String_Create((const U8*)(string), sizeof(string) - 1)
#define Buf(buffer) Buffer_Create((U8*)(buffer), sizeof(buffer))


#define StrC(string) { (const U8*)(string), sizeof(string) - 1 }

static B32 String_Valid(String string) {
    B32 result = string.content != 0 && string.count != 0;
    return result;
}

static B32 String_Match(String a, String b) {
    if (a.count != b.count) {
        return false;
    }
    
    for (I64 i = 0; i < a.count; ++i) {
        if (a.content[i] != b.content[i]) {
            return false;
        }
    }
    
    return true;
}

static String String_Advance(String string, I64 count) {
    
    String result = {0};
    
    if (string.count > count) {
        result.content = string.content + count;
        result.count = string.count - count;
    } else {
        result.content = string.content + string.count;
    }
    
    return result;
}

static String String_Substring(String string, I64 begin, I64 end) {
    String result;
    result.content = string.content + begin;
    result.count = end - begin;
    return result;
}


static String String_FindFirst(String haystack, String needle) {
    String result = {0};
    
    for (I64 i = 0; i < haystack.count; ++i) {
        if (haystack.content[i] == needle.content[0]) {
            String other = String_Substring(haystack, i, i + needle.count);
            if (String_Match(other, needle)) {
                result = String_Substring(haystack, i, haystack.count);
                break;
            }
        }
    }
    return result;
}

static String String_FindLast(String haystack, String needle) {
    String result = {0};
    
    for (I64 i = haystack.count - 1; i >=0; --i) {
        if (haystack.content[i] == needle.content[0]) {
            String other = String_Substring(haystack, i, i + needle.count);
            if (String_Match(other, needle)) {
                result = String_Substring(haystack, i, haystack.count);
                break;
            }
        }
    }
    return result;
}

static B32 String_Contains(String haystack, String needle) {
    String result = String_FindFirst(haystack, needle);
    return String_Valid(result);
}

static String String_RemovePrefix(String src, String prefix) {
    String result = src;
    
    if (prefix.count > src.count) {
        return result;
    }
    
    String other = String_Substring(src, 0, prefix.count);
    
    if (String_Match(prefix, other)) {
        result = String_Substring(src, prefix.count, src.count);
    }
    return result;
}

static String String_RemoveSuffix(String src, String suffix) {
    String result = src;
    
    if (suffix.count > src.count) {
        return result;
    }
    
    String other = String_Substring(src, src.count - suffix.count, src.count);
    
    if (String_Match(suffix, other)) {
        result = String_Substring(src, 0, src.count - suffix.count);
    }
    return result;
}


static I64 stringI64(String string) {
    I64 result = 0;
    
    if (String_Valid(string)) {
        I64 sign = 1;
        for (I64 i = 0; i < string.count; ++i)
        {
            if (string.content[i] >= '0' && string.content[i] <= '9')
            {
                result = result * 10 + (string.content[i] - '0');
            }
            else if (result == 0 && string.content[i] == '-')
            {
                sign = -1;
            }
            else
            {
                break;
            }
            
        }
        result *= sign;
    }
    return result;
}

static I64 stringCountI64(I64 value) {
    I64 count = 0;
    
    if (value == 0) {
        ++count;
    }
    
    for (I64 i = value; i != 0; i /= 10) {
        ++count;
    }
    return count;
}

static void stringBuilderReset(StringBuilder* builder) {
    builder->count = 0;
}

static StringBuilder StringBuilder_Create(Arena* arena) {
    StringBuilder result = {0};
    result.arena = arena;
    result.content = (U8*)arAlloc(arena, 128);
    result.capacity = 128;
    return result;
}

static void StringBuilder_Append(StringBuilder* builder, String string) {
    
    if ((builder->count + string.count) > builder->capacity) {
        I64 oldCapacity = builder->capacity;
        
        while ((builder->count + string.count) > builder->capacity) {
            builder->capacity *= 2;
        }
        
        builder->content = (U8*)arenaRealloc(builder->arena, builder->content, oldCapacity, builder->capacity);
    }
    
    void* destination = builder->content + builder->count;
    copy(destination, string.content, string.count);
    builder->count += string.count;
}

static void StringBuilder_AppendI64(StringBuilder* builder, I64 value) {
    
    I64 count = stringCountI64(value);
    
    if ((builder->count + count) > builder->capacity) {
        I64 oldCapacity = builder->capacity;
        builder->capacity *= 2;
        builder->content = (U8*)arenaRealloc(builder->arena, builder->content, oldCapacity, builder->capacity);
    }
    
    U8* destination = builder->content + builder->count;
    for (I64 i = 0; i < count; ++i) {
        I64 rem = value % 10;
        value = value / 10;
        destination[count - (i + 1)] = (Char)rem + '0';
    }
    
    builder->count += count;
}

static String StringBuilder_AsString(StringBuilder* builder) {
    String result = {0};
    result.content = builder->content;
    result.count = builder->count;
    return result;
}


static void stringFormatCopyToNext(StringFormat* format) {
    
    I64 index;
    for (index = 0; index < format->sub.count; ++index) {
        Char c = format->sub.content[index];
        if (c == '%') {
            break;
        }
    }
    
    I64 to = index;
    
    String sub = String_Substring(format->sub, 0, to);
    StringBuilder_Append(&format->builder, sub);
    format->sub = String_Substring(format->sub, to + 1, format->sub.count);
    
    
}

static void stringFormatReset(StringFormat* stringFormat, String format) {
    stringBuilderReset(&stringFormat->builder);
    stringFormat->format = format;
    stringFormat->sub = format;
    stringFormatCopyToNext(stringFormat);
}

static StringFormat stringFormatCreate(Arena* arena) {
    StringFormat result = {0};
    result.builder = StringBuilder_Create(arena);
    return result;
}

void stringFormatString(StringFormat* format, String value) {
    StringBuilder_Append(&format->builder, value);
    stringFormatCopyToNext(format);
}

void stringFormatI64(StringFormat* format, I64 value) {
    StringBuilder_AppendI64(&format->builder, value);
    stringFormatCopyToNext(format);
}

String stringFormatAsString(StringFormat* format) {
    String result = StringBuilder_AsString(&format->builder);
    return result;
}

String StringFormat_Print1s(StringFormat* self, String format, String value) {
    stringFormatReset(self, format);
    stringFormatString(self, value);
    String result = stringFormatAsString(self);
    return result;
}

String StringFormat_Print1i(StringFormat* self, String format, I64 value) {
    stringFormatReset(self, format);
    stringFormatI64(self, value);
    String result = stringFormatAsString(self);
    return result;
}

String StringFormat_Print2i(StringFormat* self, String format, I64 value1, I64 value2) {
    stringFormatReset(self, format);
    stringFormatI64(self, value1);
    stringFormatI64(self, value2);
    String result = stringFormatAsString(self);
    return result;
}



static I64 stringLength(CChar* string) {
    I64 i = 0;
    while (string[i]) {
        ++i;
    }
    return i;
}

static void StringCopy(Char* dst, CChar* src) {
    I64 i = 0;
    for (; src[i]; ++i) {
        dst[i] = src[i];
    }
    dst[i] = 0;
}

static B32 StringEquals(CChar* a, CChar* b) {
    B32 result = true;
    
    I64 i = 0;
    for (; a[i] && b[i]; ++i) {
        if (a[i] != b[i]) {
            result = false;
            break;
        }
    }
    
    if (a[i] != 0 || b[i] != 0) {
        result = false;
    }
    
    return result;
}


