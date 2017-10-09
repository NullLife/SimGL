//
//  SimScriptTranslator.cpp
//  SimGL
//
//  Created by yangbin on 2017/7/17.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#include "SimScriptTranslator.hpp"

#include "SimMaterialManager.hpp"
#include "SimShaderManager.hpp"

#include "SimMaterial.hpp"
#include "SimTechnique.hpp"
#include "SimPass.hpp"
#include "SimGLShader.hpp"
#include "SimGLShaderParams.hpp"
#include "SimTextureUnitState.hpp"

void ScriptTranslator::processNode(ScriptCompiler *compiler, const AbstractNodePtr &node)
{
    if(node->mType != ANT_OBJECT)
        return;
    
    // Abstract objects are completely skipped
    if((static_cast<ObjectAbstractNode*>(node.get()))->mAbstract)
        return;
    
    // Retrieve the translator to use
    ScriptTranslator *translator = ScriptCompilerManager::getSingleton().getTranslator(node);
    
    if(translator)
        translator->translate(compiler, node);
}

bool ScriptTranslator::getString(AbstractNode* node, String* str)
{
    if (node->mType != ANT_ATOM)
        return false;
    
    *str = node->getValue();
    
    return true;
}

bool ScriptTranslator::getInt(AbstractNode* node, int* val)
{
    if (node->mType != ANT_ATOM)
        return false;
    
    return StringUtils::toInt(node->getValue(), val);
}

bool ScriptTranslator::getFloat(AbstractNode* node, float* val)
{
    if (node->mType != ANT_ATOM)
        return false;
    
    return StringUtils::toFloat(node->getValue(), val);
}

bool ScriptTranslator::getMatrix4(AbstractNodeList::const_iterator iter, AbstractNodeList::const_iterator end, Mat4* matrix)
{
    int n = 0;
    while(iter != end && n < 16)
    {
        if(iter != end)
        {
            float r = 0;
            if(getFloat((*iter).get(), &r))
                (*matrix)[n/4][n%4] = r;
            else
                return false;
        }
        else
            return false;
        
        ++iter;
        ++n;
    }
    return true;
}

AbstractNodeList::const_iterator ScriptTranslator::getNodeAt(const AbstractNodeList &nodes, int index)
{
    AbstractNodeList::const_iterator i = nodes.begin();
    int n = 0;
    while(i != nodes.end())
    {
        if(n == index)
            return i;
        ++i;
        ++n;
    }
    return nodes.end();
}

bool ScriptTranslator::getConstantType(AbstractNodeList::const_iterator i, ShaderConstantType *scType)
{
    String val;
    getString((*i).get(), &val);
    
    if(val.find("float") != String::npos)
    {
        int count = 1;
        if (val.size() == 6)
            count = StringUtils::parseInt(val.substr(5));
        else if (val.size() > 6)
            return false;
        
        if (count > 4 || count == 0)
            return false;
        
        *scType = (ShaderConstantType)(SCT_FLOAT1 + count - 1);
        
    }
    else if(val.find("double") != String::npos)
    {
        int count = 1;
        if (val.size() == 6)
            count = StringUtils::parseInt(val.substr(5));
        else if (val.size() > 6)
            return false;
        
        if (count > 4 || count == 0)
            return false;
        
        *scType = (ShaderConstantType)(SCT_DOUBLE1 + count - 1);
        
    }
    else if(val.find("int") != String::npos)
    {
        int count = 1;
        if (val.size() == 4)
            count = StringUtils::parseInt(val.substr(3));
        else if (val.size() > 4)
            return false;
        
        if (count > 4 || count == 0)
            return false;
        
        *scType = (ShaderConstantType)(SCT_INT1 + count - 1);
        
    }
    else if(val.find("matrix") != String::npos)
    {
        int count;
        if (val.size() == 7)
        {
            count = StringUtils::parseInt(val.substr(6, 1));
            if (count == 3)
                *scType = SCT_MATRIX_3X3;
            else
                *scType = SCT_MATRIX_4X4;
        }
        else
            return false;
    }
    
    return true;
}

//======================== MaterialTranslator ================================//

MaterialTranslator::MaterialTranslator() : mMaterial(nullptr)
{
}

