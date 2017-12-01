//
//  SimInstancedModel.hpp
//  SimGL
//
//  Created by yangbin on 2017/10/18.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#ifndef SimInstancedModel_hpp
#define SimInstancedModel_hpp

#include "SimCommon.hpp"

class InstanceBatch;
class Transform;

class InstancedModel
{
    friend InstanceBatch;
    
public:
    InstancedModel(InstanceBatch* batch, unsigned int instanceId);
    ~InstancedModel();
    
    Transform* getTransform() { return _transform; }
    
private:
    InstanceBatch* _batch;
    unsigned int _instanceId;
    
    Transform* _transform;
};

#endif /* SimInstancedModel_hpp */
