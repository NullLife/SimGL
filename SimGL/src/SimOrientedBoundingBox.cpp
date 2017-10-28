//
//  SimOrientedBoundingBox.cpp
//  SimGL
//
//  Created by yangbin on 2017/10/19.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#include "SimOrientedBoundingBox.hpp"

const float INV_SQRT_TWO = 0.707106781f;
OrientedBoundingBox::OrientedBoundingBox() :
    _position(Vec3(0)),
    _orientation(Mat3(1.0)),
    _halfExtents(Vec3(-1.0f))
{
}

OrientedBoundingBox::~OrientedBoundingBox()
{
}

void OrientedBoundingBox::setNull()
{
    _position = Vec3(0);
    _orientation = Mat3(1.0);
    _halfExtents = Vec3(-1.0f);
}

bool OrientedBoundingBox::contain(const Vec3& point) const
{
    Vec3 pt = _position - point;
    
    return std::abs(glm::dot(pt, _orientation[0])) <= _halfExtents[0] &&
           std::abs(glm::dot(pt, _orientation[1])) <= _halfExtents[1] &&
           std::abs(glm::dot(pt, _orientation[2])) <= _halfExtents[2];
}

void OrientedBoundingBox::expandBy(const OrientedBoundingBox& obb)
{
    Vector<Vec3> targetVertices;
    obb.cornerPoints(targetVertices);
    cornerPoints(targetVertices);
    
    generateOBB(&targetVertices[0], targetVertices.size(), *this);
}

OrientedBoundingBox OrientedBoundingBox::expandBy(const OrientedBoundingBox* obbs, int num)
{
    Vector<Vec3> targetVertices;
    for (int i=0; i<num; ++i)
        obbs[i].cornerPoints(targetVertices);
    
    return create(&targetVertices[0], targetVertices.size());
}

const Vec3 OrientedBoundingBox::cornerPoint(int index) const
{
    // world
    Vec3 xl = _halfExtents.x * _orientation[0];
    Vec3 yl = _halfExtents.y * _orientation[1];
    Vec3 zl = _halfExtents.z * _orientation[2];
    
    switch(index)
    {
        default:// For release builds where assume() is disabled, return always the first option if out-of-bounds.
        case 0: return _position - xl - yl - zl;
        case 1: return _position - xl - yl + zl;
        case 2: return _position - xl + yl - zl;
        case 3: return _position - xl + yl + zl;
        case 4: return _position + xl - yl - zl;
        case 5: return _position + xl - yl + zl;
        case 6: return _position + xl + yl - zl;
        case 7: return _position + xl + yl + zl;
    }
}

void OrientedBoundingBox::cornerPoints(std::vector<Vec3>& pts) const
{
    // world
    Vec3 xl = _halfExtents.x * _orientation[0];
    Vec3 yl = _halfExtents.y * _orientation[1];
    Vec3 zl = _halfExtents.z * _orientation[2];
    
    pts.push_back(_position - xl - yl - zl);
    pts.push_back(_position - xl - yl + zl);
    pts.push_back(_position - xl + yl - zl);
    pts.push_back(_position - xl + yl + zl);
    pts.push_back(_position + xl - yl - zl);
    pts.push_back(_position + xl - yl + zl);
    pts.push_back(_position + xl + yl - zl);
    pts.push_back(_position + xl + yl + zl);
}

const Vec3 OrientedBoundingBox::facePoint(int index) const
{
    Vec3 xl = _halfExtents.x * _orientation[0];
    Vec3 yl = _halfExtents.y * _orientation[1];
    Vec3 zl = _halfExtents.z * _orientation[2];

    switch(index)
    {
        default: // For release builds where assume() is disabled, return always the first option if out-of-bounds.
        case 0: return _position - xl;
        case 1: return _position + xl;
        case 2: return _position - yl;
        case 3: return _position + yl;
        case 4: return _position - zl;
        case 5: return _position + zl;
    }
}

void OrientedBoundingBox::facePlanes(Vector<Plane>& faces) const
{
    for (int i=0; i<6; ++i) {
        const Vec3 c = facePoint(i);
        Vec3 n = glm::normalize( c - _position );
        faces.push_back(Plane(c, n));
    }
}

