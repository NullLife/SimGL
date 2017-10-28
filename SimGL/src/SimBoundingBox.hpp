//
//  SimBoundingBox.hpp
//  SimGL
//
//  Created by yangbin on 2017/10/11.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#ifndef SimBoundingBox_hpp
#define SimBoundingBox_hpp

#include "SimCommon.hpp"

template <typename VT>
class BoundingBoxImpl
{
public:
    typedef VT vec_type;
    typedef typename VT::value_type value_type;
    
    /** Minimum extent. (Smallest X, Y, and Z values of all coordinates.) */
    vec_type _min;
    /** Maximum extent. (Greatest X, Y, and Z values of all coordinates.) */
    vec_type _max;
    
    /** Creates an uninitialized bounding box. */
    inline BoundingBoxImpl() :
        _min(FLT_MAX, FLT_MAX, FLT_MAX),
        _max(-FLT_MAX, -FLT_MAX, -FLT_MAX)
    {}
    
    /** Creates a bounding box initialized to the given extents. */
    inline BoundingBoxImpl(value_type xmin, value_type ymin, value_type zmin,
                           value_type xmax, value_type ymax, value_type zmax) :
        _min(xmin,ymin,zmin),
        _max(xmax,ymax,zmax)
    {}
    
    /** Creates a bounding box initialized to the given extents. */
    inline BoundingBoxImpl(const vec_type& min,const vec_type& max) :
        _min(min),
        _max(max)
    {}
    
    /** Clear the bounding box. Erases existing minimum and maximum extents. */
    inline void init()
    {
        _min = Vec3(FLT_MAX, FLT_MAX, FLT_MAX);
        _max = Vec3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
    }
    
    /** Returns true if the bounding box extents are valid, false otherwise. */
    inline bool valid() const
    {
        return _max.x>=_min.x &&  _max.y>=_min.y &&  _max.z>=_min.z;
    }
    
    /** Sets the bounding box extents. */
    inline void set (value_type xmin, value_type ymin, value_type zmin,
                     value_type xmax, value_type ymax, value_type zmax)
    {
        _min = Vec3(xmin,ymin,zmin);
        _max = Vec3(xmax,ymax,zmax);
    }
    
    /** Sets the bounding box extents. */
    inline void set(const vec_type& min,const vec_type& max)
    {
        _min = min;
        _max = max;
    }
    
    /** Calculates and returns the bounding box center. */
    inline const vec_type center() const
    {
        return (_min+_max)*0.5;
    }
    
    /** Calculates and returns the bounding box radius. */
    inline value_type radius() const
    {
        return sqrt(radius2());
    }
    
    /** Calculates and returns the squared length of the bounding box radius.
     * Note, radius2() is faster to calculate than radius(). */
    inline value_type radius2() const
    {
        return 0.25*((_max-_min).length2());
    }
    
    /** Returns a specific corner of the bounding box.
     * pos specifies the corner as a number between 0 and 7.
     * Each bit selects an axis, X, Y, or Z from least- to
     * most-significant. Unset bits select the minimum value
     * for that axis, and set bits select the maximum. */
    inline const vec_type corner(unsigned int pos) const
    {
        return vec_type(pos&1?_max.x:_min.x,pos&2?_max.y:_min.y,pos&4?_max.z:_min.z);
    }
    
    /** Expands the bounding box to include the given coordinate.
     * If the box is uninitialized, set its min and max extents to v. */
    inline void expandBy(const vec_type& v)
    {
        if(v.x<_min.x) _min.x = v.x;
        if(v.x>_max.x) _max.x = v.x;
        
        if(v.y<_min.y) _min.y = v.y;
        if(v.y>_max.y) _max.y = v.y;
        
        if(v.z<_min.z) _min.z = v.z;
        if(v.z>_max.z) _max.z = v.z;
    }
    
    /** Expands the bounding box to include the given coordinate.
     * If the box is uninitialized, set its min and max extents to
     * Vec3(x,y,z). */
    inline void expandBy(value_type x,value_type y,value_type z)
    {
        if(x<_min.x) _min.x = x;
        if(x>_max.x) _max.x = x;
        
        if(y<_min.y) _min.y = y;
        if(y>_max.y) _max.y = y;
        
        if(z<_min.z) _min.z = z;
        if(z>_max.z) _max.z = z;
    }
    
    /** Expands this bounding box to include the given bounding box.
     * If this box is uninitialized, set it equal to bb. */
    void expandBy(const BoundingBoxImpl& bb)
    {
        if (!bb.valid()) return;
        
        if(bb._min.x<_min.x) _min.x = bb._min.x;
        if(bb._max.x>_max.x) _max.x = bb._max.x;
        
        if(bb._min.y<_min.y) _min.y = bb._min.y;
        if(bb._max.y>_max.y) _max.y = bb._max.y;
        
        if(bb._min.z<_min.z) _min.z = bb._min.z;
        if(bb._max.z>_max.z) _max.z = bb._max.z;
    }
    
    
    /** Returns the intersection of this bounding box and the specified bounding box. */
    BoundingBoxImpl intersect(const BoundingBoxImpl& bb) const
    {
        return BoundingBoxImpl(std::max(_min.x,bb._min.x),std::max(_min.y,bb._min.y),std::max(_min.z,bb._min.z),
                               std::min(_max.x,bb._max.x),std::min(_max.y,bb._max.y),std::min(_max.z,bb._max.z));
    }
    
    /** Return true if this bounding box intersects the specified bounding box. */
    bool intersects(const BoundingBoxImpl& bb) const
    {
        return std::max(_min.x,bb._min.x) <= std::min(_max.x,bb._max.x) &&
                std::max(_min.y,bb._min.y) <= std::min(_max.y,bb._max.y) &&
                std::max(_min.z,bb._min.z) <= std::min(_max.z,bb._max.z);
    }
    
    /** Returns true if this bounding box contains the specified coordinate. */
    inline bool contains(const vec_type& v) const
    {
        return valid() &&
        (v.x>=_min.x && v.x<=_max.x) &&
        (v.y>=_min.y && v.y<=_max.y) &&
        (v.z>=_min.z && v.z<=_max.z);
    }
};

typedef BoundingBoxImpl<Vec3> BoundingBox;

#endif /* SimBoundingBox_hpp */
