//
//  SimLogManager.cpp
//  SimGL
//
//  Created by yangbin on 2017/4/13.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#include "SimLogManager.hpp"
#include <iostream>
#include <assert.h>

template<> LogManager* LogManager::Singleton<LogManager>::mSingleton = nullptr;

LogManager::LogManager() {}

LogManager::~LogManager() {}

void LogManager::debug(std::stringstream& info) {
    if (OPEN_DEBUG)
        std::cout << info.str() << std::endl;
}

void LogManager::debug(const std::string& info) {
    if (OPEN_DEBUG)
        std::cout << info << std::endl;
}

void LogManager::debug(const std::string& tag, const std::string& info) {
    if (OPEN_DEBUG)
        std::cout << tag << ": "<< info << std::endl;
}


void LogManager::error(std::stringstream& info) {
    std::cout << info.str() << std::endl;
    assert(false);
}

void LogManager::error(const std::string& info) {
    std::cout << info << std::endl;
    assert(false);
}

void LogManager::error(const std::string& tag, const std::string& info) {
    std::cout << tag << ": "<< info << std::endl;
    assert(false);
}