void MaterialTranslator::translate(ScriptCompiler *compiler, const AbstractNodePtr &node)
{
    LogManager::getSingleton().debug(node->getValue());
    ObjectAbstractNode* obj = reinterpret_cast<ObjectAbstractNode*>(node.get());
    if (obj->mName.empty())
    {
        std::stringstream ss;
        ss << "The material no name in lineNo: " << node->mLineNo << ", in file: " << node->mFilename + "\n";
        LogManager::getSingleton().debug(ss);
    }
    
    // Create material and add to manager.
    mMaterial = new Material(obj->mFilename);
    MaterialManager::getSingleton().add(mMaterial);
    
    obj->mCxt = mMaterial;
    
    mMaterial->removeAllTechniques();
    
    for (AbstractNodePtr& child : obj->mChildren)
    {
        if (child->mType == ANT_PROPERTY)
        {
        }
        else if (child->mType == ANT_OBJECT)
            processNode(compiler, child);
    }
}

//======================== TechniqueTranslator ================================//

TechniqueTranslator::TechniqueTranslator() : mTechnique(nullptr)
{
}

void TechniqueTranslator::translate(ScriptCompiler *compiler, const AbstractNodePtr &node)
{
    LogManager::getSingleton().debug(node->getValue());
    
    ObjectAbstractNode* obj = reinterpret_cast<ObjectAbstractNode*>(node.get());
    Material* mtl = reinterpret_cast<Material*>(obj->mParent->mCxt);
    mTechnique = mtl->createTechnique();
    
    // Set name of technique
    if (!obj->mName.empty())
        mTechnique->setName(obj->mName);
    
    obj->mCxt = mTechnique;
    
    for (AbstractNodePtr& child : obj->mChildren)
    {
        if (child->mType == ANT_PROPERTY)
        {
            // nothing...
        }
        else if (child->mType == ANT_OBJECT)
        {
            processNode(compiler, child);
        }
    }
}

//======================== PassTranslator ================================//

PassTranslator::PassTranslator() : mPass(nullptr)
{
}

void PassTranslator::translate(ScriptCompiler *compiler, const AbstractNodePtr &node)
{
    LogManager::getSingleton().debug(node->getValue());
    
    ObjectAbstractNode* obj = reinterpret_cast<ObjectAbstractNode*>(node.get());
    Technique* tech = reinterpret_cast<Technique*>(obj->mParent->mCxt);
    mPass = tech->createPass();
    
    // Set name of pass
    if (!obj->mName.empty())
        mPass->setName(obj->mName);
    
    obj->mCxt = mPass;
    
    for (AbstractNodePtr& child : obj->mChildren)
    {
        if (child->mType == ANT_PROPERTY)
        {
            // nothing...
        }
        else if (child->mType == ANT_OBJECT)
        {
            processNode(compiler, child);
        }
    }
}

//======================== TextureUnitTranslator ================================//

TextureUnitTranslator::TextureUnitTranslator() : mTexUnitState(nullptr)
{
}

