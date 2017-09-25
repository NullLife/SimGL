//
//  SimStringUtils.cpp
//  SimGL
//
//  Created by yangbin on 2017/7/14.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#include "SimStringUtils.hpp"
#include <sstream>

void StringUtils::trim(std::string& str, bool left, bool right) {
    static const std::string delims = " \t\r";
    
    if(left) {
        str.erase(0, str.find_first_not_of(delims)); // trim left
    }
    
    if(right) {
        str.erase(str.find_last_not_of(delims)+1); // trim right
    }
}

bool StringUtils::toInt(const std::string& str, int* val) {
    int n = sscanf(str.c_str(), "%d", val);
    if (n == 0 || n == EOF) {
        return false;
    }
    return true;
}


bool StringUtils::toFloat(const std::string& str, float* val) {
    int n = sscanf(str.c_str(), "%f", val);
    if (n == 0 || n == EOF) {
        return false;
    }
    return true;
}

int StringUtils::parseInt(const std::string& str, int defaultVal) {
    std::stringstream ss(str);
    int ret = defaultVal;
    if( !(ss >> ret) )
        return defaultVal;
    return ret;
}
