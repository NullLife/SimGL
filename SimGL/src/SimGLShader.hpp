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

enum GLShaderType
{
    GST_VERTEX = 1,
    GST_FRAGMENT,
    GST_GEOMETRY
};

enum TransformFeedbackMode
{
    TFM_SEPARATE = GL_SEPARATE_ATTRIBS,
    TFM_INTERLEAVED = GL_INTERLEAVED_ATTRIBS
};

typedef SharedPtr<GLShaderParams> GLShaderParamsPtr;
typedef Vector<String> TransformFeedbackVaryings;

class GLShader
{
public:
    GLShader(const String& name);
    ~GLShader();

    const String& getName() { return _name; }
    void setName(const String& name) { _name = name; }
    
    const GLShaderType getType() { return _type; }
    void setType(GLShaderType type) { _type = type; }

    const String getLanguage() { return _language; }
    void setLanguage(const String& language) { _language = language; }
    
    const String getLanguageVerison() { return _verison; }
    void setLanguageVerison(const String& verison) { _verison = verison; }
    
    const GLShaderParamsPtr& getParameters();
    
    void setTransformFeedbackMode(const TransformFeedbackMode mode);
    
    void addFeedbackVarying(const String varying);
    
    void _setTransformFeedbackVaryings(GLuint program);
    
    GLuint getId() { return _id; };
    
    bool _compile();
    
private:
    GLShaderType _type;
    String _name;
    String _language;
    String _verison;
    GLShaderParamsPtr _shaderParams;
    
    TransformFeedbackVaryings _feedbackVaryings;
    TransformFeedbackMode _feedbackMode;
    
    bool _compiled;
    GLuint _id;
};

#endif /* SimGLShader_hpp */
