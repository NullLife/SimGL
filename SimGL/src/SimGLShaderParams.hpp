//
//  SimGLShaderParams.hpp
//  SimGL
//
//  Created by yangbin on 2017/7/11.
//  Copyright © 2017年 yangbin. All rights reserved.
//

#ifndef SimGLShaderParams_hpp
#define SimGLShaderParams_hpp

#include "SimCommon.hpp"

class GLProgram;
class ParameterDataSource;

enum ShaderConstantType
{
    SCT_FLOAT1 = 1,
    SCT_FLOAT2 = 2,
    SCT_FLOAT3 = 3,
    SCT_FLOAT4 = 4,
    
    SCT_SAMPLER1D = 5,
    SCT_SAMPLER2D = 6,
    SCT_SAMPLER3D = 7,
    SCT_SAMPLERCUBE = 8,
    SCT_SAMPLERRECT = 9,
    SCT_SAMPLER1DSHADOW = 10,
    SCT_SAMPLER2DSHADOW = 11,
    SCT_SAMPLER2DARRAY = 12,
    
    SCT_MATRIX_3X3 = 17,
    SCT_MATRIX_4X4 = 21,
    
    SCT_INT1 = 22,
    SCT_INT2 = 23,
    SCT_INT3 = 24,
    SCT_INT4 = 25,
    
    SCT_SUBROUTINE = 26,
    
    SCT_DOUBLE1 = 27,
    SCT_DOUBLE2 = 28,
    SCT_DOUBLE3 = 29,
    SCT_DOUBLE4 = 30,
    
    SCT_UINT1 = 40,
    SCT_UINT2 = 41,
    SCT_UINT3 = 42,
    SCT_UINT4 = 43,
    
    SCT_BOOL1 = 44,
    SCT_BOOL2 = 45,
    SCT_BOOL3 = 46,
    SCT_BOOL4 = 47,
    
    SCT_UNKNOWN = 99
};

enum ShaderConstantContent
{
    // Unknown content
    SCC_UNKNOWN = 0,
    
    // Matrix
    SCC_MODEL_MATRIX,
    SCC_VIEW_MATRIX,
    SCC_PROJECTIVE_MATRIX,
    SCC_MV_MATRIX,
    SCC_MVP_MATRIX,
    
    // Position
    SCC_POSITION_OBJECT_SPACE,
    SCC_POSITION_WORLD_SPACE,
    SCC_POSITION_VIEW_SPACE,
    SCC_POSITION_PROJECTIVE_SPACE,
    SCC_POSITION_LIGHT_SPACE,
    
    // Normal
    SCC_NORMAL_OBJECT_SPACE,
    SCC_NORMAL_WORLD_SPACE,
    SCC_NORMAL_VIEW_SPACE,
    SCC_NORMAL_TANGENT_SPACE,
    
    // Tangent
    SCC_TANGENT_OBJECT_SPACE,
    SCC_TANGENT_WORLD_SPACE,
    SCC_TANGENT_VIEW_SPACE,
    SCC_TANGENT_TANGENT_SPACE,
    
    // Binormal
    SCC_BINORMAL_OBJECT_SPACE,
    SCC_BINORMAL_WORLD_SPACE,
    SCC_BINORMAL_VIEW_SPACE,
    SCC_BINORMAL_TANGENT_SPACE,
    
    // Color
    SCC_COLOR_DIFFUSE,
    SCC_COLOR_SPECULAR,
    
    // Depth
    SCC_DEPTH_OBJECT_SPACE,
    SCC_DEPTH_WORLD_SPACE,
    SCC_DEPTH_VIEW_SPACE,
    SCC_DEPTH_PROJECTIVE_SPACE,
    
    // Texture coordinate
    SCC_TEXCOORDS0,
    SCC_TEXCOORDS1,
    SCC_TEXCOORDS2,
    
    // Sampler
    SCC_SAMPLER_2D,
    