bool OrientedBoundingBox::intersectLine(const Ray& ray, float &tNear, float &tFar) const
{
    // For a Line-AABB test, pass in
    //    tNear = -FLOAT_INF;
    //    tFar = FLOAT_INF;
    // For a Ray-AABB test, pass in
    //    tNear = 0.f;
    //    tFar = FLOAT_INF;
    // For a LineSegment-AABB test, pass in
    //    tNear = 0.f;
    //    tFar = LineSegment.Length();
    
    // 把Ray转到OBB局部坐标系中，转化成与AABB求交
    Ray locRay = ray;
    
    // OBB 到 射线起点的向量
    Vec3 lineVec = ray._pos - _position;
    // 算投影
    float proX = glm::dot(_orientation[0], lineVec);
    float proY = glm::dot(_orientation[1], lineVec);
    float proZ = glm::dot(_orientation[2], lineVec);
    
    locRay._pos = Vec3(proX, proY, proZ);
    locRay._dir = -glm::normalize(locRay._pos);
    
    // 局部坐标系AABB
    Vec3 min = - _halfExtents;
    Vec3 max =   _halfExtents;
    
    if (fabsf(locRay._dir.x) > 0.0f)
    {
        float recipDir = 1 / locRay._dir.x;
        float t1 = (min.x - locRay._pos.x) * recipDir;
        float t2 = (max.x - locRay._pos.x) * recipDir;
        
        // tNear tracks distance to intersect (enter) the AABB.
        // tFar tracks the distance to exit the AABB.
        if (t1 < t2)
            tNear = std::max(t1, tNear), tFar = std::min(t2, tFar);
        else // Swap t1 and t2.
            tNear = std::max(t2, tNear), tFar = std::min(t1, tFar);
        
        if (tNear > tFar)
            return false; // Box is missed since we "exit" before entering it.
    }
    else if (locRay._pos.x < min.x || locRay._pos.x > max.x)
        return false;
    
    if (fabsf(locRay._dir.y) > 0.0f)
    {
        float recipDir = 1 / locRay._dir.y;
        float t1 = (min.y - locRay._pos.y) * recipDir;
        float t2 = (max.y - locRay._pos.y) * recipDir;
        
        if (t1 < t2)
            tNear = std::max(t1, tNear), tFar = std::min(t2, tFar);
        else // Swap t1 and t2.
            tNear = std::max(t2, tNear), tFar = std::min(t1, tFar);
        
        if (tNear > tFar)
            return false; // Box is missed since we "exit" before entering it.
    }
    else if (locRay._pos.y < min.y || locRay._pos.y > max.y)
        return false; // The ray can't possibly enter the box, abort.
    
    if (fabsf(locRay._dir.z) > 0.0f) // ray is parallel to plane in question
    {
        float recipDir = 1 / locRay._dir.z;
        float t1 = (min.z - locRay._pos.z) * recipDir;
        float t2 = (max.z - locRay._pos.z) * recipDir;
        
        if (t1 < t2)
            tNear = std::max(t1, tNear), tFar = std::min(t2, tFar);
        else // Swap t1 and t2.
            tNear = std::max(t2, tNear), tFar = std::min(t1, tFar);
    }
    else if (locRay._pos.z < min.z || locRay._pos.z > max.z)
        return false; // The ray can't possibly enter the box, abort.
    
    return tNear <= tFar;
}

OrientedBoundingBox OrientedBoundingBox::create(Vec3* vertices, size_t num)
{
    OrientedBoundingBox obb;
    
    generateOBB(vertices, num, obb);
    
    return obb;
}

