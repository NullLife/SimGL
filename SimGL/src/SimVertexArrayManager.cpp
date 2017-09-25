//
//  SimVertexArrayManager.cpp
//  SimGL
//
//  Created by yangbin on 2017/8/30.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#include "SimVertexArrayManager.hpp"
#include "SimLogManager.hpp"

template<> VertexArrayManager *Singleton<VertexArrayManager>::mSingleton = nullptr;
VertexArrayManager::VertexArrayManager()
{
}

VertexArrayManager::~VertexArrayManager()
{
}

bool VertexArrayManager::cacheVao(SimUInt64 key, GLuint vao)
{
    if (mVaos.find(key) != mVaos.end())
    {
        LogManager::getSingleton().error("VertexArrayManager::cacheVao", "Can't cache same VAO object twice...");
        return false;
    }
    mVaos.insert(VaoHashMap::value_type(key, vao));
    return true;
}

GLuint VertexArrayManager::getVao(SimUInt64 key) const
{
    if (mVaos.find(key) == mVaos.end())
        return 0;
    return mVaos.at(key);
}

bool VertexArrayManager::deleteVao(SimUInt64 key)
{
    if (mVaos.find(key) == mVaos.end())
        return false;
    glDeleteVertexArrays(1, &mVaos.at(key));
    return true;
}

GLuint VertexArrayManager::removeVao(SimUInt64 key)
{
    if (mVaos.find(key) == mVaos.end())
        return 0;
    mVaos.erase(mVaos.find(key));
    return mVaos.find(key)->second;
}
