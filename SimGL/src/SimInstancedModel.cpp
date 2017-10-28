//
//  SimInstancedModel.cpp
//  SimGL
//
//  Created by yangbin on 2017/10/18.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#include "SimInstancedModel.hpp"

#include "SimInstanceBatch.hpp"

InstancedModel::InstancedModel(InstanceBatch* batch, unsigned int instanceId) :
    _batch(batch),
    _instanceId(instanceId)
{
}

InstancedModel::~InstancedModel()
{
}
