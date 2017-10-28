//
//  SimBillboard.hpp
//  SimGL
//
//  Created by yangbin on 2017/10/1.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#ifndef SimBillboard_hpp
#define SimBillboard_hpp

#include "SimCommon.hpp"

class Billboard
{
    friend class BillboardSet;
    
public:
    Billboard(const Vec3& pos);
    ~Billboard();
    
    const Vec3& getPosition() const { return _position; }
    
    void setOffset(const Vec3& offset);
    const Vec3& getOffset() const { return _offset; }

private:
    Vec3 _position;
    Vec3 _offset;
};

#endif /* SimBillboard_hpp */
