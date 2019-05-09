#include "IndexBuffer.h"

#include "Renderer.h"
#include <GL/glew.h>

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
	: m_Count(count)
{
	// This is just to be cautious, these should be equal but it's possible they won't be
	ASSERT(sizeof(unsigned int) == sizeof(GLuint));

	// Generate a number (1) of buffers and bind their ID
	GLCall(glGenBuffers(1, &m_RendererID));
	// Bind this buffer, allowing us to draw on it
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
	// Initialize the buffer and pass it data
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::UnBind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

inline unsigned int IndexBuffer::GetCount() const
{
	return m_Count;
}
