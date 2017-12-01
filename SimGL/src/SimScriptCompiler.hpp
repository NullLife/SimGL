//
//  SimScriptCompiler.hpp
//  SimGL
//
//  Created by yangbin on 2017/7/14.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#ifndef SimScriptCompilerManager_hpp
#define SimScriptCompilerManager_hpp

#include "SimCommon.hpp"
#include "SimSingleton.hpp"


//====================== ConcreteNode ======================//
struct ConcreteNode;
typedef SharedPtr<ConcreteNode> ConcreteNodePtr;
typedef Vector<ConcreteNodePtr> ConcreteNodeList;
typedef SharedPtr<ConcreteNodeList> ConcreteNodeListPtr;

enum ConcreteNodeType
{
    CNT_WORD = 1,
    CNT_LBRACE,
    CNT_RBRACE
};

struct ConcreteNode
{
    String mToken, mFilename;
    int mLineNo;
    ConcreteNodeType mType;
    ConcreteNode* mParent;
    ConcreteNodeList mChildren;
};

//====================== AbstractNode ======================//

enum AbstractNodeType
{
    ANT_UNKNOWN = 0,
    ANT_ATOM,
    ANT_OBJECT,
    ANT_PROPERTY
};
class AbstractNode;
typedef SharedPtr<AbstractNode> AbstractNodePtr;
typedef Vector<AbstractNodePtr> AbstractNodeList;
typedef SharedPtr<AbstractNodeList> AbstractNodeListPtr;

class AbstractNode
{
public:
    String mFilename;
    int mLineNo;
    AbstractNodeType mType;
    AbstractNode *mParent;
    
    void* mCxt;
public:
    AbstractNode(AbstractNode *ptr);
    virtual ~AbstractNode(){}
    /// Returns a new AbstractNode which is a replica of this one.
    virtual AbstractNode *clone() = 0;
    /// Returns a string value depending on the type of the AbstractNode.
    virtual String getValue() = 0;
};

/** This is an abstract node which cannot be broken down further */
class AtomAbstractNode : public AbstractNode
{
public:
    String mValue;
    int mId;
public:
    AtomAbstractNode(AbstractNode *ptr);
    AbstractNode *clone();
    String getValue();
private:
    void _parseNumber();
};

/** This specific abstract node represents a script object */
class ObjectAbstractNode : public AbstractNode
{
private:
    OrderHashMap<String,String> mEnv;
public:
    String mName, mCls;
    StringVector mBases;
    int mId;
    bool mAbstract;
    AbstractNodeList mChildren;
    AbstractNodeList mValues;
    AbstractNodeList mOverrides; // For use when processing object inheritance and overriding
public:
    ObjectAbstractNode(AbstractNode *ptr);
    AbstractNode *clone();
    String getValue();
    
    void addVariable(const String &name);
    void setVariable(const String &name, const String &value);
    std::pair<bool,String> getVariable(const String &name);
    const OrderHashMap<String,String> &getVariables();
};

/** This abstract node represents a script property */
class PropertyAbstractNode : public AbstractNode {
public:
    String mName;
    int mId;
    AbstractNodeList mValues;
public:
    PropertyAbstractNode(AbstractNode *ptr);
    AbstractNode *clone();
    String getValue();
};


//====================== ScriptCompiler ======================//

class ScriptCompiler {
public:
    typedef HashMap<String, int> IdMap;
public:
    ScriptCompiler();
    ~ScriptCompiler();
private:
    IdMap mIds;

public:
    bool compile(const String& scrpitName, String& data);
    
private:
    void _initWordMap();
    
    bool _compile(const String& scrpitName, ConcreteNodeListPtr& nodes);
    
    AbstractNodeListPtr _convertToAST(ConcreteNodeListPtr& nodes);
    
private: // Internal helper classes and processors
    class AbstractTreeBuilder {
    public:
        AbstractTreeBuilder(ScriptCompiler *compiler);
        const AbstractNodeListPtr &getResult();
        void visit(ConcreteNode *node);
        static void visit(AbstractTreeBuilder *visitor, const ConcreteNodeList &nodes);
        
    private:
        AbstractNodeListPtr mNodes;
        AbstractNode *mCurrent;
        ScriptCompiler *mCompiler;
    };
    friend class AbstractTreeBuilder;
};

//====================== SimScriptCompilerManager ======================//

class ScriptTranslator;
class ScriptTranslatorManager;

class ScriptCompilerManager : public Singleton<ScriptCompilerManager> {
public:
    ScriptCompilerManager();
    ~ScriptCompilerManager();

public:
    bool compileScript(const String& scriptName);
    
    ScriptTranslator *getTranslator(const AbstractNodePtr &node);
    
private:
    bool _loadScript(const String& scriptName, String& out);
    
private:
    ScriptCompiler* mCompiler;
    ScriptTranslatorManager* mTranslatorManager;
};

//======================= IDs ============================//

enum {
    // Material ids
    ID_MATERIAL = 10,
    ID_TECHNIQUE,
    ID_PASS,
    ID_VERTEX_SHADER,
    ID_FRAGMENT_SHADER,
    ID_GEOMETRY_SHADER,
    ID_COMPUTE_SHADER,
    ID_TEXTURE_UNIT,
    
    // Program ids
    ID_SOURCE = 80,
    ID_PARAM_NAMED,
    ID_FEEDBACK_OUT_VARYING,
    ID_FEEDBACK_OUT_MODE,
    ID_SUBROUTINE,
    ID_LIGHT_POS_VIEW,
    ID_AMBIENT,
    
    // Texture ids
    ID_TEXTURE = 201,
    ID_TEXTURE_ALIAS,
    ID_FILTERING,
    ID_LINEAR,
    ID_1D,
    ID_2D,
    ID_3D,
    ID_CUBIC,
    ID_2D_ARRAY,
    ID_SAMPLER
};


#endif /* SimScriptCompiler_hpp */
