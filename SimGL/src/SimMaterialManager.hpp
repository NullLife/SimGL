//
//  SimMaterialManager.hpp
//  SimGL
//
//  Created by yangbin on 2017/7/1.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#ifndef SimMaterialManager_hpp
#define SimMaterialManager_hpp

#include "SimSingleton.hpp"
#include "SimCommon.hpp"

class Material;
class Pass;

class MaterialManager : public Singleton<MaterialManager> {
public:
    typedef HashMap<String, MaterialPtr> MaterialCache;
    
    MaterialManager();
    ~MaterialManager();
    
private:
    MaterialCache mCache;
    
public:
    MaterialPtr get(const String& name);
    
    MaterialPtr add(Material* mtl);
    
    bool remove(const String& name);
    
private:
    bool _load(const String& name);
};

#endif /* SImMaterialManager_hpp */
