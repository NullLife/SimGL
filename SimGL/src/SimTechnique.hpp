//
//  SimTechnique.hpp
//  ClebGL
//
//  Created by yangbin on 2017/1/11.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#ifndef Technique_hpp
#define Technique_hpp

#include "SimCommon.hpp"

class Material;
class Pass;


class Technique {
public:
    typedef Vector<Pass*> PassList;
    
    Technique();
    Technique(const String& name);
    ~Technique();
    
private:
    String mName;

    PassList mPassList;
    
    Material* mParent;

public:
    const String& getName() { return mName; }
    void setName(const String& name) { mName= name; }
    
    void setParent(Material* mtl) { mParent = mtl; }
    
    const Material* getParent() { return mParent; }
    
    Pass* createPass();

    const int addPass(Pass* pass);

    Pass* getPass(const int index);

    const PassList& getPassList();
};

#endif /* Technique_hpp */
