//
//  main.cpp
//  SimGL
//
//  Created by yangbin on 2017/9/5.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#include "ParticleApp.hpp"
#include "PlaneApp.hpp"
#include "ManualApp.hpp"
#include "InstanceApp.hpp"

int main(int argc, const char * argv[]) {
    ParticleApp app("ParticleApp");
    app.running();
    return 0;
}
