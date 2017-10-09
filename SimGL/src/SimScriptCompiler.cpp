//
//  SimScriptCompiler.cpp
//  SimGL
//
//  Created by yangbin on 2017/7/14.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#include "SimScriptCompiler.hpp"
#include "SimStringUtils.hpp"
#include "SimScriptLexer.hpp"
#include "SimScriptParser.hpp"
#include "SimScriptTranslator.hpp"

#include <fstream>
#include <stdexcept>

//================ AbstractNode ===================//

AbstractNode::AbstractNode(AbstractNode* ptr) :
        mLineNo(0),
        mType(ANT_UNKNOWN),
        mParent(ptr) {
}


//================ AtomAbstractNode ===================//

AtomAbstractNode::AtomAbstractNode(AbstractNode* ptr) :
        AbstractNode(ptr),
        mId(0) {
    mType = ANT_ATOM;
}

AbstractNode* AtomAbstractNode::clone() {
    AtomAbstractNode *node = new AtomAbstractNode(mParent);
    node->mFilename = mFilename;
    node->mLineNo = mLineNo;
    node->mId = mId;
    node->mType = mType;
    node->mValue = mValue;
    return node;
}

String AtomAbstractNode::getValue() {
    return mValue;
}

void AtomAbstractNode::_parseNumber() {
}

//================ ObjectAbstractNode ===================//

ObjectAbstractNode::ObjectAbstractNode(AbstractNode* ptr) :
        AbstractNode(ptr), mId(0), mAbstract(false) {
    mType = ANT_OBJECT;
}

AbstractNode *ObjectAbstractNode::clone() {
    ObjectAbstractNode *node = new ObjectAbstractNode(mParent);
    node->mFilename = mFilename;
    node->mLineNo = mLineNo;
    node->mType = mType;
    node->mName = mName;
    node->mCls = mCls;
    node->mId = mId;
    node->mAbstract = mAbstract;
    
    for(AbstractNodeList::const_iterator i = mChildren.begin(); i != mChildren.end(); ++i) {
        AbstractNodePtr newNode = AbstractNodePtr((*i)->clone());
        newNode->mParent = node;
        node->mChildren.push_back(newNode);
    }
    for(AbstractNodeList::const_iterator i = mValues.begin(); i != mValues.end(); ++i) {
        AbstractNodePtr newNode = AbstractNodePtr((*i)->clone());
        newNode->mParent = node;
        node->mValues.push_back(newNode);
    }
    node->mEnv = mEnv;
    return node;
}

String ObjectAbstractNode::getValue() {
    return mCls;
}

void ObjectAbstractNode::addVariable(const String &name) {
    mEnv.insert(std::make_pair(name, ""));
}

void ObjectAbstractNode::setVariable(const String &name, const String &value) {
    mEnv[name] = value;
}

std::pair<bool,String> ObjectAbstractNode::getVariable(const String &name) {
    OrderHashMap<String,String>::const_iterator i = mEnv.find(name);
    if(i != mEnv.end()) {
        return std::make_pair(true, i->second);
    }
    
    ObjectAbstractNode *parentNode = (ObjectAbstractNode*)this->mParent;
    while(parentNode)
    {
        i = parentNode->mEnv.find(name);
        if(i != parentNode->mEnv.end()) {
            return std::make_pair(true, i->second);
        }
        parentNode = (ObjectAbstractNode*)parentNode->mParent;
    }
    return std::make_pair(false, "");

}

const OrderHashMap<String,String> &ObjectAbstractNode::getVariables() {
    return mEnv;
}

//================ PropertyAbstractNode ===================//

PropertyAbstractNode::PropertyAbstractNode(AbstractNode *ptr) :
        AbstractNode(ptr), mId(0) {
    mType = ANT_PROPERTY;
}

AbstractNode *PropertyAbstractNode::clone() {
    PropertyAbstractNode *node = new PropertyAbstractNode(mParent);
    node->mFilename = mFilename;
    node->mLineNo = mLineNo;
    node->mType = mType;
    node->mName = mName;
    node->mId = mId;
    
    for(AbstractNodeList::const_iterator i = mValues.begin(); i != mValues.end(); ++i)  {
        AbstractNodePtr newNode = AbstractNodePtr((*i)->clone());
        newNode->mParent = node;
        node->mValues.push_back(newNode);
    }
    return node;
}

String PropertyAbstractNode::getValue() {
    return mName;
}

//================ Script compiler ===================//

ScriptCompiler::ScriptCompiler() {
    _initWordMap();
}

ScriptCompiler::~ScriptCompiler() {
    
}

bool ScriptCompiler::compile(const String& scrpitName, String &data) {
    ScriptLexer lexer;
    ScriptParser parser;
//    LogManager::getSingleton().debug(data);
//    ScriptTokenListPtr tokens = lexer.tokenize(scrpitName, data);
//    ScriptTokenList::iterator iter = tokens->begin();
//    for (; iter!= tokens->end(); ++iter) {
//        LogManager::getSingleton().debug((*iter)->mLexeme);
//    }
    ConcreteNodeListPtr nodes = parser.parse(lexer.tokenize(scrpitName, data));
    _compile(scrpitName, nodes);
    return true;
}

