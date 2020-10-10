#include <glimac/SDLWindowManager.hpp>
#include <GL/glew.h>
#include <iostream>
#include <glimac/Program.hpp>
#include <glimac/FilePath.hpp>
#include <glimac/glm.hpp>
#include <glimac/image.hpp>
#include <glimac/Sphere.hpp>
#include <glimac/TrackballCamera.hpp>
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
			applicationPath.dirPath() + "shaders/multiTex3D.fs.glsl")) {
		uMVPMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
		uMVMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
		uNormalMatrix = glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix");
		uEarthTexture = glGetUniformLocation(m_Program.getGLId(), "uTexture");
		uCloudTexture = glGetUniformLocation(m_Program.getGLId(), "uTexture2");
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
			applicationPath.dirPath() + "shaders/tex3D.fs.glsl")) {
		uMVPMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVPMatrix");
		uMVMatrix = glGetUniformLocation(m_Program.getGLId(), "uMVMatrix");
		uNormalMatrix = glGetUniformLocation(m_Program.getGLId(), "uNormalMatrix");
		uTexture = glGetUniformLocation(m_Program.getGLId(), "uTexture");
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
	FilePath applicationPath(argv[0]);
	EarthProgram earthProgram(applicationPath);
	MoonProgram moonProgram(applicationPath);

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
	
	TrackballCamera TrackCam;


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
				TrackCam.rotateLeft(1.f);
			}
			
			if (e.key.keysym.sym == 'l') {
				TrackCam.rotateLeft(-1.f);
			}

			if (e.key.keysym.sym == 'i') {
				TrackCam.rotateUp(1.f);
			}

			if (e.key.keysym.sym == 'k') {
				TrackCam.rotateUp(-1.f);
			}
		}

        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/
		
		
		float fov = glm::radians(70.f);
		float ratio = 800. / 600.;
		glm::mat4 ProjMatrix = glm::perspective(fov, ratio, 0.1f, 100.f);
		glm::mat4 MVMatrix = glm::rotate(glm::mat4(1.0f), windowManager.getTime(), glm::vec3(0, 1, 1));
		glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
		MVMatrix = TrackCam.getViewMatrix() * MVMatrix;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		earthProgram.m_Program.use();

		glUniform1i(earthProgram.uEarthTexture, 0);
		glUniform1i(earthProgram.uCloudTexture, 1);
		
		glUniformMatrix4fv(earthProgram.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
		glUniformMatrix4fv(earthProgram.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));
		glUniformMatrix4fv(earthProgram.uMVMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureEarth);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textureCloud);

		glBindVertexArray(vao);
		
		glDrawArrays(GL_TRIANGLES, 0, sphere.getVertexCount());
		
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, 0);

		moonProgram.m_Program.use();
		
		glUniform1i(moonProgram.uTexture, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureMoon);
		
		glUniformMatrix4fv(moonProgram.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));

		for (int i = 0; i < nbLune; i++) {
		glm::mat4 MVMatrixLune = glm::translate(glm::mat4(1.0f), glm::vec3(2 * glm::sin(orbitLune[i].x), 2 * glm::cos(orbitLune[i].y), 1.5* glm::sin(orbitLune[i].z)));
		MVMatrixLune = glm::scale(MVMatrixLune, glm::vec3(0.2, 0.2, 0.2));
		MVMatrixLune = glm::rotate(MVMatrixLune, windowManager.getTime(), orbitLune[i]);
		MVMatrixLune = TrackCam.getViewMatrix() * MVMatrixLune;

		glUniformMatrix4fv(moonProgram.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrixLune));
		glUniformMatrix4fv(moonProgram.uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrixLune));

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
