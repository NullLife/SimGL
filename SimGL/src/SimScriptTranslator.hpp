//
//  SimScriptTranslator.hpp
//  SimGL
//
//  Created by yangbin on 2017/7/17.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#ifndef SimScriptTranslator_hpp
#define SimScriptTranslator_hpp

#include "SimScriptCompiler.hpp"
#include "SimGLShaderParams.hpp"

class Technique;
class Pass;
class TextureUnitState;


//==================== Script Translator ===============================//

class ScriptTranslator
{
public:
    virtual void translate(ScriptCompiler *compiler, const AbstractNodePtr &node) = 0;
    
protected:
    virtual ~ScriptTranslator() {}
    
    void processNode(ScriptCompiler *compiler, const AbstractNodePtr &node);
    
    static bool getString(AbstractNode* node, String* str);
    
    static bool getInt(AbstractNode* node, int* val);
    
    static bool getFloat(AbstractNode* node, float* val);
    
    static bool getMatrix4(AbstractNodeList::const_iterator iter, AbstractNodeList::const_iterator end, Mat4* matrix);
    
    static AbstractNodeList::const_iterator getNodeAt(const AbstractNodeList &nodes, int index);
    
    static bool getConstantType(AbstractNodeList::const_iterator i, ShaderConstantType *scType);
};

//==================== Material Translator ===============================//

class MaterialTranslator : public ScriptTranslator
{
protected:
    Material *mMaterial;
    
public:
    MaterialTranslator();
    void translate(ScriptCompiler *compiler, const AbstractNodePtr &node);
};

//==================== Technique Translator ===============================//

class TechniqueTranslator : public ScriptTranslator
{
protected:
    Technique *mTechnique;
public:
    TechniqueTranslator();
    void translate(ScriptCompiler *compiler, const AbstractNodePtr &node);
};

//==================== Pass Translator ===============================//

class PassTranslator : public ScriptTranslator
{
protected:
    Pass *mPass;
public:
    PassTranslator();
    void translate(ScriptCompiler *compiler, const AbstractNodePtr &node);
protected:
    void translateVertexProgramRef(ScriptCompiler *compiler, ObjectAbstractNode *node);
    void translateFragmentProgramRef(ScriptCompiler *compiler, ObjectAbstractNode *node);
};

//==================== TextureUnit Translator ===============================//

class TextureUnitTranslator : public ScriptTranslator
{
protected:
    TextureUnitState *mTexUnitState;
public:
    TextureUnitTranslator();
    void translate(ScriptCompiler *compiler, const AbstractNodePtr &node);
};

//==================== Gpu Program Translator ===============================//

class GpuShaderTranslator : public ScriptTranslator
{
public:
    GpuShaderTranslator();
    void translate(ScriptCompiler *compiler, const AbstractNodePtr &node);
protected:
    void translateVertexShader(ScriptCompiler *compiler, ObjectAbstractNode *obj);
    void translateFragmentShader(ScriptCompiler *compiler, ObjectAbstractNode *obj);
};

//==================== Material Translator Manager ===============================//

// String map to content type.
typedef OrderHashMap<String, ShaderConstantContent> ConstantContentMap;

class ScriptTranslatorManager
{
public:
    ScriptTranslatorManager();
    ~ScriptTranslatorManager();
    
private:
    MaterialTranslator mMtlTranslator;
    TechniqueTranslator mTechTranslator;
    PassTranslator mPassTranslator;
    TextureUnitTranslator mTexUnitTranslator;
    GpuShaderTranslator mProgramTranslator;
    
public:
    size_t getNumTranslators();
    ScriptTranslator *getTranslator(const AbstractNodePtr& node);
    
    static ConstantContentMap _ccMap;
};

#endif /* SimScriptTranslator_hpp */
