//
// Created by yangbin on 2017/4/15.
// Copyright (c) 2017 yangbin. All rights reserved.
//

#include "SimVertexIndexData.hpp"

#include "SimHardwareVertexBuffer.hpp"
#include "SimHardwareIndexBuffer.hpp"

VertexElement::VertexElement(const VertexElementSemantic semantic, const VertexElementType type, const size_t arraySize) :
    _semantic(semantic),
    _type(type),
    _arraySize(arraySize)
{
}

VertexElement::~VertexElement()
{
    LogManager::getSingleton().debug("Delete VertexElement", std::to_string((long)this) + " semantic: " + std::to_string(_semantic));
}

VertexElement* VertexElement::clone() const
{
    return new VertexElement(this->_semantic, this->_type, this->_arraySize);
}

size_t VertexElement::getVertexElementOffset(const VertexElementType type)
{
    size_t offset = 0;
    switch (type) {
        case VET_FLOAT3:
            offset = sizeof(float) * 3;
            break;
        case VET_FLOAT4:
            offset = sizeof(float) * 4;
            break;
        case VET_FLOAT2:
            offset = sizeof(float) * 2;
            break;
        case VET_FLOAT1:
            offset = sizeof(float) * 1;
    }
    return offset;
}

size_t VertexElement::getVertexElementComponentCount(const VertexElementType type)
{
    size_t offset = 0;
    switch (type)
    {
        case VET_FLOAT3:
            offset = 3;
            break;
        case VET_FLOAT4:
            offset = 4;
            break;
        case VET_FLOAT2:
            offset = 2;
            break;
        case VET_FLOAT1:
            offset = 1;
    }
    return offset;
}

////////////////////////////////////////////////

VertexDataDeclare::VertexDataDeclare(const VertexElementLayout layout) :
    _layout(layout),
    _stride(0),
    _eleNumber(0),
    _componentCount(0)
{
}

VertexDataDeclare::~VertexDataDeclare()
{
    LogManager::getSingleton().debug("Delete VertexDataDeclare");
    VertexElements::iterator iter = _eles.begin();
    VertexElements::const_iterator enditer = _eles.end();
    for (; iter != enditer; ++iter)
    {
        delete *iter;
    }
    _eles.clear();
    _eles.shrink_to_fit();
}

VertexDataDeclare* VertexDataDeclare::clone() const
{
    VertexDataDeclare* newVdd = new VertexDataDeclare(this->_layout);
    newVdd->_stride = this->_stride;
    newVdd->_eleNumber = this->_eleNumber;
    newVdd->_componentCount = this->_componentCount;
    // clone elements
    newVdd->_eles.resize(this->_eles.size());
    newVdd->_eles.reserve(this->_eles.size());
    for (int i=0; i<this->_eles.size(); ++i)
        newVdd->_eles[i] = this->_eles[i]->clone();
    return newVdd;
}

const VertexDataDeclare::VertexElements & VertexDataDeclare::getVertexElements() const
{
    return _eles;
}

void VertexDataDeclare::addElement(const VertexElementSemantic semantic, const VertexElementType type,  const size_t num)
{
    addElement(new VertexElement(semantic, type, num));
}

void VertexDataDeclare::addElement(VertexElement* ele)
{
    auto it = _eles.begin();
    while (it != _eles.end())
    {
        if ((*it)->getVertexElementSemantic() == ele->getVertexElementSemantic())
        {
            LogManager::getSingleton().debug("VertexDataDeclare::addElement", "A vertex semantic was declared twice!");
            return;
        }
        ++it;
    }
    _eles.push_back(ele);
    _eleNumber++;
    
    _stride += (VertexElement::getVertexElementOffset(ele->getVertexElementType()) * ele->getArraySize());
    _componentCount += (VertexElement::getVertexElementComponentCount(ele->getVertexElementType()) * ele->getArraySize());
}

const size_t VertexDataDeclare::getSemanticNumber(const VertexElementSemantic semantic)
{
    VertexElements::iterator it = _eles.begin();
    for (; it != _eles.end(); ++it)
    {
        if ((*it)->getVertexElementSemantic() == semantic)
        {
            return (*it)->getArraySize();
        }
    }
    return 0;
}

