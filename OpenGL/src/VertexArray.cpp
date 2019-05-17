#include "VertexArray.h"
#include "Renderer.h"
#include "VertexBufferLayout.h"
#include <iostream>

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_RendererID)); // Generate one vertex array and store id in m_RendererID variable
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer & vb, const VertexBufferLayout & layout)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for(unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		
		// Tell OpenGL The layout of our vertex buffer.
		// Enable drawing of Vertex
		GLCall(glEnableVertexAttribArray(i));
		// Size = num points per vertex (two in this case).
		// Stride = length in bytes from the start of one vertex to start of the next.
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*) offset));

		std::cout << "Elements Count " << element.count;

		// increment offset, multiply increment amount by size in bytes of type
		// The offset says where the first element starts
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::UnBind() const
{
	GLCall(glBindVertexArray(0));
}