void ScriptCompiler::_initWordMap()
{
    // Material ids
    mIds["material"] = ID_MATERIAL;
    mIds["technique"] = ID_TECHNIQUE;
    mIds["pass"] = ID_PASS;
    mIds["vertex_shader"] = ID_VERTEX_SHADER;
    mIds["fragment_shader"] = ID_FRAGMENT_SHADER;
    mIds["geometry_shader"] = ID_GEOMETRY_SHADER;
    mIds["compute_shader"] = ID_COMPUTE_SHADER;
    mIds["texture_unit"] = ID_TEXTURE_UNIT;
    
    // Program ids
    mIds["source"] = ID_SOURCE;
    mIds["param_named"] = ID_PARAM_NAMED;
    mIds["light_pos_view"] = ID_LIGHT_POS_VIEW;
    mIds["ambient"] = ID_AMBIENT;
    
    // Texture ids
    mIds["texture"] = ID_TEXTURE;
    mIds["texture_alias"] = ID_TEXTURE_ALIAS;
    mIds["1d"] = ID_1D;
    mIds["2d"] = ID_2D;
    mIds["3d"] = ID_3D;
    mIds["cubic"] = ID_CUBIC;
    mIds["2d_array"] = ID_2D_ARRAY;
    mIds["sampler"] = ID_SAMPLER;
    mIds["filtering"] = ID_FILTERING;
    mIds["linear"] = ID_LINEAR;
}

bool ScriptCompiler::_compile(const String& scrpitName, ConcreteNodeListPtr& nodes)
{
    AbstractNodeListPtr asts = _convertToAST(nodes);
    for(AbstractNodeList::iterator i = asts->begin(); i != asts->end(); ++i)
    {
        if((*i)->mType == ANT_OBJECT && reinterpret_cast<ObjectAbstractNode*>((*i).get())->mAbstract)
            continue;
        //LogManager::getSingleton().logMessage(reinterpret_cast<ObjectAbstractNode*>((*i).get())->name);
        ScriptTranslator *translator = ScriptCompilerManager::getSingleton().getTranslator(*i);
        if(translator)
        {
            translator->translate(this, *i);
        }
    }
    return true;
}

AbstractNodeListPtr ScriptCompiler::_convertToAST(ConcreteNodeListPtr& nodes)
{
    AbstractTreeBuilder builder(this);
    AbstractTreeBuilder::visit(&builder, *nodes.get());
    return builder.getResult();
}

ScriptCompiler::AbstractTreeBuilder::AbstractTreeBuilder(ScriptCompiler* compiler) :
        mNodes(new AbstractNodeList()),
        mCompiler(compiler),
        mCurrent(nullptr)
{
}

const AbstractNodeListPtr &ScriptCompiler::AbstractTreeBuilder::getResult()
{
    return mNodes;
}