void TextureUnitTranslator::translate(ScriptCompiler *compiler, const AbstractNodePtr &node)
{
    LogManager::getSingleton().debug(node->getValue());
    
    ObjectAbstractNode* obj = reinterpret_cast<ObjectAbstractNode*>(node.get());
    Pass* pass = reinterpret_cast<Pass*>(obj->mParent->mCxt);
    
    // Create texture unit state.
    mTexUnitState = pass->createTextureUnitState();
    
    obj->mCxt = mTexUnitState;

    // Set property.
    for (AbstractNodePtr& child : obj->mChildren)
    {
        if (child->mType == ANT_PROPERTY)
        {
            PropertyAbstractNode *property = reinterpret_cast<PropertyAbstractNode*>(child.get());
            LogManager::getSingleton().debug(property->getValue());
            
            switch (property->mId)
            {
                case ID_TEXTURE:
                {
                    // Type.
                    AtomAbstractNode* atom1 = (AtomAbstractNode*) property->mValues[0].get();
                    Texture::TextureType texType = Texture::TEX_TYPE_2D;
                    
                    if (atom1->mId == ID_2D)
                        texType = Texture::TEX_TYPE_2D;
                    else if (atom1->mId == ID_CUBIC)
                        texType = Texture::TEX_TYPE_CUBIC;
                    
                    // Get numbers of picture.
                    int texNum = (int) property->mValues.size() - 1;
                    if (texNum == 1)
                    {
                        // Name.
                        String texName = property->mValues[1]->getValue();
                        
                        // Set texture.
                        mTexUnitState->setTexture(texName, texType);
                    }
                    else if (texNum == 6)
                    {
                        String nameBK = property->mValues[1]->getValue();
                        String nameDN = property->mValues[2]->getValue();
                        String nameFR = property->mValues[3]->getValue();
                        String nameLF = property->mValues[4]->getValue();
                        String nameRT = property->mValues[5]->getValue();
                        String nameUP = property->mValues[6]->getValue();
                        
                        String texName = nameBK + "," + nameDN + "," + nameFR + "," + nameLF + "," + nameRT + "," + nameUP;
                        mTexUnitState->setTexture(texName, texType);
                    }
                }
                    break;
                    
                case ID_SAMPLER:
                {
                    int val = 0;
                    getInt(property->mValues[0].get(), &val);
                    mTexUnitState->setTextureUnit(val);
                }
                    break;
                    
                case ID_FILTERING:
                {
                    const int num = (int) property->mValues.size();
                    String minFilter = property->mValues[0]->getValue();
                    String magFilter = property->mValues[1]->getValue();
                    
                    if ("linear" == minFilter)
                        mTexUnitState->setTextureFiltering(FT_MIN, FO_LINEAR);
                    else if ("near" == minFilter)
                        mTexUnitState->setTextureFiltering(FT_MIN, FO_NEAR);
                
                    if ("linear" == magFilter)
                        mTexUnitState->setTextureFiltering(FT_MAG, FO_LINEAR);
                    else if ("near" == magFilter)
                        mTexUnitState->setTextureFiltering(FT_MAG, FO_NEAR);
                    
                    if (num <=2)
                        break;
                    
                    String mipmapFilter = property->mValues[2]->getValue();
                    
                    if ("none" == mipmapFilter)
                        mTexUnitState->setTextureFiltering(FT_MIP, FO_NONE);
                    else if ("near" == mipmapFilter)
                        mTexUnitState->setTextureFiltering(FT_MIP, FO_NEAR);
                    else if ("linear" == mipmapFilter)
                        mTexUnitState->setTextureFiltering(FT_MIP, FO_LINEAR);
                    
                    if (num <=3)
                        break;
                    
                    int numMipmaps = 0;
                    getInt(property->mValues[3].get(), &numMipmaps);
                    mTexUnitState->setNumMipmaps(numMipmaps);
                }
                    break;
                    
                default:
                    break;
            }
            
        }
        else if (child->mType == ANT_OBJECT)
        {
            processNode(compiler, child);
        }
    }
}

//======================== GpuShaderTranslator ================================//

GpuShaderTranslator::GpuShaderTranslator()
{
}

void GpuShaderTranslator::translate(ScriptCompiler *compiler, const AbstractNodePtr &node)
{
    LogManager::getSingleton().debug(node->getValue());
    ObjectAbstractNode* obj = reinterpret_cast<ObjectAbstractNode*>(node.get());
    
    if (obj->mName.empty())
    {
        std::stringstream ss;
        ss << "The shader is no name at lineNo: "<< node->mLineNo << ", in file:" << node->mFilename << "\n";
        LogManager::getSingleton().debug(ss);
        return;
    }
    
    if (obj->mValues.empty())
    {
        std::stringstream ss;
        ss << "The shader is no information for language or version at lineNo: "<< node->mLineNo << ", in file:" << node->mFilename << "\n";
        LogManager::getSingleton().debug(ss);
        return;

    }
    
    if (obj->mId == ID_VERTEX_SHADER)
        translateVertexShader(compiler, obj);
    else if (obj->mId == ID_FRAGMENT_SHADER)
        translateFragmentShader(compiler, obj);
    else if (obj->mId == ID_GEOMETRY_SHADER)
        translateGeometryShader(compiler, obj);
}