const size_t VertexDataDeclare::getElementOffset(const VertexElementSemantic semantic) const
{
    size_t offset = 0;
    
    for (int i=0; i<_eles.size(); ++i)
    {
        if (_eles[i]->getVertexElementSemantic() == semantic)
            break;
        offset += (VertexElement::getVertexElementOffset(_eles[i]->getVertexElementType()) * _eles[i]->getArraySize());
    }
    return offset;
}

const int VertexDataDeclare::getElementIndex(const VertexElementSemantic semantic) const
{
    for (int i=0; i<_eles.size(); ++i)
    {
        if (_eles[i]->getVertexElementSemantic() == semantic)
        {
            return i;
        }
    }
    return -1;
}

void VertexDataDeclare::sort()
{
    std::sort(_eles.begin(), _eles.end(), _funcVertexElementSemanticCompare);
}

////////////////////////////////////////////////

VertexData::VertexData(VertexDataDeclare* dataDeclaration) :
    _declaration(dataDeclaration),
    _verBuffer(nullptr),
    _isBinded(false)
{
}

VertexData::~VertexData()
{
    LogManager::getSingleton().debug("Delete VertexData", "vertex number: " + std::to_string(_numVertices));
    if (_declaration)
    {
        delete _declaration;
        _declaration = nullptr;
    }
    
    if (_verBuffer)
    {
        delete _verBuffer;
        _verBuffer = nullptr;
    }
}

HardwareVertexBuffer* VertexData::createBuffer(const size_t bufSize, const HardwareBuffer::Usage usage)
{
    return _verBuffer = new HardwareVertexBuffer(bufSize, usage);
}

