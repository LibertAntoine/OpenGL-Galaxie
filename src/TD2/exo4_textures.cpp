#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/glm.hpp>
#include <glimac/image.hpp>
#include <vector>

using namespace glimac;

typedef struct Vertex2DUV {
	glm::vec2 position;
	glm::vec2 texture;
	Vertex2DUV() {}
	Vertex2DUV(glm::vec2 position, glm::vec2 texture) {
		this->position = position;
		this->texture = texture;
	}
} Vertex2DColor;

glm::mat3 translate(float tx, float ty) {
	return glm::mat3(glm::vec3(1, 0, 0), glm::vec3(0, 1, 0), glm::vec3(tx, ty, 1));
}

glm::mat3 scale(float sx, float sy) {
	return glm::mat3(glm::vec3(sx, 0, 0), glm::vec3(0, sy, 0), glm::vec3(0, 0, 1));
}

glm::mat3 rotate(float a) {
	return glm::mat3(glm::vec3(cos(a), sin(a), 0), glm::vec3(-sin(a), cos(a), 0), glm::vec3(0, 0, 1));
}



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
	Program program = loadProgram(applicationPath.dirPath() + "shaders/logo2D.vs.glsl",
		applicationPath.dirPath() + "shaders/logo2D.fs.glsl");
	program.use();

	GLint samplerId = glGetUniformLocation(program.getGLId(), "uTexture");

	std::unique_ptr<Image> logo;
	logo = loadImage(applicationPath.dirPath() + "textures/triforce.png");
	


    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/
	
	GLuint textureLogo;
	glGenTextures(1, &textureLogo);
	
	glBindTexture(GL_TEXTURE_2D, textureLogo);


	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, logo->getWidth(),  logo->getHeight(), 0, GL_RGBA, GL_FLOAT, logo->getPixels());
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint vbo;
	glGenBuffers(1, &vbo); // VBO
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // Bind of the VBO


	Vertex2DUV vertices[] = {
		Vertex2DUV(glm::vec2(-1, -1), glm::vec2(0, 1)),
		Vertex2DUV(glm::vec2(1, -1), glm::vec2(1, 1)),
		Vertex2DUV(glm::vec2(0., 1), glm::vec2(0.5, 0))
		}; // Float Table
	glBufferData(GL_ARRAY_BUFFER, 3* (sizeof(Vertex2DUV)), vertices, GL_STATIC_DRAW); // Add VBO data
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Debind

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	const GLuint VERTEX_ATTR_POSITION = 0;
	glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
	const GLuint VERTEX_ATTR_TEXTURE = 1;
	glEnableVertexAttribArray(VERTEX_ATTR_TEXTURE);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(VERTEX_ATTR_POSITION,
		2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DUV), (const GLvoid*)offsetof(Vertex2DUV, position));


	glVertexAttribPointer(VERTEX_ATTR_TEXTURE,
		2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DUV), (const GLvoid*)offsetof(Vertex2DUV, texture));
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
		glBindTexture(GL_TEXTURE_2D, textureLogo);
		glUniform1i(samplerId, 0);

		glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);

        // Update the display
        windowManager.swapBuffers();
    }

	glDeleteTextures(1, &textureLogo);

    return EXIT_SUCCESS;
}
