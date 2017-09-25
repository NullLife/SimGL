//
//  SimMaterial.cpp
//  SimGL
//
//  Created by yangbin on 2017/1/4.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#include "SimMaterial.hpp"
#include "SimTechnique.hpp"

Material::Material(const String &name) :
        mName(name) {
            
    mAmbientIntensity = 0.25f;
    mSpecularIntensity = 0.5f;
    mShininess = 128.0f;
}

Material::~Material() {
    removeAllTechniques();
    LogManager::getSingleton().debug("Delete Material name", mName);
}

Technique* Material::createTechnique() {
    Technique* tech = new Technique();
    addTechnique(tech);
    return tech;
}

const int Material::addTechnique(Technique* technique) {
    assert(technique);
    const int index = static_cast<int>(mTechList.size());
    technique->setParent(this);
    
    mTechList.push_back(technique);
    return index;
}

void Material::removeAllTechniques() {
    for(TechniqueList::iterator iter = mTechList.begin(); iter != mTechList.end(); ++iter) {
        delete (*iter);
    }
    mTechList.clear();
    mTechList.shrink_to_fit();
}

const float& Material::getAmbientIntensity() {
    return mAmbientIntensity;
}

const float& Material::getSpecularIntensity() {
    return mSpecularIntensity;
}

const float& Material::getShininess() {
    return mShininess;
}
