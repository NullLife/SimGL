//
//  SimScriptParser.cpp
//  SimGL
//
//  Created by yangbin on 2017/7/14.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#include "SimScriptParser.hpp"

ScriptParser::ScriptParser() {
    
}


ScriptParser::~ScriptParser() {
    
}

ConcreteNodeListPtr ScriptParser::parse(const ScriptTokenListPtr& tokens)
{
    ConcreteNodeListPtr nodes(new ConcreteNodeList());
    
    enum { READY, OBJECT };
    
    int state = READY;
    
    ConcreteNode *parent = nullptr;
    ConcreteNodePtr node;
    ScriptToken *token = nullptr;
    
    ScriptTokenList::iterator iter = tokens->begin();
    ScriptTokenList::iterator endIter = tokens->end();
    for (; iter!=endIter; ++iter)
    {
        token = (*iter).get();
        switch (state)
        {
            case READY:
                if (token->mType == TID_WORD)
                {
                    node = ConcreteNodePtr(new ConcreteNode());
                    node->mToken = token->mLexeme;
                    node->mFilename = token->mFile;
                    node->mLineNo = token->mLineNo;
                    node->mType = CNT_WORD;
                    
                    // Set parent node
                    if (parent)
                    {
                        node->mParent = parent;
                        parent->mChildren.push_back(node);
                    }
                    else
                    {
                        node->mParent = nullptr;
                        nodes->push_back(node);
                    }
                    
                    // Switch parent node to current node
                    parent = node.get();
                    
                    // Switch state
                    state = OBJECT;
                    
                    // Restore node
                    node = ConcreteNodePtr();
                }
                else if (token->mType == TID_RBRACKET)
                {
                    // Consider return up level when is see "}"
                    if (parent)
                    {
                        parent = parent->mParent;
                    }
                    
                    node = ConcreteNodePtr(new ConcreteNode());
                    node->mToken = token->mLexeme;
                    node->mFilename = token->mFile;
                    node->mLineNo = token->mLineNo;
                    node->mType = CNT_RBRACE;

                    if (parent)
                    {
                        node->mParent = parent;
                        parent->mChildren.push_back(node);
                    }
                    else
                    {
                        node->mParent = nullptr;
                        nodes->push_back(node);
                    }
                    
                    
                    // Move up another level
                    if(parent)
                    {
                        parent = parent->mParent;
                    }
                    
                    node = ConcreteNodePtr();
                }
                else if (token->mType == TID_NEWLINE)
                {
                    // Nothing
                }
                
                break;
                
            case OBJECT:
                if(token->mType == TID_NEWLINE)
                {
                    // Look ahead to the next non-newline token and if it isn't an {, this was a property
                    ScriptTokenList::iterator next = _skipNewlines(iter, endIter);

                    if(next == endIter || (*next)->mType != TID_LBRACKET)
                    {
                        // Ended a property here
                        if(parent)
                        {
                            parent = parent->mParent;
                        }
                        
                        state = READY;
                    }
                }
                else if (token->mType == TID_WORD)
                {
                    node = ConcreteNodePtr(new ConcreteNode());
                    node->mToken = token->mLexeme;
                    node->mFilename = token->mFile;
                    node->mLineNo = token->mLineNo;
                    node->mType = CNT_WORD;
                    
                    // Insert the node
                    if(parent)
                    {
                        node->mParent = parent;
                        parent->mChildren.push_back(node);
                    }
                    else
                    {
                        node->mParent = nullptr;
                        nodes->push_back(node);
                    }
                    
                    node = ConcreteNodePtr();
                }
                else if (token->mType == TID_LBRACKET)
                {
                    node = ConcreteNodePtr(new ConcreteNode());
                    node->mToken = token->mLexeme;
                    node->mFilename = token->mFile;
                    node->mLineNo = token->mLineNo;
                    node->mType = CNT_LBRACE;
                    
                    if (parent)
                    {
                        node->mParent = parent;
                        parent->mChildren.push_back(node);
                    }
                    else
                    {
                        node->mParent = nullptr;
                        nodes->push_back(node);
                    }
                    
                    parent = node.get();
                    
                    state = READY;
                    
                    node = ConcreteNodePtr();
                }
                else if (token->mType == TID_RBRACKET)
                {
                    // Consider return up level when is see "}"
                    if (parent)
                    {
                        parent = parent->mParent;
                    }
                    
                    // If the parent is currently a { then go up again
                    if(parent && parent->mType == CNT_LBRACE && parent->mParent)
                    {
                        parent = parent->mParent;
                    }

                    node = ConcreteNodePtr(new ConcreteNode());
                    node->mToken = token->mLexeme;
                    node->mFilename = token->mFile;
                    node->mLineNo = token->mLineNo;
                    node->mType = CNT_RBRACE;
                    
                    if (parent)
                    {
                        node->mParent = parent;
                        parent->mChildren.push_back(node);
                    }
                    else
                    {
                        node->mParent = nullptr;
                        nodes->push_back(node);
                    }
                    
                    if(parent)
                    {
                        parent = parent->mParent;
                    }

                    node = ConcreteNodePtr();
                    
                    state = READY;
                }
                break;
        }
    }
    
    return nodes;
}

ScriptTokenList::iterator ScriptParser::_skipNewlines(ScriptTokenList::iterator currPos, ScriptTokenList::iterator end)
{
    while (currPos != end && (*currPos)->mType == TID_NEWLINE)
    {
        ++currPos;
    }
    return currPos;
}
