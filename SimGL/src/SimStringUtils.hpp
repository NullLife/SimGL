//
//  SimStringUtils.hpp
//  SimGL
//
//  Created by yangbin on 2017/7/14.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#ifndef SimStringUtils_hpp
#define SimStringUtils_hpp

#include <string>

class StringUtils {
public:
    /** Delete empty line and empty chars.
     */
    static void trim(std::string& str, bool left = true, bool right = true);
    
    static bool toInt(const std::string& str, int* val);
    
    static bool toFloat(const std::string& str, float* val);
    
    static int parseInt(const std::string& str, int defaultVal = 0);
};

#endif /* SimStringUtils_hpp */