void OrientedBoundingBox::generateOBB(Vec3* vertices, size_t num, OrientedBoundingBox& obb)
{
    // 计算均值
    Vec3 avg = Vec3(0);
    for(int i=0; i<num; i++)
    {
        avg += vertices[i];
    }
    avg /= static_cast<float> (num);
    
    obb._position = avg;
    
    Mat3 covMatrix = Mat3(1.0f);
    calcCovarianceMatrix(covMatrix, vertices, avg, num);
    
    
    Vec3 eValue = Vec3(0);
    Vec3 *eVectors = new Vec3[3];
    calcEspecialValues(covMatrix, eValue, eVectors);
    
    float temp;
    Vec3 tempVec;
    
    // sort to obtain eValue[0] <= eValue[1] <= eValue[2]:
    if (eValue[0] <= eValue[1])
    {
        if (eValue[1] > eValue[2])
        {
            if (eValue[0] > eValue[2])
            {
                temp = eValue[0];
                tempVec = eVectors[0];
                
                eValue[0] = eValue[2];
                eValue[2] = eValue[1];
                eValue[1] = temp;
                
                eVectors[0] = eVectors[2];
                eVectors[2] = eVectors[1];
                eVectors[1] = tempVec;
            }
            else
            {
                temp = eValue[1];
                tempVec = eVectors[1];
                
                eValue[1] = eValue[2];
                eValue[2] = temp;
                
                eVectors[1] = eVectors[2];
                eVectors[2] = tempVec;
            }
        }
        // else do nothing
    }
    else
    {
        if (eValue[0] > eValue[2])
        {
            if (eValue[1] < eValue[2])
            {
                temp = eValue[0];
                tempVec = eVectors[0];
                
                eValue[0] = eValue[1];
                eValue[1] = eValue[2];
                eValue[2] = temp;
                
                eVectors[0] = eVectors[1];
                eVectors[1] = eVectors[2];
                eVectors[2] = tempVec;
            }
            else
            {
                temp = eValue[0];
                tempVec = eVectors[0];
                
                eValue[0] = eValue[2];
                eValue[2] = temp;
                
                eVectors[0] = eVectors[2];
                eVectors[2] = tempVec;
            }
        }
        else
        {
            //  a <= c && a>b  ==> b < a <= c
            //            if (eValue[0] > eValue[1])
            //            {
            temp = eValue[0];
            tempVec = eVectors[0];
            
            eValue[0] = eValue[1];
            eValue[1] = temp;
            
            eVectors[0] = eVectors[1];
            eVectors[1] = tempVec;
            //            }
            // a <= b < c
            // nothing...
        }
    }
    
    // 从大到小排列  特征向量
    obb._orientation[0] = eVectors[2];   // R
    obb._orientation[1] = eVectors[1];   // S
    obb._orientation[2] = eVectors[0];   // T
    
    grahmSchmidt(obb._orientation[0], obb._orientation[1], obb._orientation[2]);
    
    // eigenbasis set- now center the OBB in the middle
    Vec3 min = Vec3(FLT_MAX);
    Vec3 max = Vec3(FLT_MIN);
    for (int index = 0; index < num; ++index)
    {
        Vec3 displacement = vertices[index] - obb._position;
        
        // 投影 到 新坐标轴上
        float len0 = glm::dot(displacement, obb._orientation[0]);
        float len1 = glm::dot(displacement, obb._orientation[1]);
        float len2 = glm::dot(displacement, obb._orientation[2]);
        
        min[0] = glm::min(min[0], len0);
        min[1] = glm::min(min[1], len1);
        min[2] = glm::min(min[2], len2);
        
        max[0] = glm::max(max[0], len0);
        max[1] = glm::max(max[1], len1);
        max[2] = glm::max(max[2], len2);
    }
    
    Vec3 offset = (max + min) * 0.5f;
    obb._position += ((offset.x * obb._orientation[0]) + (offset.y * obb._orientation[1]) + (offset.z * obb._orientation[2]));
    
    for (int i = 0; i < 3; ++i)
        obb._halfExtents[i] = (max[i] - min[i]) * 0.5f;
    
    delete [] eVectors;
}

/**
 * x与y的协方差：
 * cov(x, y) = E[(x-E(x))(y-E(y))] = E[xy - xE(y) - yE(x) + E(x)E(y)] = E(xy) - E(x)E(y) - E(x)E(y) + E(x)E(y) = E(xy) - E(x)E(y)
 *
 * 协方差矩阵： 协方差矩阵的每个元素是各个向量元素之间的协方差。
 * N维随机变量 { x1, ... , xn }
 * ---- cov(x1, x1), cov(x1, x2), ..., cov(x1, xn) ----
 * ---- cov(x2, x1), cov(x2, x2), ..., cov(x2, xn) ----
 * ---- ..........., ..........., ..., ........... ----
 * ---- cov(xn, x1), cov(xn, x2), ..., cov(xn, xn) ----
 */
void OrientedBoundingBox::calcCovarianceMatrix(Mat3& covMatrix, Vec3* vertices, Vec3 avg, size_t num)
{
    // 计算期望
    Vec3 *eVals = new Vec3[num];
    for(int i=0; i<num; i++)
    {
        eVals[i] = vertices[i] - avg;
    }
    
    // 计算协方差
    /**  {x, y, z}
     *  cov(x, x), cov(x, y), cov(x, z)
     *  cov(y, x), cov(y, y), cov(y, z)
     *  cov(z, x), cov(z, y), cov(z, z)
     */
    for (int c = 0; c < 3; ++c)
    {
        for (int r = c; r < 3; ++r)
        {
            float& acc = covMatrix[c][r];
            acc = 0;
            
            // cov(x, y) = E[(x-E(x))(y-E(y))]
            for (int i = 0; i < num; ++i)
            {
                acc += eVals[i][c] * eVals[i][r];
            }
            
            acc /= static_cast<float> (num);
            
            covMatrix[r][c] = acc;    // 对称矩阵
        }
    }
    
    delete [] eVals;
}

