//
// Created by yangbin on 2017/4/15.
// Copyright (c) 2017 yangbin. All rights reserved.
//

#ifndef SIMGL_VERTEXINDEXDATA_HPP
#define SIMGL_VERTEXINDEXDATA_HPP

#include "SimCommon.hpp"
#include "SimHardwareBuffer.hpp"
#include "SimHardwareVertexBuffer.hpp"
#include "SimHardwareIndexBuffer.hpp"

/** Types of members in vertex.
 */
enum VertexElementSemantic
{
    VES_POSITION = 0,
    VES_NORMAL,
    VES_TANGENT,
    VES_BITANGENT,
    VES_TEXCOORD,
    VES_DIFFUSECOLOR,
    
    VES_VELOCITY,
    VES_LIFETIME
};

/** Data types for a member in vertex.
 */
enum VertexElementType
{
    VET_FLOAT1 = 1,
    VET_FLOAT2,
    VET_FLOAT3,
    VET_FLOAT4
};

/** A member object in vertex.
 */
class VertexElement
{
public:
    static size_t getVertexElementOffset(VertexElementType type);
    static size_t getVertexElementComponentCount(VertexElementType type);
public:
    VertexElement(const VertexElementSemantic semantic, const VertexElementType type);
    ~VertexElement();

private:
    VertexElementSemantic mSemantic;
    VertexElementType mType;

public:
    const VertexElementSemantic getVertexElementSemantic() const;
    VertexElementType getVertexElementType()const;
};

/** The declaration for a vertex including its members.
 */
class VertexDataDeclare
{
public:
    typedef Vector<VertexElement*> VertexElements;
public:
    VertexDataDeclare();
    ~VertexDataDeclare();

private:
    VertexElements mEles;
    size_t         mNumber;   // The number of vertex members.
    size_t         mStride;   // The size of the all elements. [byte]
    int            mComponentCount;   // The size of the elements according its type. eg: if a element's type is the Vec3,  then the unit count is 3.

public:
    const VertexElements & getVertexElements() const;
    void addElement(VertexElementSemantic semantic, VertexElementType type);
    void addElement(VertexElement* ele);
    const size_t& getNumber() const;

    const size_t& getStride() const;
    const int getComponentCount() const;
};

/** Vertex collection.
 */
class VertexData
{
public:
    VertexData(VertexDataDeclare* dataDeclaration);
    ~VertexData();

private:
    VertexDataDeclare* mDeclaration;    // Vertex declaration for including which members
    
    HardwareVertexBuffer* mVerBuffer;
    
    size_t _numVertices;
    
    bool _isBinded;

public:
    const VertexDataDeclare* getVertexDataDeclare() { return mDeclaration; }
    
    // Initilise gpu buffer.
    HardwareVertexBuffer* createBuffer(size_t vertexSize, size_t numVertices, HardwareBuffer::Usage usage);
    
    HardwareVertexBuffer* getBuffer() { return mVerBuffer; }
    
    const size_t getNumVertices() const { return _numVertices; }
        
    const bool isBinded() const { return _isBinded; }
    
    void bind();
};

/** Vertex index collection.
 */
class IndexData
{
public:
    IndexData();
    ~IndexData();

private:
    size_t _numIndices;
    HardwareIndexBuffer* mIdxBuffer;
    
public:
    // Initilise gpu buffer.
    HardwareIndexBuffer* createBuffer(HardwareIndexBuffer::IndexType itype, size_t numIndices, HardwareBuffer::Usage usage);
    
    HardwareIndexBuffer* getBuffer() { return mIdxBuffer; }
    
    
    
    const size_t getNumIndices() const { return _numIndices; }
};


#endif //SIMGL_VERTEXINDEXDATA_HPP