void GpuShaderTranslator::translateVertexShader(ScriptCompiler *compiler, ObjectAbstractNode *obj)
{
    Pass* pass = reinterpret_cast<Pass*>(obj->mParent->mCxt);
    pass->setVertexShaderName(obj->mName);
    
    GLShader* vs = ShaderManager::getSingleton().createShader(obj->mName).get();
    vs->setType(GST_VERTEX);
    vs->setLanguage(obj->mValues[0]->getValue());
    vs->setLanguageVerison(obj->mValues[1]->getValue());
    
    obj->mCxt = vs;
    
    for (AbstractNodePtr& child : obj->mChildren)
    {
        if (child->mType == ANT_PROPERTY)
        {
            PropertyAbstractNode *property = reinterpret_cast<PropertyAbstractNode*>(child.get());
            
            if (property->mId == ID_PARAM_NAMED)
            {
                const size_t atomNum = property->mValues.size();
                if (atomNum < 2)
                {
                    std::stringstream ss;
                    ss << "The vertex shader is no property or less values at lineNo: "<< child->mLineNo << ", in file:" << child->mFilename << "\n";
                    LogManager::getSingleton().debug(ss);
                    return;
                }
                
                GLShaderParamsPtr params = vs->getParameters();
                
                AbstractNodeList::const_iterator i0 = getNodeAt(property->mValues, 0);
                AbstractNodeList::const_iterator i1 = getNodeAt(property->mValues, 1);
                AbstractNodeList::const_iterator k = getNodeAt(property->mValues, 2);
                
                AtomAbstractNode* atom0 = (AtomAbstractNode*)(*i0).get();
                AtomAbstractNode* atom1 = (AtomAbstractNode*)(*i1).get();
                
                String name = atom0->mValue;
                String type = atom1->mValue;
                
                if (type == "matrix4")
                {
                    Mat4 matirx;
                    getMatrix4(k, property->mValues.end(), &matirx);
                    params->addConstantDefinition(name, SCT_MATRIX_4X4, ScriptTranslatorManager::_ccMap[name]);
                    params->setNamedConstant(name, matirx);
                }
            }
        }
        else if (child->mType == ANT_OBJECT)
        {
            processNode(compiler, child);
        }
    }
}

void GpuShaderTranslator::translateFragmentShader(ScriptCompiler *compiler, ObjectAbstractNode *obj)
{
    Pass* pass = reinterpret_cast<Pass*>(obj->mParent->mCxt);
    pass->setFragmentShaderName(obj->mName);
    
    GLShader* fs = ShaderManager::getSingleton().createShader(obj->mName).get();
    fs->setType(GST_FRAGMENT);
    fs->setLanguage(obj->mValues[0]->getValue());
    fs->setLanguageVerison(obj->mValues[1]->getValue());
    
    obj->mCxt = fs;
    
    for (AbstractNodePtr& child : obj->mChildren)
    {
        if (child->mType == ANT_PROPERTY)
        {
            PropertyAbstractNode *property = reinterpret_cast<PropertyAbstractNode*>(child.get());
            LogManager::getSingleton().debug(property->getValue());
            
            if (property->mId == ID_PARAM_NAMED)
            {
                const size_t atomNum = property->mValues.size();
                if (atomNum < 2)
                {
                    std::stringstream ss;
                    ss << "The fragment shader is no property or less values at lineNo: "<< child->mLineNo << ", in file:" << child->mFilename << "\n";
                    LogManager::getSingleton().debug(ss);
                    return;
                }
                
                GLShaderParamsPtr params = fs->getParameters();
                
                AbstractNodeList::const_iterator i0 = getNodeAt(property->mValues, 0);
                AbstractNodeList::const_iterator i1 = getNodeAt(property->mValues, 1);
                AbstractNodeList::const_iterator k = getNodeAt(property->mValues, 2);
                
                AtomAbstractNode* atom0 = (AtomAbstractNode*)(*i0).get();
                AtomAbstractNode* atom1 = (AtomAbstractNode*)(*i1).get();
                
                String name = atom0->mValue;
                String type = atom1->mValue;
                
                if (type == "sampler2d")
                {
                    int val;
                    getInt((*k).get(), &val);
                    params->addConstantDefinition(name, SCT_SAMPLER2D, ScriptTranslatorManager::_ccMap[name]);
                    params->setNamedConstant(name, val);
                }
                
            }
        }
        else if (child->mType == ANT_OBJECT) {
            processNode(compiler, child);
        }
    }
}

