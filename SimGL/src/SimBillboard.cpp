//
//  SimBillboard.cpp
//  SimGL
//
//  Created by yangbin on 2017/10/1.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#include "SimBillboard.hpp"

Billboard::Billboard(const Vec3& pos) :
    _position(pos),
    _offset(Vec3(1))
{
}

Billboard::~Billboard()
{
}

void Billboard::setOffset(const Vec3& offset)
{
    _offset = offset;
}
