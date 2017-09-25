//
//  SimScriptLexer.cpp
//  SimGL
//
//  Created by yangbin on 2017/7/14.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#include "SimScriptLexer.hpp"

ScriptLexer::ScriptLexer()
{
}

ScriptLexer::~ScriptLexer()
{
}

ScriptTokenListPtr ScriptLexer::tokenize(const String& scrpitName, String &data)
{
    // State enums
    enum { READY = 0, COMMENT, WORD };
    
    // Set up some constant characters of interest
    const wchar_t sharp = '#', openbrace = '{', closebrace = '}', cr = '\r', lf = '\n';
    char c = 0;
    char lastc = 0;  // Distinguish two lexemers
    
    // Result
    ScriptTokenListPtr tokens(new ScriptTokenList());
    
    String filename = scrpitName;
    String lexeme = "";
    int lineNo = 1, state = READY;
    
    String::const_iterator iter = data.begin();
    for (; iter!=data.end(); ++iter)
    {
        lastc = c;
        c = *iter;
        
        switch (state)
        {
            case READY:
                if (c == sharp)
                {   // It is comments, so clear the lexeme.
                    lexeme = "";
                    state = COMMENT;
                }
                else if (_isNewline(c))
                {   // The state should not be changed when it is new line.
                    lexeme = c;
                    _setToken(lexeme, lineNo, filename, tokens);
                }
                else if (!(_isWhitespace(c)))
                {  // It is word which need to change state to WORD.
                    lexeme = c;
                    state = WORD;
                }
                break;
            case COMMENT:
                if (_isNewline(c))
                {  // All the time there will be comments until new line, then change it to READY.
                    // The lexeme is alreay empty(""), so needn't to do same thing.
                    state = READY;
                }
                break;
            case WORD:
                if (_isNewline(c))
                {
                    _setToken(lexeme, lineNo, filename, tokens);   // Set token of last time
                    lexeme = c;
                    _setToken(lexeme, lineNo, filename, tokens);   // Set token of current time
                    state = READY;
                }
                else if (_isWhitespace(c))
                {
                    _setToken(lexeme, lineNo, filename, tokens);
                    state = READY;

                }
                else if (c == openbrace || c == closebrace)
                {
                    _setToken(lexeme, lineNo, filename, tokens);
                    lexeme = c;
                    _setToken(lexeme, lineNo, filename, tokens);
                    state = READY;
                }
                else if (c == sharp)
                {
                    _setToken(lexeme, lineNo, filename, tokens);
                    lexeme = "";
                    state = COMMENT;
                }
                else
                {
                    lexeme += c;
                }
                break;
                
            default:
                break;
        }
        
        // Separate check for newlines just to track line numbers
        if(c == cr || (c == lf && lastc != cr))
        {
            lineNo++;
        }
    }
    
    // Set token for the final time
    if (state == WORD && !lexeme.empty()) {
        _setToken(lexeme, lineNo, filename, tokens);
    }
    
    return tokens;
}

void ScriptLexer::_setToken(String lexeme, int lineNo, String& filename, ScriptTokenListPtr& tokens)
{
    const char openBracket = '{', closeBracket = '}';
    
    ScriptTokenPtr token(new ScriptToken());
    
    StringUtils::trim(lexeme);
    token->mLexeme = lexeme;
    token->mLineNo = lineNo;
    token->mFile = filename;
    token->mType = TID_WORD;
    
    bool ignore = false;
    
    if (lexeme.length() == 1)
    {
        if (_isNewline(lexeme.at(0)))
        {
            token->mType = TID_NEWLINE;
            if(tokens->empty() || tokens->back()->mType == TID_NEWLINE)
            {  // Delete newline more than one
                ignore = true;
            }
        }
        else if (lexeme.at(0) == openBracket)
        {
            token->mType = TID_LBRACKET;
        }
        else if (lexeme.at(0) == closeBracket)
        {
            token->mType = TID_RBRACKET;
        }
    }
    
    if (!ignore)
    {
        tokens->push_back(token);
    }
}

bool ScriptLexer::_isNewline(String::value_type val)
{
    return val == '\n' || val == '\r';
}

bool ScriptLexer::_isWhitespace(String::value_type val)
{
    return val == ' ' || val == '\t' || val == '\r';
}


