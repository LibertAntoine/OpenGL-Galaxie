#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/glm.hpp>
#include <vector>

using namespace glimac;

typedef struct Vertex2DColor {
	glm::vec2 position;
	glm::vec3 color;
	Vertex2DColor() {}
	Vertex2DColor(glm::vec2 position, glm::vec3 color) {
		this->position = position;
		this->color = color;
	}
} Vertex2DColor;

int main(int argc, char** argv) {
    // Initialize SDL and open a window
    SDLWindowManager windowManager(800, 600, "GLImac");

    // Initialize glew for OpenGL3+ support
    GLenum glewInitError = glewInit();
    if(GLEW_OK != glewInitError) {
        std::cerr << glewGetErrorString(glewInitError) << std::endl;
        return EXIT_FAILURE;
    }

	// Shaders compilation
	FilePath applicationPath(argv[0]);
	Program program = loadProgram(applicationPath.dirPath() + "shaders/mandelbrot2D.vs.glsl",
		applicationPath.dirPath() + "shaders/mandelbrot2D.fs.glsl");
	program.use();

    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

	GLuint vbo;
	glGenBuffers(1, &vbo); // VBO
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // Bind of the VBO


	Vertex2DColor vertices[] = {
		Vertex2DColor(glm::vec2(-1., -1.), glm::vec3(1, 1, 1)),
		Vertex2DColor(glm::vec2(1., -1.), glm::vec3(1, 1, 1)),
		Vertex2DColor(glm::vec2(-1.,  1.), glm::vec3(1, 1, 1)),
		Vertex2DColor(glm::vec2(1.,  1.), glm::vec3(1, 1, 1))
	}; // Float Table
	glBufferData(GL_ARRAY_BUFFER, 4* (sizeof(Vertex2DColor)), vertices, GL_STATIC_DRAW); // Add VBO data
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Debind


	GLuint ibo; // index buffer
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);


	std::vector<uint32_t> indices;
		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(3);
		indices.push_back(2);
		indices.push_back(1);

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	const GLuint VERTEX_ATTR_POSITION = 0;
	glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
	const GLuint VERTEX_ATTR_COLOR = 1;
	glEnableVertexAttribArray(VERTEX_ATTR_COLOR);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(VERTEX_ATTR_POSITION,
		2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor), (const GLvoid*)offsetof(Vertex2DColor, position));


	glVertexAttribPointer(VERTEX_ATTR_COLOR,
		3, GL_FLOAT, GL_FALSE, sizeof(Vertex2DColor), (const GLvoid*)offsetof(Vertex2DColor, color));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);


    // Application loop:
    bool done = false;
    while(!done) {
        // Event loop:
        SDL_Event e;
        while(windowManager.pollEvent(e)) {
            if(e.type == SDL_QUIT) {
                done = true; // Leave the loop after this iteration
            }
        }

        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/
		glClear(GL_COLOR_BUFFER_BIT);
		glBindVertexArray(vao);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

        // Update the display
        windowManager.swapBuffers();
    }

    return EXIT_SUCCESS;
}
