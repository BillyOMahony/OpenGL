#include "VertexBuffer.h"

#include "Renderer.h"
#include <GL/glew.h>

VertexBuffer::VertexBuffer(const void * data, unsigned int size)
{
	// Generate a number (1) of buffers and bind their ID
	GLCall(glGenBuffers(1, &m_RendererID));
	// Bind this buffer, allowing us to draw on it
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	// Initialize the buffer and pass it data
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::UnBind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
