//
//  SimScriptParser.hpp
//  SimGL
//
//  Created by yangbin on 2017/7/14.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#ifndef SimScriptParser_hpp
#define SimScriptParser_hpp

#include "SimScriptCompiler.hpp"
#include "SimScriptLexer.hpp"

class ScriptParser {
public:
    ScriptParser();
    ~ScriptParser();
    
public:
    ConcreteNodeListPtr parse(const ScriptTokenListPtr& tokens);
    
private:
    ScriptTokenList::iterator _skipNewlines(ScriptTokenList::iterator currPos, ScriptTokenList::iterator end);
};

#endif /* SimScriptParser_hpp */
