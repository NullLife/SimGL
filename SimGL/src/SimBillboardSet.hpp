//
//  SimBillboardSet.hpp
//  SimGL
//
//  Created by yangbin on 2017/10/11.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#ifndef SimBillboardSet_hpp
#define SimBillboardSet_hpp

#include "SimCommon.hpp"
#include "SimMovableObject.hpp"

class Billboard;

class BillboardSet : MovableObject
{
public:
    typedef Vector<Billboard*> BillBoardPool;
    
    BillboardSet(const String& name, const size_t size);
    ~BillboardSet();
    
    const String& getName() const { return _name; }
    
    Billboard* createBillboard(const Vec3& pos);
    
    void setMaterialPtr(const MaterialPtr& mtl);
    void setMaterialPtr(const String& mtlName);
    const MaterialPtr& getMaterial() const;
    
    virtual void updateRenderQueue(RenderQueue* queue);
    
private:
    String _name;
    size_t _size;
    size_t _billboardNum;
    
    MaterialPtr _mtlPtr;
    
    BillBoardPool _bbPool;
};

#endif /* SimBillboardSet_hpp */
