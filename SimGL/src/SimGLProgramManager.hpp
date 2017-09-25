//
//  SimGLProgramManager.hpp
//  SimGL
//
//  Created by yangbin on 2017/7/22.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#ifndef SimGLProgramManager_hpp
#define SimGLProgramManager_hpp

#include "SimSingleton.hpp"
#include "SimCommon.hpp"

class GLProgram;
class GLShader;

class GLProgramManager : public Singleton<GLProgramManager> {
private:
    typedef HashMap<SimUInt64, GLProgram*> GLProgramCache;
public:
    GLProgramManager();
    ~GLProgramManager();
    
private:
    GLProgramCache _cache;
    
    GLShader* _verShader;
    GLShader* _fragShader;
    
    GLProgram* _activeProgram;
    
public:
    
    void setActiveVertexShader(GLShader *verShader);
    
    void setActiveFragmentShader(GLShader *fragShader);
    
    GLProgram* getActiveProgram();
};

#endif /* SimGLProgramManager_hpp */