void GpuShaderTranslator::translateGeometryShader(ScriptCompiler *compiler, ObjectAbstractNode *obj)
{
    Pass* pass = reinterpret_cast<Pass*>(obj->mParent->mCxt);
    pass->setGeometryShaderName(obj->mName);
    
    GLShader* gs = ShaderManager::getSingleton().createShader(obj->mName).get();
    gs->setType(GST_GEOMETRY);
    gs->setLanguage(obj->mValues[0]->getValue());
    gs->setLanguageVerison(obj->mValues[1]->getValue());
    
    obj->mCxt = gs;
    
    for (AbstractNodePtr& child : obj->mChildren)
    {
        if (child->mType == ANT_PROPERTY)
        {
            PropertyAbstractNode *property = reinterpret_cast<PropertyAbstractNode*>(child.get());
            LogManager::getSingleton().debug(property->getValue());
            
            if (property->mId == ID_PARAM_NAMED)
            {
                const size_t atomNum = property->mValues.size();
                if (atomNum < 2)
                {
                    std::stringstream ss;
                    ss << "The fragment shader is no property or less values at lineNo: "<< child->mLineNo << ", in file:" << child->mFilename << "\n";
                    LogManager::getSingleton().debug(ss);
                    return;
                }
                
                GLShaderParamsPtr params = gs->getParameters();
                
                AbstractNodeList::const_iterator i0 = getNodeAt(property->mValues, 0);
                AbstractNodeList::const_iterator i1 = getNodeAt(property->mValues, 1);
                
                AtomAbstractNode* atom0 = (AtomAbstractNode*)(*i0).get();
                AtomAbstractNode* atom1 = (AtomAbstractNode*)(*i1).get();
                
                String name = atom0->mValue;
                String type = atom1->mValue;
                
                if (type == "in")
                {
                    // Set draw type.
                }
                else if (type == "float")
                {
                    params->addConstantDefinition(name, SCT_FLOAT1, ScriptTranslatorManager::_ccMap[name]);
                }
            }
        }
        else if (child->mType == ANT_OBJECT)
        {
            processNode(compiler, child);
        }
    }
}

//======================== ScriptTranslatorManager ================================//

ScriptTranslatorManager::ScriptTranslatorManager()
{
}

ScriptTranslatorManager::~ScriptTranslatorManager()
{
    _ccMap.clear();
}

size_t ScriptTranslatorManager::getNumTranslators()
{
    return 5;
}

