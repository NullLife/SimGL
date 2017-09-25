//
// Created by yangbin on 16/9/21.
// Copyright (c) 2016 cleb. All rights reserved.
//

#include "SimShader.hpp"
#include <fstream>

Shader::Shader(const String &filename) : mName(filename) {
    compileShader();
}

Shader::~Shader() {
    LogManager::getSingleton().debug("delete Shader");
    if (mProgram != 0) {
        glDeleteProgram(mProgram);
    }
    mValueMap.clear();
}

const String Shader::getName() const {
    return mName;
}

const GLuint Shader::getProgram() const {
    return mProgram;
}

void Shader::compileShader() {
    LogManager::getSingleton().debug("shader: " + mName);
    
    std::string filepath         = SHADER_RESOURCE_DIR + mName;
    //compile shaders
    GLuint      vertexShaderID   = compile(filepath + "_vs.glsl", GL_VERTEX_SHADER);
    GLuint      fragmentShaderID = compile(filepath + "_fs.glsl", GL_FRAGMENT_SHADER);

    //create the program object
    mProgram = glCreateProgram();
    if (mProgram == 0) {
        LogManager::getSingletonPtr()->debug("Shader::compileShader",
                                           "glCreateProgram failed");
    }

    //attach all the shaders
    glAttachShader(mProgram, vertexShaderID);
    glAttachShader(mProgram, fragmentShaderID);

    //link the shaders together
    glLinkProgram(mProgram);

    //detach all the shaders
    glDetachShader(mProgram, vertexShaderID);
    glDetachShader(mProgram, fragmentShaderID);

    //throw exception if linking failed
    GLint status = -1;
    glGetProgramiv(mProgram, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        String msg("Program linking failure: ");

        GLint infoLogLength;
        glGetProgramiv(mProgram, GL_INFO_LOG_LENGTH, &infoLogLength);
        char *strInfoLog = new char[infoLogLength + 1];
        glGetProgramInfoLog(mProgram, infoLogLength, NULL, strInfoLog);
        msg += strInfoLog;
        delete[] strInfoLog;

        glDeleteProgram(mProgram);
        mProgram = 0;
        LogManager::getSingletonPtr()->debug("Shader::compileShader", msg);
    }

}

GLuint Shader::compile(const std::string &filePath, const GLenum type) {
    //create the shader object
    GLuint shaderID = glCreateShader(type);
    if (shaderID == 0) {
        LogManager::getSingletonPtr()->debug("Shader::compileShader",
                                           "glCreateShader failed");
    }
    //open file
    std::ifstream f;
    f.open(filePath.c_str(), std::ios::in | std::ios::binary);
    if (!f.is_open()) {
        LogManager::getSingletonPtr()->debug("Shader::compileShader", "Failed to open file: " + filePath);
    }

    //read whole file into std::stringstream buffer
    std::stringstream buffer;
    buffer << f.rdbuf();

    //set the source code
    std::string con   = buffer.str();
    const char  *code = con.c_str();
    glShaderSource(shaderID, 1, &code, NULL);
    //compile
    glCompileShader(shaderID);

    //throw exception if compile error occurred
    GLint status;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        std::string msg("Compile failure in " + filePath + ":\n");

        GLint infoLogLength;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
        char *strInfoLog = new char[infoLogLength + 1];
        glGetShaderInfoLog(shaderID, infoLogLength, NULL, strInfoLog);
        msg += strInfoLog;
        delete[] strInfoLog;

        glDeleteShader(shaderID);
        shaderID = 0;
        LogManager::getSingletonPtr()->debug("Shader::compileShader", msg);
    }

    return shaderID;
}

void Shader::use()const {
    if(isInUsing()) {
        return;
    }
    glUseProgram(mProgram);
}

bool Shader::isInUsing() const {
    GLint currentProgram = 0;
    glGetIntegerv(GL_CURRENT_PROGRAM, &currentProgram);
    return (currentProgram == (GLint) mProgram);
}

void Shader::stopUsing() const {
    if (isInUsing()) {
        glUseProgram(0);
    }
}

void Shader::findShaderMembers(const String members[], int memberNum, const ShaderMemberType memberType) {
    if (memberType == ShaderMemberType::SMT_ATTRIBUTE) {
        initAttributes(members, memberNum);
    } else {
        initUniforms(members, memberNum);
    }
}

void Shader::findShaderMember(const String& member, const ShaderMemberType memberType) {
    if (memberType == ShaderMemberType::SMT_ATTRIBUTE) {
        initAttributes(&member, 1);
    } else {
        initUniforms(&member, 1);
    }
}

GLint Shader::getShaderValue(const String &member) {
    auto it = mValueMap.find(member);
    if (it == mValueMap.end()) {
        LogManager::getSingleton().debug("The "+ member + " can't be found in shader!!!");
        return -1;
    }
    return mValueMap[member];
}

const bool Shader::setUniformMatrix4f(const String& member, const Mat4& matrix) {
    GLint POS = getShaderValue(member);
    if (POS < 0) {
        return false;
    }
    glUniformMatrix4fv(POS, 1, GL_FALSE, &matrix[0][0]);
    return true;
}

const bool Shader::setUniform4fv(const String &member, const Vec4 &value) {
    GLint POS = getShaderValue(member);
    if (POS < 0) {
        return false;
    }
    glUniform4fv(POS, 1, &value[0]);
    return true;
}

const bool Shader::setUniform3fv(const String &member, const Vec3 &value) {
    GLint POS = getShaderValue(member);
    if (POS < 0) {
        return false;
    }
    glUniform3fv(POS, 1, &value[0]);
    return true;
}


const bool Shader::setUniform1f(const String &member, const float value) {
    GLint POS = getShaderValue(member);
    if (POS < 0) {
        return false;
    }
    glUniform1f(POS, value);
    return true;
}

const bool Shader::setUniform1i(const String& member, const int value) {
    GLint POS = getShaderValue(member);
    if (POS < 0) {
        return false;
    }
    glUniform1i(POS, value);
    return true;
}

void Shader::initUniforms(const String members[], int num) {
    for (int i=0; i<num; ++i) {
        auto it = mValueMap.find(members[i]);
        if (it != mValueMap.end()) {
            continue;
        }
        LogManager::getSingleton().debug("findShaderName: "+members[i]);
        GLint value = getUniformLocation(members[i]);
        if (value <0) {
            LogManager::getSingleton().debug("The "+ members[i] + " can't be found in shader!!!");
            continue;
        }
        mValueMap.insert(ShaderValueMap::value_type(members[i], value));
    }
}

void Shader::initAttributes(const String members[], int num) {
    for (int i=0; i<num; ++i) {
        auto it = mValueMap.find(members[i]);
        if (it != mValueMap.end()) {
            continue;
        }
        LogManager::getSingleton().debug("findShaderName: "+members[i]);
        GLint value = getAttributeLocation(members[i]);
        if (value <0) {
            LogManager::getSingleton().debug("The "+ members[i] + " can't be found in shader!!!");
            continue;
        }
        mValueMap.insert(ShaderValueMap::value_type(members[i], value));
    }
}


GLint Shader::getAttributeLocation(const String &member) const {
    return glGetAttribLocation(mProgram, member.c_str());
}


GLint Shader::getUniformLocation(const String &member) const {
    return glGetUniformLocation(mProgram, member.c_str());
}
