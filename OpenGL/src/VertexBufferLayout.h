#pragma once
#include <vector>
#include <GL/glew.h>
#include "Renderer.h"

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch(type)
		{
			case GL_FLOAT: return sizeof(GLfloat);
				
			case GL_UNSIGNED_INT: return sizeof(GLuint);

			case GL_UNSIGNED_BYTE: return sizeof(GLbyte);
		}
		// We don't support the type
		ASSERT(false);
		return 0;
	}
};

/**
 * Describes the layout of the vertex buffer
 * I think this is the number of vertices per point. so this is 2D so two...
 * Might also be the number of Triangles but I don't think so
 * TODO find out which (if either) of the above is right
 */
class VertexBufferLayout
{
public:
	/**
	 *
	 */
	VertexBufferLayout()
		: m_Stride(0) {};

	template<typename T>
	void Push(unsigned int count)
	{
		static_assert(false);
	}

	/**
	 * Adds an element of type float to m_elements
	 * Increments m_Stride.
	 */
	template<>
	void Push<float>(unsigned int count)
	{
		m_Elements.push_back({ GL_FLOAT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
	}

	/**
	 * Adds an element of type unsigned int to m_elements
	 * Increments m_Stride.
	 */
	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
	}

	/**
	 * Adds an element of char float to m_elements
	 * Increments m_Stride.
	 */
	template<>
	void Push<unsigned char>(unsigned int count)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE, count, GL_TRUE });
		m_Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	}

	/**
	 * returns elements vector
	 */
	inline std::vector<VertexBufferElement> GetElements() const
	{
		return m_Elements;
	}

	/**
	 * returns the stride between elements in elements vector
	 */
	inline unsigned int GetStride() const
	{
		return m_Stride;
	}

private:
	std::vector<VertexBufferElement> m_Elements;

	// Amount of bytes from the beginning of one element to the start of the next element
	unsigned int m_Stride;
};