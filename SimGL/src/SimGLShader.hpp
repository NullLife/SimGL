//
//  SimGLShader.hpp
//  SimGL
//
//  Created by yangbin on 2017/7/11.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#ifndef SimGLShader_hpp
#define SimGLShader_hpp

#include "SimCommon.hpp"

class GLShaderParams;

enum GLShaderType {
    GST_VERTEX = 1,
    GST_FRAGMENT,
    GST_GEOMETRY
};

typedef SharedPtr<GLShaderParams> GLShaderParamsPtr;

class GLShader
{
public:
    GLShader(const String& name);
    ~GLShader();
    
private:
    GLShaderType mType;
    String mName;
    String mLanguage;
    String mVerison;
    GLShaderParamsPtr mShaderParams;
    
    bool mCompiled;
    GLuint mId;
    
public:
    const String& getName() { return mName; }
    void setName(const String& name) { mName = name; }
    
    const GLShaderType getType() { return mType; }
    void setType(GLShaderType type) { mType = type; }

    const String getLanguage() { return mLanguage; }
    void setLanguage(const String& language) { mLanguage = language; }
    
    const String getLanguageVerison() { return mVerison; }
    void setLanguageVerison(const String& verison) { mVerison = verison; }
    
    const GLShaderParamsPtr& getParameters();
    
    GLuint getId() { return mId; };
    
    bool _compile();
};

#endif /* SimGLShader_hpp */