void ScriptCompiler::AbstractTreeBuilder::visit(ConcreteNode *node)
{
//    LogManager::getSingleton().debug(node->mToken + "--" + (node->mParent ? node->mParent->mToken : "root") );
//    for (ConcreteNodePtr& n : node->mChildren) {
//        visit(n.get());
//    }
//    LogManager::getSingleton().debug(node->mToken);
    
    AbstractNodePtr asn;
    if (!node->mChildren.empty())
    {
        // Grab the last two nodes
        ConcreteNodePtr temp1, temp2;
        ConcreteNodeList::reverse_iterator riter = node->mChildren.rbegin();
        if(riter != node->mChildren.rend())
        {
            temp1 = *riter;
            riter++;
        }
        if(riter != node->mChildren.rend())
        {
            temp2 = *riter;
        }

        // {}
        if (temp1->mType == CNT_RBRACE && temp2->mType == CNT_LBRACE)
        {
            if(node->mChildren.size() < 2)
            {
                LogManager::getSingleton().debug("AbstractTreeBuilder::visit");
                return;
            }

            ObjectAbstractNode *impl = new ObjectAbstractNode(mCurrent);
            impl->mLineNo = node->mLineNo;
            impl->mFilename = node->mFilename;
            impl->mAbstract = false;

            // Create a temporary detail list
            Vector<ConcreteNode*> temp;
            temp.push_back(node);
            for(ConcreteNodeList::const_iterator i = node->mChildren.begin(); i != node->mChildren.end(); ++i)
            {
                temp.push_back((*i).get());
            }

            // Get the type of object   [eg: material]
            Vector<ConcreteNode*>::const_iterator iter = temp.begin();
            impl->mCls = (*iter)->mToken;
            ++iter;

            // Get the name of object  [eg floorGLSL]
            if(iter != temp.end() && (*iter)->mType == CNT_WORD)
            {
                impl->mName = (*iter)->mToken;
                ++iter;
            }

            // Everything up until the colon is a "value" of this object
            while(iter != temp.end() && (*iter)->mType != CNT_LBRACE)
            {

                AtomAbstractNode *atom = new AtomAbstractNode(impl);
                atom->mFilename = (*iter)->mFilename;
                atom->mLineNo = (*iter)->mLineNo;
                atom->mType = ANT_ATOM;
                atom->mValue = (*iter)->mToken;
                impl->mValues.push_back(AbstractNodePtr(atom));

                ++iter;
            }

            // Finally try to map the cls to an id
            ScriptCompiler::IdMap::const_iterator iter2 = mCompiler->mIds.find(impl->mCls);
            if(iter2 != mCompiler->mIds.end())
            {
                impl->mId = iter2->second;
            }
            else
            {
                LogManager::getSingleton().debug("ScriptCompiler::_convertToAST", impl->mFilename + ", lineNo: " + std::to_string(impl->mLineNo) + ", token class: " + impl->mCls + ", unrecognized.");
            }

            asn = AbstractNodePtr(impl);
            mCurrent = impl;

            // Visit the children of the {
            AbstractTreeBuilder::visit(this, temp2->mChildren);

            // Go back up the stack
            mCurrent = impl->mParent;

        }
        else
        {
            // Otherwise, it is a property
            PropertyAbstractNode *impl = new PropertyAbstractNode(mCurrent);
            impl->mLineNo = node->mLineNo;
            impl->mFilename = node->mFilename;
            impl->mName = node->mToken;

            ScriptCompiler::IdMap::const_iterator iter2 = mCompiler->mIds.find(impl->mName);
            if(iter2 != mCompiler->mIds.end())
            {
                impl->mId = iter2->second;
            }

            asn = AbstractNodePtr(impl);
            mCurrent = impl;

            // Visit the children of the {
            AbstractTreeBuilder::visit(this, node->mChildren);

            // Go back up the stack
            mCurrent = impl->mParent;
        }

    }
    else
    {
        AtomAbstractNode *impl = new AtomAbstractNode(mCurrent);

        impl->mLineNo = node->mLineNo;
        impl->mFilename = node->mFilename;
        impl->mValue = node->mToken;

        ScriptCompiler::IdMap::const_iterator iter2 = mCompiler->mIds.find(impl->mValue);
        if(iter2 != mCompiler->mIds.end())
        {
            impl->mId = iter2->second;
        }

        asn = AbstractNodePtr(impl);
    }

    // Here, we must insert the node into the tree
    if(asn)
    {
        if(mCurrent)
        {
            if(mCurrent->mType == ANT_PROPERTY)
            {
                PropertyAbstractNode *impl = reinterpret_cast<PropertyAbstractNode*>(mCurrent);
                impl->mValues.push_back(asn);
            }
            else
            {
                ObjectAbstractNode *impl = reinterpret_cast<ObjectAbstractNode*>(mCurrent);
                impl->mChildren.push_back(asn);
            }
        }
        else
        {
            mNodes->push_back(asn);
        }
    }
}

void ScriptCompiler::AbstractTreeBuilder::visit(AbstractTreeBuilder *visitor, const ConcreteNodeList &nodes)
{
    for(ConcreteNodeList::const_iterator i = nodes.begin(); i != nodes.end(); ++i)
    {
        visitor->visit((*i).get());
    }
}

//================= Script manager ==================//

template<> ScriptCompilerManager *Singleton<ScriptCompilerManager>::mSingleton = nullptr;
ScriptCompilerManager::ScriptCompilerManager()
{
    mCompiler = new ScriptCompiler();
    mTranslatorManager = new ScriptTranslatorManager();
}

ScriptCompilerManager::~ScriptCompilerManager()
{
    if (mCompiler)
    {
        delete mCompiler;
        mCompiler = nullptr;
    }
    
    if (mTranslatorManager)
    {
        delete mTranslatorManager;
        mTranslatorManager = nullptr;
    }
}

ScriptTranslator *ScriptCompilerManager::getTranslator(const AbstractNodePtr &node)
{
    return mTranslatorManager->getTranslator(node);
}

bool ScriptCompilerManager::compileScript(const String& scriptName)
{
    // Load script.
    String result;
    if (!_loadScript(scriptName, result))
    {
        return false;
    }
    
    return mCompiler->compile(scriptName, result);
}

bool ScriptCompilerManager::_loadScript(const String& scriptName, String& out)
{
    const String& filepath = SCRIPT_RESOURCE_DIR + scriptName;

    //Open file
    std::ifstream inputStream;
    inputStream.open(filepath.c_str(), std::ios::in);
    if (!inputStream.is_open())
    {
        LogManager::getSingletonPtr()->debug("SimScriptCompilerManager::loadScript", "Failed to open file: " + filepath);
        return false;
    }
    
    std::stringstream ss;
    ss << inputStream.rdbuf();
    out = ss.str();
    return true;
}

