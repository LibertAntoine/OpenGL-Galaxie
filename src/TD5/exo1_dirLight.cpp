#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/glm.hpp>
#include <glimac/image.hpp>
#include <glimac/Sphere.hpp>
#include <glimac/FreeflyCamera.hpp>
#include <vector>

using namespace glimac;

struct EarthProgram {
	Program m_Program;

	GLint uMVPMatrix;
	GLint uMVMatrix;
	GLint uNormalMatrix;
	GLint uEarthTexture;
	GLint uCloudTexture;

	EarthProgram(const FilePath& applicationPath) :
		m_Program(loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl",
			applicationPath.dirPath() + "shaders/directionallight.fs.glsl")) {
		uMVPMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
		uMVMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
		uNormalMatrix = glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix");
	}
};

struct MoonProgram {
	Program m_Program;

	GLint uMVPMatrix;
	GLint uMVMatrix;
	GLint uNormalMatrix;
	GLint uTexture;

	MoonProgram(const FilePath& applicationPath) :
		m_Program(loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl",
			applicationPath.dirPath() + "shaders/directionallight.fs.glsl")) {
		uMVPMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
		uMVMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
		uNormalMatrix = glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix");
	}
};



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
	FilePath applicationPath(argv[0]);;
	Program program = loadProgram(applicationPath.dirPath() + "shaders/3D.vs.glsl",
		applicationPath.dirPath() + "shaders/directionallight.fs.glsl");
	program.use();

	GLint uMVPMatrix = glGetUniformLocation(program.getGLId(), "uMVPMatrix");
	GLint uMVMatrix = glGetUniformLocation(program.getGLId(), "uMVMatrix");
	GLint uNormalMatrix = glGetUniformLocation(program.getGLId(), "uNormalMatrix");
	GLint uKd = glGetUniformLocation(program.getGLId(), "uKd");
	GLint uKs = glGetUniformLocation(program.getGLId(), "uKs");
	GLint uShininess = glGetUniformLocation(program.getGLId(), "uShininess");
	GLint uLightDir_vs = glGetUniformLocation(program.getGLId(), "uLightDir_vs");
	GLint uLightIntensity = glGetUniformLocation(program.getGLId(), "uLightIntensity");

	GLint uTexture0 = glGetUniformLocation(program.getGLId(), "uTexture");
	GLint uTexture1 = glGetUniformLocation(program.getGLId(), "uTexture2");
	

	glUniform3f(uKd, 0.7, 0.7, 0.7);
	glUniform3f(uKs, 0.3, 0.3, 0.3);
	glUniform1f(uShininess, 1);
	


	std::unique_ptr<Image> earth;
	earth = loadImage(applicationPath.dirPath() + "textures/EarthMap.jpg");

	std::unique_ptr<Image> moon;
	moon = loadImage(applicationPath.dirPath() + "textures/MoonMap.jpg");

	std::unique_ptr<Image> cloud;
	cloud = loadImage(applicationPath.dirPath() + "textures/CloudMap.jpg");


	glEnable(GL_DEPTH_TEST);

    std::cout << "OpenGL Version : " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLEW Version : " << glewGetString(GLEW_VERSION) << std::endl;

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/

	GLuint textureEarth;
	glGenTextures(1, &textureEarth);

	glBindTexture(GL_TEXTURE_2D, textureEarth);


	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, earth->getWidth(), earth->getHeight(), 0, GL_RGBA, GL_FLOAT, earth->getPixels());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint textureMoon;
	glGenTextures(1, &textureMoon);

	glBindTexture(GL_TEXTURE_2D, textureMoon);


	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, moon->getWidth(), moon->getHeight(), 0, GL_RGBA, GL_FLOAT, moon->getPixels());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	GLuint textureCloud;
	glGenTextures(1, &textureCloud);

	glBindTexture(GL_TEXTURE_2D, textureCloud);


	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, cloud->getWidth(), cloud->getHeight(), 0, GL_RGBA, GL_FLOAT, cloud->getPixels());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);


	int nbLune = 26;
	std::vector<glm::vec3> orbitLune;
	for (int i = 0; i < nbLune;i++) {
		orbitLune.push_back(glm::vec3(glm::sphericalRand(glm::radians(360.))));
	}

	Sphere sphere(1, 32, 16); //rayon, nb segments lat, long.
	sphere.getVertexCount(); //Retourne nombre de sommet.
	sphere.getDataPointer(); //vec3 position, vec3 normal, vec2 texCoords.

	GLuint vbo;
	glGenBuffers(1, &vbo); // VBO
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // Bind of the VBO

	glBufferData(GL_ARRAY_BUFFER, sphere.getVertexCount() * (sizeof(ShapeVertex)), sphere.getDataPointer(), GL_STATIC_DRAW); // Add VBO data
	glBindBuffer(GL_ARRAY_BUFFER, 0); // Debind

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	const GLuint VERTEX_ATTR_POSITION = 0;
	glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
	const GLuint VERTEX_ATTR_NORMALE = 1;
	glEnableVertexAttribArray(VERTEX_ATTR_NORMALE);
	const GLuint VERTEX_ATTR_TEXTURE = 2;
	glEnableVertexAttribArray(VERTEX_ATTR_TEXTURE);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(VERTEX_ATTR_POSITION,
		3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid*)offsetof(ShapeVertex, position));

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glVertexAttribPointer(VERTEX_ATTR_NORMALE,
		3, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid*)offsetof(ShapeVertex, normal));


	glVertexAttribPointer(VERTEX_ATTR_TEXTURE,
		2, GL_FLOAT, GL_FALSE, sizeof(ShapeVertex), (const GLvoid*)offsetof(ShapeVertex, texCoords));
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
	
	FreeflyCamera FreeCam;



    // Application loop:
    bool done = false;
    while(!done) {
        // Event loop:
        SDL_Event e;
        while(windowManager.pollEvent(e)) {
            if(e.type == SDL_QUIT) {
                done = true; // Leave the loop after this iteration
            }
        
			if (e.key.keysym.sym == 'j') {
				FreeCam.rotateLeft(1.f);
			}
			
			if (e.key.keysym.sym == 'l') {
				FreeCam.rotateLeft(-1.f);
			}

			if (e.key.keysym.sym == 'i') {
				FreeCam.rotateUp(1.f);
			}

			if (e.key.keysym.sym == 'k') {
				FreeCam.rotateUp(-1.f);
			}

			if (e.key.keysym.sym == 'f') {
				FreeCam.moveLeft(1.f);
			}

			if (e.key.keysym.sym == 'h') {
				FreeCam.moveLeft(-1.f);
			}

			if (e.key.keysym.sym == 't') {
				FreeCam.moveFront(1.f);
			}

			if (e.key.keysym.sym == 'g') {
				FreeCam.moveFront(-1.f);
			}
		}

        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/
		
		
		float fov = glm::radians(70.f);
		float ratio = 800. / 600.;
		glm::mat4 ProjMatrix = glm::perspective(fov, ratio, 0.1f, 100.f);
		glm::mat4 MVMatrix = glm::rotate(glm::mat4(1.0f), windowManager.getTime(), glm::vec3(0, 1, 1));
		MVMatrix = FreeCam.getViewMatrix() * MVMatrix;
		glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::vec3 uLight(glm::sin(windowManager.getTime()), glm::cos(windowManager.getTime()), glm::cos(windowManager.getTime()));

		uLight = glm::vec3(FreeCam.getViewMatrix() * glm::vec4(uLight, 0));
		glUniform3f(uLightDir_vs, uLight.x, uLight.y, uLight.z);

		glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
		glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
		glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));
		glUniform3f(uLightIntensity, 0.7, 0.7, 0.7);

		glBindVertexArray(vao);

		glBindVertexArray(vao);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureEarth);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureCloud);
		
		glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);


		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureMoon);
		glUniform3f(uLightIntensity, 0.2, 0.2, 0.2);
		
		for (int i = 0; i < nbLune; i++) {
		glm::mat4 MVMatrixLune = glm::translate(glm::mat4(1), glm::vec3(2 * glm::sin(windowManager.getTime() + orbitLune[i].x), 2 * glm::cos(windowManager.getTime() + orbitLune[i].y), 1.5* glm::sin(windowManager.getTime() + orbitLune[i].z)));
		MVMatrixLune = glm::scale(MVMatrixLune, glm::vec3(0.2, 0.2, 0.2));
		MVMatrixLune = glm::rotate(MVMatrixLune, windowManager.getTime(),orbitLune[i]);
		MVMatrixLune = FreeCam.getViewMatrix() * MVMatrixLune;
		glm::mat4 NormalMatrixLune = glm::transpose(glm::inverse(MVMatrixLune));


		glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrixLune));
		glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrixLune));
		glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrixLune));


		glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
		}
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindVertexArray(0);

        // Update the display
        windowManager.swapBuffers();
    }

	//glDeleteTextures(1, &textureLogo);

    return EXIT_SUCCESS;
}