void VertexData::bind(const GLuint vao, const bool hasInstanceData)
{
    if (vao == 0)
        LogManager::getSingleton().error("VertexData::bind", "The vertex array object can not be 0 !");
    
    glBindVertexArray(vao);
    
    // First, bind the buffer.
    glBindBuffer(GL_ARRAY_BUFFER, _verBuffer->getBufferId());
    
    if (_isBinded)
        return;
    
    if (!hasInstanceData)
    {
        const VertexDataDeclare::VertexElements& ves = _declaration->getVertexElements();
        if (_declaration->getVertexElementLayout() == VEL_ALIGN)
        {
            size_t offset = 0;
            for (unsigned int num=0; num<_declaration->getElementNumber(); ++num)
            {
                VertexElement* ve = ves[num];
                glVertexAttribPointer(num,
                                      (GLsizei) VertexElement::getVertexElementComponentCount(ve->getVertexElementType()),
                                      GL_FLOAT,
                                      GL_FALSE,
                                      0, GL_BUFFER_OFFSET(offset));
                
                glEnableVertexAttribArray(num);
                
                offset += ve->getArraySize() * VertexElement::getVertexElementOffset(ve->getVertexElementType());
            }
        }
        else
        {
            for (unsigned int num=0; num<_declaration->getElementNumber(); ++num)
            {
                VertexElement* ve = ves[num];
                size_t offset = 0;
                for (int j=num-1; j>=0; --j)
                {
                    offset += VertexElement::getVertexElementOffset(ves[j]->getVertexElementType());
                }
                
                glVertexAttribPointer(num,
                                      (GLsizei) VertexElement::getVertexElementComponentCount(ve->getVertexElementType()),
                                      GL_FLOAT,
                                      GL_FALSE,
                                      (GLsizei)_declaration->getStride(), GL_BUFFER_OFFSET(offset));
                
                glEnableVertexAttribArray(num);
            }
        }
    }
    else
    {
        const VertexDataDeclare::VertexElements& ves = _declaration->getVertexElements();
        size_t offset = 0;
        for (unsigned int num=0; num<_declaration->getElementNumber(); ++num)
        {
            VertexElement* ve = ves[num];
            glVertexAttribPointer(num,
                                  (GLsizei) VertexElement::getVertexElementComponentCount(ve->getVertexElementType()),
                                  GL_FLOAT,
                                  GL_FALSE,
                                  0, GL_BUFFER_OFFSET(offset));
            glEnableVertexAttribArray(num);
            
            offset += ve->getArraySize() * VertexElement::getVertexElementOffset(ve->getVertexElementType());
            
            // We must know which elements are the properties of instances.
            // Set update in every frame.
            if (ve->getVertexElementSemantic() >= VES_INSTANCE_POSITION)
            {
                glVertexAttribDivisor(num, 1);
            }

        }
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glBindVertexArray(0);
    
    _isBinded = true;
}

void VertexData::bind(const bool hasInstanceData)
{
    // First, bind the buffer.
    glBindBuffer(GL_ARRAY_BUFFER, _verBuffer->getBufferId());
    
    if (_isBinded)
        return;
    
    if (!hasInstanceData)
    {
        const VertexDataDeclare::VertexElements& ves = _declaration->getVertexElements();
        if (_declaration->getVertexElementLayout() == VEL_ALIGN)
        {
            size_t offset = 0;
            for (unsigned int num=0; num<_declaration->getElementNumber(); ++num)
            {
                VertexElement* ve = ves[num];
                glVertexAttribPointer(num,
                                      (GLsizei) VertexElement::getVertexElementComponentCount(ve->getVertexElementType()),
                                      GL_FLOAT,
                                      GL_FALSE,
                                      0, GL_BUFFER_OFFSET(offset));
                
                glEnableVertexAttribArray(num);
                
                offset += ve->getArraySize() * VertexElement::getVertexElementOffset(ve->getVertexElementType());
            }
        }
        else
        {
            for (unsigned int num=0; num<_declaration->getElementNumber(); ++num)
            {
                VertexElement* ve = ves[num];
                size_t offset = 0;
                for (int j=num-1; j>=0; --j)
                {
                    offset += VertexElement::getVertexElementOffset(ves[j]->getVertexElementType());
                }
                
                glVertexAttribPointer(num,
                                      (GLsizei) VertexElement::getVertexElementComponentCount(ve->getVertexElementType()),
                                      GL_FLOAT,
                                      GL_FALSE,
                                      (GLsizei)_declaration->getStride(), GL_BUFFER_OFFSET(offset));
                
                glEnableVertexAttribArray(num);
            }
        }
    }
    else
    {
        const VertexDataDeclare::VertexElements& ves = _declaration->getVertexElements();
        size_t offset = 0;
        for (unsigned int num=0; num<_declaration->getElementNumber(); ++num)
        {
            VertexElement* ve = ves[num];
            glVertexAttribPointer(num,
                                  (GLsizei) VertexElement::getVertexElementComponentCount(ve->getVertexElementType()),
                                  GL_FLOAT,
                                  GL_FALSE,
                                  0, GL_BUFFER_OFFSET(offset));
            glEnableVertexAttribArray(num);
            
            offset += ve->getArraySize() * VertexElement::getVertexElementOffset(ve->getVertexElementType());
            
            // We must know which elements are the properties of instances.
            // Set update in every frame.
            if (ve->getVertexElementSemantic() >= VES_INSTANCE_POSITION)
            {
                glVertexAttribDivisor(num, 1);
            }
            
        }
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    _isBinded = true;
}

void VertexData::bindAttr(const GLint location, const bool hasInstanceData)
{
    if (location < 0)
        LogManager::getSingleton().error("VertexData::bind", "The location of vertex attributes can not be negtive number !");
        
    // First, bind the buffer.
    glBindBuffer(GL_ARRAY_BUFFER, _verBuffer->getBufferId());
    
    if (_isBinded)
        return;
    
    const VertexDataDeclare::VertexElements& ves = _declaration->getVertexElements();
    
    glVertexAttribPointer(location,
                          (GLsizei) VertexElement::getVertexElementComponentCount(ves[0]->getVertexElementType()),
                          GL_FLOAT,
                          GL_FALSE,
                          0,
                          0);
    
    glEnableVertexAttribArray(location);
    
    // We must know which elements are the properties of instances.
    // Set update in every frame.
    if (ves[0]->getVertexElementSemantic() >= VES_INSTANCE_POSITION)
    {
        glVertexAttribDivisor(location, 1);
    }
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    _isBinded = true;
}

void VertexData::unbind()
{
    _isBinded = false;
}

///////////////////////////////////////////////

IndexData::IndexData() :
    _idxBuffer(nullptr)
{
}

IndexData::~IndexData()
{
    LogManager::getSingleton().debug("Delete VertexIndexData", "indices number: " + std::to_string(_idxBuffer->getNumIndices()));
    if (_idxBuffer)
    {
        delete _idxBuffer;
        _idxBuffer = nullptr;
    }
}

HardwareIndexBuffer* IndexData::createBuffer(HardwareIndexBuffer::IndexType itype, size_t numIndices, HardwareBuffer::Usage usage)
{
    _numIndices = numIndices;
    return _idxBuffer = new HardwareIndexBuffer(itype, numIndices, usage);
}