ScriptTranslator *ScriptTranslatorManager::getTranslator(const AbstractNodePtr& node)
{
    ScriptTranslator *translator = nullptr;
    
    if (node->mType == ANT_OBJECT)
    {
        ObjectAbstractNode *obj = reinterpret_cast<ObjectAbstractNode*>(node.get());
        ObjectAbstractNode *parent = obj->mParent ? reinterpret_cast<ObjectAbstractNode*>(obj->mParent) : nullptr;
        
        if(obj->mId == ID_MATERIAL)
            translator = &mMtlTranslator;
            
        else if (obj->mId == ID_TECHNIQUE && parent && parent->mId == ID_MATERIAL)
            translator = &mTechTranslator;
            
        else if (obj->mId == ID_PASS && parent && parent->mId == ID_TECHNIQUE)
            translator = &mPassTranslator;
            
        else if (obj->mId == ID_TEXTURE_UNIT && parent && parent->mId == ID_PASS)
            translator = &mTexUnitTranslator;
            
        else if (obj->mId == ID_VERTEX_SHADER ||
                   obj->mId == ID_FRAGMENT_SHADER ||
                   obj->mId == ID_GEOMETRY_SHADER ||
                   obj->mId == ID_COMPUTE_SHADER)
            translator = &mProgramTranslator;
    }
    
    return translator;
}
ConstantContentMap ScriptTranslatorManager::_ccMap =
{
    // Unknown content
    { "unknown", SCC_UNKNOWN },
    
    // Matrix
    { "model_matrix", SCC_MODEL_MATRIX },
    { "view_matrix", SCC_VIEW_MATRIX },
    { "projection_matrix", SCC_PROJECTIVE_MATRIX },
    { "mv_matrix", SCC_MV_MATRIX },
    { "mvp_matrix", SCC_MVP_MATRIX },
    
    // Position
    { "position_objectspace", SCC_POSITION_OBJECT_SPACE },
    { "position_worldspace", SCC_POSITION_WORLD_SPACE },
    { "position_viewspace", SCC_POSITION_VIEW_SPACE },
    { "position_projectivespace", SCC_POSITION_PROJECTIVE_SPACE },
    { "position_loghtspace", SCC_POSITION_LIGHT_SPACE },
    
    // Normal
    { "normal_objectspace", SCC_NORMAL_OBJECT_SPACE },
    { "normal_worldspace", SCC_NORMAL_WORLD_SPACE },
    { "normal_viewspace", SCC_NORMAL_VIEW_SPACE },
    { "normal_tangentspace", SCC_NORMAL_TANGENT_SPACE },
    
    // Tangent
    { "tangent_objectspace", SCC_TANGENT_OBJECT_SPACE },
    { "tangent_worldspace", SCC_TANGENT_WORLD_SPACE },
    { "tangent_viewspace", SCC_TANGENT_VIEW_SPACE },
    { "tangent_tangentspace", SCC_TANGENT_TANGENT_SPACE },
    
    // Binormal
    { "binormal_objectspace", SCC_BINORMAL_OBJECT_SPACE },
    { "binormal_worldspace", SCC_BINORMAL_WORLD_SPACE },
    { "binormal_viewspace", SCC_BINORMAL_VIEW_SPACE },
    { "binormal_tangentspace", SCC_BINORMAL_TANGENT_SPACE },
    
    // Color
    { "color_diffuse", SCC_COLOR_DIFFUSE },
    { "color_specular", SCC_COLOR_SPECULAR },
    
    // Depth
    { "depth_objectspace", SCC_DEPTH_OBJECT_SPACE },
    { "depth_worldspace", SCC_DEPTH_WORLD_SPACE },
    { "depth_viewspace", SCC_DEPTH_VIEW_SPACE },
    { "depth_projectivespace", SCC_DEPTH_PROJECTIVE_SPACE },
    
    // Texture coordinate
    { "texcoord0", SCC_TEXCOORDS0 },
    { "texcoord1", SCC_TEXCOORDS1 },
    { "texcoord2", SCC_TEXCOORDS2 },
    
    // Sampler
    { "tex_sampler", SCC_SAMPLER_2D },
    
    // Camera
    { "camera_position_objectspace", SCC_CAMERAPOSITION_OBJECT_SPACE },
    { "camera_position_worldspace", SCC_CAMERAPOSITION_WORLD_SPACE },
    { "camera_position_viewspace", SCC_CAMERAPOSITION_VIEW_SPACE },
    { "camera_position_tangentspace", SCC_CAMERAPOSITION_TANGENT_SPACE },
    
    // Light
    { "light_count", SCC_LIGHT_COUNT },
    { "light_type", SCC_LIGHT_TYPE },
    
    { "light_position_objectspace", SCC_LIGHTPOSITION_OBJECT_SPACE },
    { "light_position_worldspace", SCC_LIGHTPOSITION_WORLD_SPACE },
    { "light_position_viewspace", SCC_LIGHTPOSITION_VIEW_SPACE },
    { "light_position_tangentspace", SCC_LIGHTPOSITION_TANGENT_SPACE },
    
    { "light_direction_tangentspace", SCC_LIGHTDIRECTION_OBJECT_SPACE },
    { "light_direction_tangentspace", SCC_LIGHTDIRECTION_WORLD_SPACE },
    { "light_direction_tangentspace", SCC_LIGHTDIRECTION_VIEW_SPACE },
    { "light_direction_tangentspace", SCC_LIGHTDIRECTION_TANGENT_SPACE },
    
    { "light_diffuse_color", SCC_LIGHT_DIFFUSE_COLOR },
    { "light_specular_color", SCC_LIGHT_SPECULAR_COLOR },
    
    { "light_attenuation", SCC_LIGHT_ATTENUATION },
    { "light_intensity", SCC_LIGHT_INTENSITY },
    
    { "spot_light_cutoff", SCC_SPOT_LIGHT_CUTOFF },
    
    // Ambient light color
    { "ambient_light_color", SCC_AMBIENT_LIGHT_COLOR },
    
    // Material
    { "surface_ambient_color", SCC_SURFACE_AMBIENT_COLOR },
    { "surface_diffuse_color", SCC_SURFACE_DIFFUSE_COLOR },
    { "surface_specular_color", SCC_SURFACE_SPECULAR_COLOR },
    { "surface_emissive_color", SCC_SURFACE_EMISSIVE_COLOR },
    { "surface_shininess", SCC_SURFACE_SHININESS },
    
    { "time", SCC_TIME }
};
