//
//  SimLogManager.hpp
//  SimGL
//
//  Created by yangbin on 2017/4/13.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#ifndef LogManager_hpp
#define LogManager_hpp

#include "SimSingleton.hpp"
#include <string>
#include <sstream>

static bool OPEN_DEBUG = true;

class LogManager : public Singleton<LogManager> {
public:
    LogManager();
    ~LogManager();

public:
    void debug(std::stringstream& info);

    void debug(const std::string& info);

    void debug(const std::string& tag, const std::string& info);
    
    void error(std::stringstream& info);
    
    void error(const std::string& info);
    
    void error(const std::string& tag, const std::string& info);
};

#endif /* LogManager_hpp */