    // Camera
    SCC_CAMERAPOSITION_OBJECT_SPACE,
    SCC_CAMERAPOSITION_WORLD_SPACE,
    SCC_CAMERAPOSITION_VIEW_SPACE,
    SCC_CAMERAPOSITION_TANGENT_SPACE,
    
    // Light
    SCC_LIGHT_COUNT,
    SCC_LIGHT_TYPE,
    
    SCC_LIGHTPOSITION_OBJECT_SPACE,
    SCC_LIGHTPOSITION_WORLD_SPACE,
    SCC_LIGHTPOSITION_VIEW_SPACE,
    SCC_LIGHTPOSITION_TANGENT_SPACE,
    
    SCC_LIGHTDIRECTION_OBJECT_SPACE,
    SCC_LIGHTDIRECTION_WORLD_SPACE,
    SCC_LIGHTDIRECTION_VIEW_SPACE,
    SCC_LIGHTDIRECTION_TANGENT_SPACE,
    
    SCC_LIGHT_DIFFUSE_COLOR,
    SCC_LIGHT_SPECULAR_COLOR,
    
    SCC_LIGHT_ATTENUATION,
    SCC_LIGHT_INTENSITY,
    
    SCC_SPOT_LIGHT_CUTOFF,
    
    // Ambient light color
    SCC_AMBIENT_LIGHT_COLOR,
    
    // Material
    SCC_SURFACE_AMBIENT_COLOR,
    SCC_SURFACE_DIFFUSE_COLOR,
    SCC_SURFACE_SPECULAR_COLOR,
    SCC_SURFACE_EMISSIVE_COLOR,
    SCC_SURFACE_SHININESS,
    
    SCC_TIME
};

/** Define constant for shader.
 */
struct ShaderConstantDefinition
{
    ShaderConstantType _type;      // Constant data type
    ShaderConstantContent _content;
    
    size_t _physicalIndex;         // index for cpu data buffer.
    size_t _elementComponentSize;  // eg: float4:  4,   matrix4: 16
    size_t _data;                  // If it is array, then it is the index.
    size_t _arraySize;
    
    ShaderConstantDefinition() :
            _type(SCT_UNKNOWN),
            _content(SCC_UNKNOWN),
            _physicalIndex(0),
            _elementComponentSize(0),
            _data(0),
            _arraySize(1)
    {}
    
    bool isFloat() const
    {
        return isFloat(_type);
    }
    
    static bool isFloat(ShaderConstantType c)
    {
        switch(c) {
            case SCT_INT1:
            case SCT_INT2:
            case SCT_INT3:
            case SCT_INT4:
            case SCT_SAMPLER1D:
            case SCT_SAMPLER2D:
            case SCT_SAMPLER2DARRAY:
            case SCT_SAMPLER3D:
            case SCT_SAMPLERCUBE:
            case SCT_SAMPLERRECT:
            case SCT_SAMPLER1DSHADOW:
            case SCT_SAMPLER2DSHADOW:
                return false;
            default:
                return true;
        };
        
    }
    
    bool isDouble() const
    {
        return isDouble(_type);
    }
    
    static bool isDouble(ShaderConstantType c)
    {
        switch(c)
        {
            case SCT_INT1:
            case SCT_INT2:
            case SCT_INT3:
            case SCT_INT4:
            case SCT_FLOAT1:
            case SCT_FLOAT2:
            case SCT_FLOAT3:
            case SCT_FLOAT4:
            case SCT_SAMPLER1D:
            case SCT_SAMPLER2D:
            case SCT_SAMPLER2DARRAY:
            case SCT_SAMPLER3D:
            case SCT_SAMPLERCUBE:
            case SCT_SAMPLER1DSHADOW:
            case SCT_SAMPLER2DSHADOW:
                return false;
            default:
                return true;
        };
        
    }
    
    bool isSampler() const
    {
        return isSampler(_type);
    }
    
    static bool isSampler(ShaderConstantType c)
    {
        switch(c) {
            case SCT_SAMPLER1D:
            case SCT_SAMPLER2D:
            case SCT_SAMPLER2DARRAY:
            case SCT_SAMPLER3D:
            case SCT_SAMPLERCUBE:
            case SCT_SAMPLERRECT:
            case SCT_SAMPLER1DSHADOW:
            case SCT_SAMPLER2DSHADOW:
                return true;
            default:
                return false;
        };
    }
    
