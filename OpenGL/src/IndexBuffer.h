#pragma once

class IndexBuffer
{

public:
	/**
	 *	Construct an Index Buffer
	 *  @param data An array containing information on how vertices form edges between each other
	 *  @param count The element count of the object
	 */
	IndexBuffer(const unsigned int* data, unsigned int count);

	/**
	 * Instruct OpenGL to delete this buffer
	 */
	~IndexBuffer();

	/**
	 *	Bind this buffer, allowing it to be drawn
	 */
	void Bind() const;

	/**
	 *	Unbind this buffer so it can no longer be drawn
	 */
	void UnBind() const;

	/**
	 * @return number of elements of this object
	 */
	inline unsigned int GetCount() const;

protected:

private:
	/**
	 *	The ID for this buffer 
	 */
	unsigned int m_RendererID;

	/**
	 * The number of indices this has
	 */
	unsigned int m_Count;

};