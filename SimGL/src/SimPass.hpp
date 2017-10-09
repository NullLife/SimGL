//
// Created by yangbin on 2017/2/28.
// Copyright (c) 2017 yangbin. All rights reserved.
//

#ifndef CLEBGL_PASS_HPP
#define CLEBGL_PASS_HPP

#include "SimTechnique.hpp"

class TextureUnitState;
class GLSLShader;
class ParameterDataSource;

class Pass
{
public:
    typedef Vector<TextureUnitState *> TextureUnitStateList;
    
    Pass();
    Pass(const String &name);
    ~Pass();

private:
    String _name;
    Technique *_parent;
    String _tag;
    
    String _verShaderName;
    String _fragShaderName;
    String _geoShaderName;
    
    String _shaderAttrs;
    String _shaderUniforms;

    GLShader* _verShader;
    GLShader* _geoShader;
    GLShader* _fragShader;
    
    bool _lightEnable;
    
    TextureUnitStateList _texStateList;
    
    // Material infos.
    Vec4 _ambient;
    Vec4 _diffuse;
    Vec4 _specular;
    Vec4 _emission;
    float _shininess;


public:
    const String &getName() { return _name; }
    void setName(const String& name) { _name= name;}
    
    void setParent(Technique* technique) { _parent = technique; }
    Technique* getParent() { return _parent; }
    
    void setTag(const String& tag) { _tag = tag; }
    const String& getTag() { return _tag; }
    
    void setVertexShaderName(const String& shaderName);
    const String& getVertexShaderName() { return _verShaderName; }
    
    void setGeometryShaderName(const String& shaderName);
    const String& getGeometryShaderName() { return _geoShaderName; }
    
    void setFragmentShaderName(const String& shaderName);
    const String& getFragmentShaderName() { return _fragShaderName; }
    
    void setLightEnable(const bool enable) { _lightEnable = enable; }
    const bool getLightEnable() { return _lightEnable; }

    TextureUnitStateList& getTextureUnitStateList() { return _texStateList; }
    
    TextureUnitState* createTextureUnitState();
    
    int addTextureUnitState(TextureUnitState* texState);
    void addTextureUnitStates(TextureUnitState** texStates, int texStateNum);

    // FixMe: should use reference in here!!!
    GLShader* getVertexShader();
    GLShader* getGeometryShader();
    GLShader* getFragmentShader();
    
    // Update parameters of shaders.
    void updateParameters(ParameterDataSource* dataSource);
    
    void setAmbient(const Vec4& diffuse);
    const Vec4& getAmbient() const;
    
    void setDiffuse(const Vec4& diffuse);
    const Vec4& getDiffuse() const;
    
    void setSpecular(const Vec4& diffuse);
    const Vec4& getSpecular() const;
    
    void setEmission(const Vec4& emission);
    const Vec4& getEmission() const;
    
    // Set smooth. its range is 0-128 and 128 is the most smooth.
    void setShininess(float shininess);
    float getShininess() const;
};


#endif //CLEBGL_PASS_HPP
