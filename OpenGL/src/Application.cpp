#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
	{
		std::cout << "Could not initialise OpenGL" << std::endl;
		std::cin.get();
		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(500, 500, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* Glew Code, this code allows access to graphics card's OpenGL functions */
	if (glewInit() != GLEW_OK) {
		// GLEW did not start correctly
		std::cout << "Glew Not OK" << std::endl;
		std::cin.get();
		return -1;
	}

	std::cout << "OpenGL has found: " << glGetString(GL_VERSION) << std::endl;
	
	{
		// Triangle Vertex locations
		// Also now has texture coordinates
		// Each line represents vertexX, vertexY, textureX, textureY
		float shapeVertexPositions[] =
		{
		/*   vX ,   vY,   tX,   tY*/
		
			-0.7f, -0.5f, 0.0f, 0.0f, // bottom left
			 0.7f, -0.5f, 1.0f, 0.0f, // bottom right
			 0.7f,  0.5f, 1.0f, 1.0f, // top left
			-0.7f,  0.5f, 0.0f, 1.0f  // top right
		};

		// The index buffer tells us which points in the shape are joined to form edges
		unsigned int shapeIndexBuffer[] = {
			0, 1, 2, // TriangleOne
			2, 3, 0  // TriangleTwo
		};

		// Set up blending so PNG does not have hard edges
		// For the source, take the source alpha. When rendering over it take 1 - source alpha
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		VertexArray va;
		// 4 * 4 * sizeof(float) = num vertices * number of floats per vertex * size in bytes of a float
		VertexBuffer vb(shapeVertexPositions, 4 * 4 * sizeof(float));
		
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);

		IndexBuffer ib(shapeIndexBuffer, 6);

		Shader shader("res/shaders/Basic.shader");
		shader.Bind();
		shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

		Texture texture("res/textures/test.png");
		texture.Bind();
		shader.SetUniform1i("u_Texture", 0);

		// Unbind everything
		va.UnBind();
		vb.UnBind();
		ib.UnBind();
		shader.UnBind();

		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

		Renderer renderer;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();

			shader.Bind();
			shader.SetUniform4f("u_Color", 1.0f, 0.0f, 1.0f, 1.0f);
			
			renderer.Draw(va, ib, shader);

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}

	glfwTerminate();

	return 0;
}
