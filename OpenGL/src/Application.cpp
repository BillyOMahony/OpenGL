#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

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
	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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

	// Triangle Vertex locations. 
	float TriangleVertices[6] = 
	{
		-1.f, -1.f,
		 0.f,  0.f, 
		 1.f, -1.f
	};

	// Create buffer
	unsigned int buffer;
	glGenBuffers(1, &buffer);
	// Select the buffer, as in we're about to work on this buffer
	// Bind to what you want to draw on, think of layers in photoshop
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	// Initialize buffer and give it data
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), TriangleVertices, GL_STATIC_DRAW);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}