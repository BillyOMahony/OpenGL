#pragma once

class VertexBuffer
{

public:
	/**
	 *	Construct a Vertex Buffer
	 *  @param data An array containing the vertex positions used in this buffer
	 *  @param size The size in bytes of the data
	 */
	VertexBuffer(const void* data, unsigned int size);

	/**
	 * Instruct OpenGL to delete this buffer
	 */
	~VertexBuffer();

	/**
	 *	Bind this buffer, allowing it to be drawn
	 */
	void Bind() const;

	/**
	 *	Unbind this buffer so it can no longer be drawn
	 */
	void UnBind() const;

protected:

private:
	/**
	 *	The ID for this buffer 
	 */
	unsigned int m_RendererID;

};