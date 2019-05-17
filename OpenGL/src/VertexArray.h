#pragma once

#include "VertexBuffer.h";

class VertexBufferLayout;

class VertexArray
{
public:
	/**
	 *	Generates this vertex array
	 */
	VertexArray();

	/**
	 *	Deletes the associated vertex array
	 */
	~VertexArray();

	/**
	 *	Adds a VertexBuffer and VertexBufferLayout and binds them to this VertexArray
	 */
	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	/**
	 * Binds this VertexArray
	 */
	void Bind() const;

	/**
	 * Unbinds this vertex array
	 */
	void UnBind() const;

protected:

private:
	unsigned int m_RendererID;

};
