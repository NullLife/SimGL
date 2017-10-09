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

void StringUtils::split(std::vector<std::string>& result, const std::string& str, const std::string& pattern)
{
    size_t index = 0;
    size_t pos = 0;
    while (pos != -1)
    {
        pos = str.find(pattern, index);
        result.push_back(str.substr(index, pos-index));
        index = pos + 1;
    }
}

std::string StringUtils::toString(const glm::vec3& val)
{
    return "x: " + std::to_string(val[0]) + ", y: " +  std::to_string(val[1]) + ", z: " +  std::to_string(val[2]);
}

std::string StringUtils::toString(const glm::vec4& val)
{
    return "x: " + std::to_string(val[0]) + ", y: " +  std::to_string(val[1]) + ", z: " +  std::to_string(val[2]) + ", w: " + std::to_string(val[3]);
}

std::string StringUtils::toString(const int val)
{
    return std::to_string(val);
}

std::string StringUtils::toString(const float val)
{
    return std::to_string(val);
}

std::string StringUtils::toString(const double val)
{
    return std::to_string(val);
}
