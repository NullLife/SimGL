//
//  SimMaterial.hpp
//  SimGL
//
//  Created by yangbin on 2017/1/4.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#ifndef Material_hpp
#define Material_hpp

#include "SimCommon.hpp"

class Technique;

class Material {
public:
    typedef Vector<Technique*> TechniqueList;
    
    Material(const String &name);
    ~Material();


private:
    String mName;

    TechniqueList mTechList;
    
    float mAmbientIntensity;
    
    float mSpecularIntensity;
    
    float mShininess;

public:
    const String &getName() { return mName; }
    
    Technique* createTechnique();
    
    const int addTechnique(Technique* technique);

    Technique *getBestTechnique() { return mTechList[0]; }
    
    const TechniqueList& getTechniqueList() { return mTechList; }
    
    void removeAllTechniques();
    
    const float& getAmbientIntensity();
    
    const float& getSpecularIntensity();
    
    const float& getShininess();
};

#endif /* Material_hpp */
