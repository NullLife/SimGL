//
//  SimVertexArrayManager.hpp
//  SimGL
//
//  Created by yangbin on 2017/8/30.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#ifndef SimVertexArrayManager_hpp
#define SimVertexArrayManager_hpp

#include "SimSingleton.hpp"
#include "SimCommon.hpp"

class VertexArrayManager : public Singleton<VertexArrayManager>
{
public:
    typedef HashMap<SimUInt64, GLuint> VaoHashMap;
    
    VertexArrayManager();
    ~VertexArrayManager();
    
    bool cacheVao(SimUInt64 key, GLuint vao);
    
    GLuint getVao(SimUInt64 key) const;
    
    bool deleteVao(SimUInt64 key);
    
    GLuint removeVao(SimUInt64 key);
    
private:
    VaoHashMap mVaos;
};

#endif /* SimVertexArrayMananger_hpp */
