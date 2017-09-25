//
//  SimScriptLexer.hpp
//  SimGL
//
//  Created by yangbin on 2017/7/14.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#ifndef SimScriptLexer_hpp
#define SimScriptLexer_hpp

#include "SimCommon.hpp"

enum {
    TID_LBRACKET = 0, // {
    TID_RBRACKET,     // }
    TID_WORD,         // *
    TID_NEWLINE,      // \n
    TID_UNKNOWN,
    TID_END
};

struct ScriptToken
{
    /// This is the lexeme for this token
    String mLexeme;
    String mFile;
    /// This is the id associated with the lexeme, which comes from a lexeme-token id mapping
    int mType;
    /// This holds the line number of the input stream where the token was found.
    int mLineNo;
};

typedef SharedPtr<ScriptToken> ScriptTokenPtr;
typedef Vector<ScriptTokenPtr> ScriptTokenList;
typedef SharedPtr<ScriptTokenList> ScriptTokenListPtr;


class ScriptLexer {
public:
    ScriptLexer();
    ~ScriptLexer();
    
public:
    ScriptTokenListPtr tokenize(const String& scrpitName, String& data);
    
private:
    void _setToken(String lexeme, int lineNo, String& filename, ScriptTokenListPtr& tokens);
    
    bool _isNewline(String::value_type val);
    
    bool _isWhitespace(String::value_type val);
};

#endif /* SimScriptLexer_hpp */
