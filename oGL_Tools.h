#ifndef OGL_TOOLS_H
#define OGL_TOOLS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"

#include <cmath>

const double PI = 3.14159265358979323846;

// Can put predefined colors here to save typing
const glm::vec4 BLACK = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
const glm::vec4 WHITE = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

int loadTexture(std::string name, GLenum format = GL_RGB);

/** Returns a VAO for grids at starting X/Y coordinates (lower left hand corner),
  * the spacing between grid lines, and number of grid lines in one dimension */
int grids(float startX, float startY, float spacing, int num, GLenum usage = GL_STATIC_DRAW);

/** Draws a gridVAO on the screen (though it could be used for others) with a particular
  * color and offset (translation). */
void drawGrids(int gridVAO, int numVerts, glm::vec4 color, glm::vec3 offset, Shader sProgram);

/** Returns a VAO for a rectangle with given upper left hand coordinates and size */
int rectangle(float ulX, float ulY, float sizeX, float sizeY, GLenum usage = GL_STATIC_DRAW);
int rectangleTex(float ulX, float ulY, float sizeX, float sizeY, GLenum usage = GL_STATIC_DRAW);
int rectangleTex2(float ulX, float ulY, float sizeX, float sizeY, GLenum usage = GL_STATIC_DRAW);

/** Returns a VAO for a circle with given specifications and number of vertexes */
int circle(float centX, float centY, float rad, int numVert, GLenum usage = GL_STATIC_DRAW);
int circleTex(float centX, float centY, float rad, int numVert, GLenum usage = GL_STATIC_DRAW);

/** Returns the parameter normalized to [0-360.0] */
float normDegrees(float degrees);
float capAmount(float amt, float minVal, float maxVal);

/** Returns a VAO given vertex data and usage, EBO index data and size index optional */
int setupVAO(const void* data, GLsizeiptr sizedata, GLenum usage, const void* indexdata = NULL, GLsizeiptr sizeindex = 0);
int setupVAOTex(const void* data, GLsizeiptr sizedata, GLenum usage, const void* indexdata = NULL, GLsizeiptr sizeindex = 0);

/** setupOpenGL will initialize GLFW to use OpenGL v3.3 in core profile mode
  * It will also initialize GLAD and return the new window
  * Will print an error message and return NULL if GLFW or GLAD fails.
  */
GLFWwindow* setupOpenGL(int width, int height, std::string name);

#endif // OGL_TOOLS_H
