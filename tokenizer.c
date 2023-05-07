
typedef enum TokenType {
    Token_Unknown,
    Token_Identifier,
    Token_String,
    Token_True,
    Token_False,
    Token_Int,
    Token_Float,
    Token_Asterisk,
    Token_Equals,
    Token_Comma,
    Token_Colon,
    Token_Semicolon,
    Token_OpenParen,
    Token_CloseParen,
    Token_OpenBrace,
    Token_CloseBrace,
    Token_OpenBracket,
    Token_CloseBracket,
    Token_EndOfStream
} TokenType;

typedef struct Token {
    TokenType type;
    String text;
    I64 i64;
    F64 f64;
} Token;

typedef struct Tokenizer {
    String input;
    U8 at[2];
    B32 error;
} Tokenizer;

static B32 IsEndOfLine(U8 c) {
    B32 result = c == '\r' || c == '\n';
    return result;
}

static B32 IsWhitespace(U8 c) {
    B32 result = c == ' ' || c == '\t' || c == '\v' || c == '\f' || IsEndOfLine(c);
    return result;
}

static B32 IsNumber(U8 c) {
    B32 result = c >= '0' && c <= '9';
    return result;
}

static B32 IsAlpha(U8 c) {
    B32 result = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
    return result;
}

static void Tokenizer_Refill(Tokenizer* self) {
    
    if (self->input.count == 0) {
        self->at[0] = 0;
        self->at[1] = 0;
    } else if (self->input.count == 1) {
        self->at[0] = self->input.content[0];
        self->at[1] = 0;
    } else {
        self->at[0] = self->input.content[0];
        self->at[1] = self->input.content[1];
    }
}

static Tokenizer Tokenizer_Create(String input) {
    Tokenizer self = {0};
    self.input = input;
    Tokenizer_Refill(&self);
    return self;
}

static void Tokenizer_Advance(Tokenizer* self, I64 count) {
    self->input = String_Advance(self->input, count);
    Tokenizer_Refill(self);
}

static void Tokenizer_SkipWhitespace(Tokenizer* self) {
	for (;;) {
        if (IsWhitespace(self->at[0])) {
			Tokenizer_Advance(self, 1);
		} else if (self->at[0] == '/' && self->at[1] == '/') {
			Tokenizer_Advance(self, 2);
			while (self->at[0] && !IsEndOfLine(self->at[0])) {
				Tokenizer_Advance(self, 1);
			}
		} else if (self->at[0] == '/' && self->at[1] == '*') {
			Tokenizer_Advance(self, 2);
			while (!(self->at[0] == '*' && self->at[1] == '/')) {
				Tokenizer_Advance(self, 1);
                
			}
			if (self->at[0] == '*') {
				Tokenizer_Advance(self, 2);
			}
		} else {
			break;
		}
        
	}
}

Token Tokenizer_NextToken(Tokenizer* self) {
    
	Tokenizer_SkipWhitespace(self);
    
	Token token = {0};
	token.text = self->input;
    if (token.text.count) {
        token.text.count = 1;
    }
    
    U8 c = self->at[0];
    Tokenizer_Advance(self, 1);
    
    switch (c) {
        case '\0': { token.type = Token_EndOfStream; } break;
        case '(': { token.type = Token_OpenParen; } break;
        case ')': { token.type = Token_CloseParen; } break;
        case '[': { token.type = Token_OpenBracket; } break;
        case ']': { token.type = Token_CloseBracket; } break;
        case '{': { token.type = Token_OpenBrace; } break;
        case '}': { token.type = Token_CloseBrace; } break;
        case '*': { token.type = Token_Asterisk; } break;
        case '=': { token.type = Token_Equals; } break;
        case ',': { token.type = Token_Comma; } break;
        case ':': { token.type = Token_Colon; } break;
        case ';': { token.type = Token_Semicolon; } break;
        
        case '"': {
            token.type = Token_String;
            token.text = self->input;
            
            while (self->at[0] && self->at[0] != '"') {
                
                if (self->at[0] == '\\' && self->at[1]) {
                    Tokenizer_Advance(self, 1);
                }
                Tokenizer_Advance(self, 1);
            }
            
            token.text.count = (self->input.content - token.text.content);
            
            if (self->at[0] == '"') {
                Tokenizer_Advance(self, 1);
            }
            
        } break;
        
        
        default: {
            if (IsAlpha(c)|| c == '_') {
                token.type = Token_Identifier;
                
                while (IsAlpha(self->at[0]) || self->at[0] == '_' || IsNumber(self->at[0])) {
                    Tokenizer_Advance(self, 1);
                }
                
                token.text.count = (self->input.content - token.text.content);
                
            } else if (IsNumber(c)) {
                
            } else {
                token.type = Token_Unknown;
            }
            
        } break;
    }
    
	return token;
}

static B32 Tokenizer_Parsing(Tokenizer* self) {
    B32 result = !self->error;
    return result;
}

static void Tokenizer_Error(Tokenizer* self, Token token, String message) {
    self->error = true;
}


static B32 Token_Match(Token self, String other) {
	B32 result = String_Match(self.text, other);
	return result;
}

static Token Tokenizer_RequireToken(Tokenizer* self, TokenType requiredType) {
	Token token = Tokenizer_NextToken(self);
	
    if (token.type != requiredType) {
        Tokenizer_Error(self, token, Str("Unexpected token type"));
    }
    
	return token;
}

static B32 Tokenizer_OptionalToken(Tokenizer* self, TokenType type) {
	Token token = Tokenizer_NextToken(self);
	B32 result = token.type == type;
	return result;
}
