//
// Created by yangbin on 16/9/21.
// Copyright (c) 2016 cleb. All rights reserved.
//

#ifndef Shader_hpp
#define Shader_hpp

#include "SimCommon.hpp"

class Shader {
public:
    enum ShaderMemberType {
        SMT_ATTRIBUTE,
        SMT_UNIFORM
    };
    typedef HashMap<String, GLint> ShaderValueMap;

public:
    Shader(const String &filename);

    ~Shader();

private:
    String                         mName;
    GLuint                         mProgram;
    ShaderValueMap         mValueMap;

public:
    const String getName() const;

    const GLuint getProgram() const;

    void use() const;

    void stopUsing() const;

    void findShaderMembers(const String members[], int memberName, const ShaderMemberType memberType);
    
    void findShaderMember(const String& member, const ShaderMemberType memberType);

    GLint getShaderValue(const String &name);
    
    const bool setUniformMatrix4f(const String& member, const Mat4& matrix);
    
    const bool setUniform4fv(const String& member, const Vec4& value);
    
    const bool setUniform3fv(const String& member, const Vec3& value);
    
    const bool setUniform1f(const String& member, const float value);
    
    const bool setUniform1i(const String& member, const int value);

private:
    bool isInUsing() const;
    
    void initUniforms(const String members[], int num);
    
    void initAttributes(const String members[], int num);

    GLint getAttributeLocation(const String &member) const;

    GLint getUniformLocation(const String &member) const;

    // compile shader
    void compileShader();

    GLuint compile(const String &filePath, const GLenum type);
};


#endif //Shader_hpp