/** 雅可比迭代法求出特征值，特征向量--->就是轴
 */
void OrientedBoundingBox::calcEspecialValues(Mat3 m, Vec3& eigenvalues, Vec3 eigenVectors[3])
{
    Vec3 eps = Vec3(1.e-5);
    float p, q, spq;
    float cosa = 0, sina = 0;            // holds cos(alpha) and sin(alpha)
    float temp;                          // used for temporary storage
    float s1 = 0.0f;                     // sums of squares of diagonal
    float s2;                            // elements
    
    bool flag = true;                    // determines whether to iterate again.
    int iteration = 0;                   // iteration counter
    
    Vec3 mik;                            // used for temporary storage of m[i][k]
    
    Mat3 t = Mat3(1.0f);                 // stores the product of the rotation matrices.
    
    // Its columns ultimately hold the eigenvectors
    do
    {
        iteration++;
        
        for (int i = 0; i < 2; ++i)
        {
            for (int j = i + 1; j < 3; ++j)
            {
                if ((fabs(m[j][i]) < eps[0]))
                {
                    m[j][i] = 0.0f;
                }
                else
                {
                    q = fabs(m[i][i] - m[j][j]);
                    
                    if (q > eps[1])
                    {
                        p = 2.0f * m[j][i] * q / (m[i][i] - m[j][j]);
                        spq = sqrt(p * p + q * q);
                        cosa = sqrt((1.0f + q / spq) / 2.0f);
                        sina = p / (2.0f * cosa * spq);
                    }
                    else
                    {
                        sina = cosa = INV_SQRT_TWO;
                    }
                    
                    for (int k = 0; k < 3; ++k)
                    {
                        temp = t[i][k];
                        t[i][k] = temp * cosa + t[j][k] * sina;
                        t[j][k] = temp * sina - t[j][k] * cosa;
                    }
                    
                    for (int k = i; k < 3; ++k)
                    {
                        if (k > j)
                        {
                            temp = m[k][i];
                            m[k][i] = cosa * temp + sina * m[k][j];
                            m[k][j] = sina * temp - cosa * m[k][j];
                        }
                        else
                        {
                            mik[k] = m[k][i];
                            m[k][i] = cosa * mik[k] + sina * m[j][k];
                            
                            if (k == j)
                            {
                                m[k][j] = sina * mik[k] - cosa * m[k][j];
                            }
                        }
                    }
                    
                    mik[j] = sina * mik[i] - cosa * mik[j];
                    
                    for (int k = 0; k <= j; ++k)
                    {
                        if (k <= i)
                        {
                            temp = m[i][k];
                            m[i][k] = cosa * temp + sina * m[j][k];
                            m[j][k] = sina * temp - cosa * m[j][k];
                        }
                        else
                        {
                            m[j][k] = sina * mik[k] - cosa * m[j][k];
                        }
                    }
                }
            }
        }
        
        s2 = 0.0f;
        
        for (int i = 0; i < 3; ++i)
        {
            eigenvalues[i] = m[i][i];
            s2 += eigenvalues[i] * eigenvalues[i];
        }
        
        if (fabs(s2) < static_cast<float>(1.e-5) || fabs(1 - s1 / s2) < eps[2])
        {
            flag = false;
        }
        else
        {
            s1 = s2;
        }
    } while (flag);
    
    eigenVectors[0] = t[0];
    eigenVectors[1] = t[1];
    eigenVectors[2] = t[2];
    
    // preserve righthanded-ness:
    Vec3 cross = glm::cross(eigenVectors[0], eigenVectors[1]);
    if (glm::dot(cross, eigenVectors[2]) < 0.0f)
    {
        eigenVectors[2] = -eigenVectors[2];
    }
}

// 施密特正交化
void OrientedBoundingBox::grahmSchmidt(Vec3& v0, Vec3& v1, Vec3& v2)
{
    v0 = glm::normalize(v0);
    
//    v1 -= (v1 * v0) * v0;
    v1 -= glm::dot(v1, v0) * v0;
    
    v1 = glm::normalize(v1);
    
    v2 = glm::cross(v0, v1);    // no need to normalize
    
    v2 = glm::normalize(v2);
}

