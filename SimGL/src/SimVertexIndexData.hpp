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
    VES_POSITION  = 1,
    VES_NORMAL,
    VES_TANGENT,
    VES_TEXCOORD,
    
    VES_BITANGENT,
    VES_DIFFUSECOLOR,
    
    VES_INSTANCE_POSITION = 100,
    VES_INSTANCE_DIFFUSECOLOR,
    VES_INSTANCE_VELOCITY,
    VES_INSTANCE_LIFETIME,
    VES_INSTANCE_SIZE,
    VES_INSTANCE_BIRTH,
    VES_INSTANCE_LIFERATE,
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

enum VertexElementLayout
{
    VEL_CROSS = 1,
    VEL_ALIGN
};

/** A member object in vertex.
 */
class VertexElement
{
public:
    static size_t getVertexElementOffset(VertexElementType type);
    static size_t getVertexElementComponentCount(VertexElementType type);
public:
    /**
     * @para Num if vertice using cross layout, then the num will be 1, or the num is the element count.
     */
    VertexElement(const VertexElementSemantic semantic, const VertexElementType type, const size_t arraySize = 1);
    ~VertexElement();
    
    /// Deeply copy.
    VertexElement* clone() const;
    
    const size_t getArraySize() const { return _arraySize; }
    
private:
    VertexElementSemantic _semantic;
    VertexElementType _type;
    size_t _arraySize;

public:
    const VertexElementSemantic getVertexElementSemantic() const { return _semantic; }
    VertexElementType getVertexElementType()const { return _type; }
};

/** The declaration for a vertex including its members.
 */
class VertexDataDeclare
{
public:
    typedef Vector<VertexElement*> VertexElements;
    
    VertexDataDeclare(const VertexElementLayout layout = VEL_CROSS);
    ~VertexDataDeclare();
    
    /// Deeply copy.
    VertexDataDeclare* clone() const;
    
    /// Get vertex buffer layout.
    const VertexElementLayout getVertexElementLayout() const { return _layout; }
    
    const VertexElements& getVertexElements() const;
    
    /// Add element.
    void addElement(const VertexElementSemantic semantic, const VertexElementType type, const size_t num = 1);
    void addElement(VertexElement* ele);
    
    /// Get number for a componnent of a vertex.
    const size_t getSemanticNumber(const VertexElementSemantic semantic);

    /// Get element number for constructing a vertex.
    const size_t getElementNumber() const { return _eleNumber; }
    
    /// Get stride size for a vertex.
    const size_t getStride() const { return _stride; }
    
    /// Get components count for all elements.
    const size_t getComponentCount() const { return _componentCount; }
    
    /// Get elemenet offset according to its locatation in a vertex.
    const size_t getElementOffset(const VertexElementSemantic semantic) const;
    
    /// Get index of element in a vertex.
    const int getElementIndex(const VertexElementSemantic semantic) const;
    
    /// Sort by enum VertexElementSemantic.
    void sort();

private:
    VertexElementLayout _layout;
    VertexElements _eles;
    
    size_t _eleNumber;       // The number of vertex members.
    size_t _stride;          // The size of the all elements. [byte]
    size_t _componentCount;  // The size of the elements according its type. eg: if a element's type is the Vec3,  then the unit count is 3.
    
    static bool _funcVertexElementSemanticCompare(VertexElement* v0, VertexElement* v1)
    {
        // from small ti big.
        return v0->getVertexElementSemantic() < v1->getVertexElementSemantic();
    }
};

/** Vertex collection.
 */
class VertexData
{
public:
    VertexData(VertexDataDeclare* dataDeclaration);
    ~VertexData();
    
    const VertexDataDeclare* getVertexDataDeclare() { return _declaration; }
    
    /// Initilise GPU buffer.
    HardwareVertexBuffer* createBuffer(const size_t bufSize, const HardwareBuffer::Usage usage);
    
    HardwareVertexBuffer* getBuffer() { return _verBuffer; }
        
    const bool isBinded() const { return _isBinded; }
    
    /// For a vertex buffer including all attributes.
    void bind(const GLuint vao, const bool hasInstanceData = false);
    
    void bind(const bool hasInstanceData = false);
    
    /// For single attribute buffer.
    void bindAttr(const GLint location, const bool hasInstanceData = false);
    
    void unbind();
    
    void setNumberVertices(const size_t num) { _numVertices = num; }
    const size_t getNumberVertices() const { return _numVertices; }
    
private:
    VertexDataDeclare* _declaration;    // Vertex declaration for including which members
    
    size_t _numVertices;
    
    HardwareVertexBuffer* _verBuffer;
    
    bool _isBinded;
};

/** Vertex index collection.
 */
class IndexData
{
public:
    IndexData();
    ~IndexData();
    
    /// Initilise gpu buffer.
    HardwareIndexBuffer* createBuffer(HardwareIndexBuffer::IndexType itype, size_t numIndices, HardwareBuffer::Usage usage);
    
    HardwareIndexBuffer* getBuffer() { return _idxBuffer; }
    
    const size_t getNumberIndices() const { return _numIndices; }
    
private:
    size_t _numIndices;
    HardwareIndexBuffer* _idxBuffer;
};


#endif //SIMGL_VERTEXINDEXDATA_HPP