    static size_t getElementComponentSize(ShaderConstantType ctype)
    {
        switch(ctype)
        {
            case SCT_FLOAT1:
            case SCT_DOUBLE1:
            case SCT_INT1:
            case SCT_SAMPLER1D:
            case SCT_SAMPLER2D:
            case SCT_SAMPLER2DARRAY:
            case SCT_SAMPLER3D:
            case SCT_SAMPLERCUBE:
            case SCT_SAMPLER1DSHADOW:
            case SCT_SAMPLER2DSHADOW:
                return 1;
            case SCT_FLOAT2:
            case SCT_INT2:
            case SCT_DOUBLE2:
                return 2;
            case SCT_FLOAT3:
            case SCT_INT3:
            case SCT_DOUBLE3:
                return 3;
            case SCT_FLOAT4:
            case SCT_INT4:
            case SCT_DOUBLE4:
                return 4;
            case SCT_MATRIX_3X3:
                return 9;
            case SCT_MATRIX_4X4:
                return 16;
            default:
                return 4;
        };
    }
};

typedef HashMap<String, ShaderConstantDefinition> ShaderConstantDefinitionMap;

struct ShaderNamedConstants
{
    /// Total size of the float buffer required
    size_t _floatBufferSize;
    /// Total size of the double buffer required
    size_t _doubleBufferSize;
    /// Total size of the int buffer required
    size_t _intBufferSize;
    /// Map of parameter names to GpuConstantDefinition
    ShaderConstantDefinitionMap _defMap;
    
    ShaderNamedConstants() : _floatBufferSize(0), _doubleBufferSize(0), _intBufferSize(0) {}
};

typedef SharedPtr<ShaderNamedConstants> ShaderNamedConstantsPtr;
typedef Vector<int> IntConstantList;
typedef Vector<float> FloatConstantList;
typedef Vector<double> DoubleConstantList;

// For render
typedef HashMap<String, GLint> ParamNameIndices;

class GLShaderParams
{
public:
    GLShaderParams();
    ~GLShaderParams();
    
private:
    ShaderNamedConstantsPtr _namedConstants;
    
    FloatConstantList _floatConstants;
    DoubleConstantList _doubleConstants;
    IntConstantList _intConstants;
    
public:
    // Get shader parameter list.
    const ShaderNamedConstantsPtr& getNamedConstants() { return _namedConstants; }
    
    // Add constant definition.
    void addConstantDefinition(const String& name, ShaderConstantType scType, ShaderConstantContent scc, size_t arraySize = 1);
    ShaderConstantDefinition* findConstantDefinition(const std::string& name, bool throwErrorIfNotFound = false);
    
    void _updateParameters(ParameterDataSource* dataSource);
    
    void setNamedConstant(const String& name, int val);
    void setNamedConstant(const String& name, float val);
    void setNamedConstant(const String& name, double val);
    void setNamedConstant(const String& name, Vec2 val);
    void setNamedConstant(const String& name, Vec3 val);
    void setNamedConstant(const String& name, Vec4 val);
    void setNamedConstant(const String& name, Mat4 val);
    
    void setNamedConstant(const String& name, const float *val, size_t count);
    void setNamedConstant(const String& name, const double *val, size_t count);
    void setNamedConstant(const String& name, const int *val, size_t count);
    
    void _writeRawData(size_t index, const int val);
    void _writeRawData(size_t index, const float val);
    void _writeRawData(size_t index, const double val);
    void _writeRawData(size_t index, const int* val, size_t count);
    void _writeRawData(size_t index, const float* val, size_t count);
    void _writeRawData(size_t index, const double* val, size_t count);
    
    float* getFloatPointer(size_t pos);
    
    int* getIntPointer(size_t pos);
    
    double* getDoublePointer(size_t pos);
};

#endif /* SimGLShaderParams_hpp */
