//
//  SimSingleton.hpp
//  SimGL
//
//  Created by yangbin on 2017/4/13.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#ifndef SimSingleton_hpp
#define SimSingleton_hpp

template <typename T>
class Singleton {
private:
    Singleton(Singleton const&) { };
    Singleton& operator=(Singleton const&){ return *mSingleton; }
    
protected:
    static T* mSingleton;
    
    Singleton() {
        mSingleton = static_cast< T* >( this );
    }
    virtual ~Singleton(){ }
    
    
public:
    
    static T& getSingleton() {
        return *getSingletonPtr();
    }
    static T* getSingletonPtr() {
        if (mSingleton == nullptr) {
            Singleton<T>::mSingleton = new T();
        }
        return mSingleton;
    }
    
    static void destroySingleton() {
        if (mSingleton == nullptr) {
            return;
        }
        delete mSingleton;
        mSingleton = nullptr;
    }
};

#endif /* SimSingleton_hpp */
