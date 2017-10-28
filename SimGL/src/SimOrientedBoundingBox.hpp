//
//  SimOrientBoundingBox.hpp
//  SimGL
//
//  Created by yangbin on 2017/10/18.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#ifndef SimOrientBoundingBox_hpp
#define SimOrientBoundingBox_hpp

#include "SimCommon.hpp"

class OrientedBoundingBox
{
public:
    struct Plane
    {
        Plane() {}
        Plane(const Vec3& pos, const Vec3& normal) : _pos(pos), _normal(normal) {}
        
        Vec3 _pos;        // 点
        Vec3 _normal;     // 法向量 单位化了
    };
    struct Ray
    {
        Ray() {}
        Ray(const Vec3& pos, const Vec3& dir) : _pos(pos), _dir(dir) {}
        
        Vec3 _pos;        // 点
        Vec3 _dir;        // 方向  单位化了
    };

public:
    OrientedBoundingBox();
    ~OrientedBoundingBox();
    
    static OrientedBoundingBox create(Vec3* vertices, size_t num);
    static void calcCovarianceMatrix(Mat3& covMatrix, Vec3* vertices, Vec3 avg, size_t num);
    static void calcEspecialValues(Mat3 m, Vec3& eigenvalues, Vec3 eigenVectors[3]);
    static void grahmSchmidt(Vec3& v0, Vec3& v1, Vec3& v2);
    
    inline bool valid() const
    {
        return _halfExtents.x>0 &&  _halfExtents.y>0 &&  _halfExtents.z>0;
    }
    
    const Vec3 cornerPoint(int index) const;
    const Vec3 facePoint(int index) const;
    void cornerPoints(std::vector<Vec3>& pts) const;
    void facePlanes(Vector<Plane>& faces) const;
    
    bool intersectLine(const Ray& ray, float &tNear, float &tFar) const;
    
    void setNull();
    bool contain(const Vec3& point) const;
    void expandBy(const OrientedBoundingBox& obb);
    static OrientedBoundingBox expandBy(const OrientedBoundingBox* obbs, int num);
    
    Vec3 _position;
    Vec3 _halfExtents;
    
    Mat3 _orientation;
    
private:
    static void generateOBB(Vec3* vertices, size_t num, OrientedBoundingBox& outOBB);
};

#endif /* SimOrientBoundingBox_hpp */
