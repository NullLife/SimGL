//
//  SimBillboardSet.cpp
//  SimGL
//
//  Created by yangbin on 2017/10/11.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#include "SimBillboardSet.hpp"

#include "SimBillboard.hpp"

#include "SimMaterialManager.hpp"

BillboardSet::BillboardSet(const String& name, size_t size) :
    MovableObject(name),
    _name(name),
    _size(size)
{
    _bbPool.resize(size);
}

BillboardSet::~BillboardSet()
{
}

Billboard* BillboardSet::createBillboard(const Vec3& pos)
{
    Billboard* bb = new Billboard(pos);
    _bbPool.push_back(bb);
    return bb;
}

void BillboardSet::setMaterialPtr(const MaterialPtr& mtl)
{
    _mtlPtr = mtl;
}

void BillboardSet::setMaterialPtr(const String& mtlName)
{
    _mtlPtr = MaterialManager::getSingleton().get(mtlName);
    if (_mtlPtr == nullptr)
    {
        LogManager::getSingleton().error("BillboardSet::setMaterialPtr", "load material errors...");
    }
}

const MaterialPtr& BillboardSet::getMaterial() const
{
    return _mtlPtr;
}

void BillboardSet::updateRenderQueue(RenderQueue* queue)
{
}
