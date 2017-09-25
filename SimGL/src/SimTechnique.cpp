//
//  Technique.cpp
//  ClebGL
//
//  Created by yangbin on 2017/1/11.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#include "SimMaterial.hpp"
#include "SimTechnique.hpp"
#include "SimPass.hpp"

Technique::Technique() :
        mName(""),
        mParent(nullptr) {
    
}

Technique::Technique(const String &name) :
        mName(name),
        mParent(nullptr) {
}

Technique::~Technique() {
    for (PassList::iterator iter = mPassList.begin(); iter!=mPassList.end(); ++iter) {
        delete (*iter);
    }
    mPassList.clear();
    mPassList.shrink_to_fit();
    
    LogManager::getSingleton().debug("Delete Technique name", mName);
}

Pass* Technique::createPass() {
    Pass* pass = new Pass();
    addPass(pass);
    return pass;
}

const int Technique::addPass(Pass *pass) {
    assert(pass);
    const int index = static_cast<int>(mPassList.size());
    pass->setParent(this);
    mPassList.push_back(pass);
    return index;
}

Pass *Technique::getPass(const int index) {
    return mPassList[index];
}

const Technique::PassList &Technique::getPassList() {
    return mPassList;
}


