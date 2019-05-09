#include "Renderer.h"
#include <iostream>
#include <GL/glew.h>

void GLClearError()
{
	// While there are errors (glGetError will return error flags until there are no errors)
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char * function, const char * file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL_Error] (" << error << ")" << function << " " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}
