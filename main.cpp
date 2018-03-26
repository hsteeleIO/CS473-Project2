#define STB_IMAGE_IMPLEMENTATION

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stb_image.h>
#include <string> // for string class

#include "Shader.h"
#include "Cube.h"
#include "Cat.h"
#include "Mouse.h"
#include "House.h"
#include "Camera.h"
#include "oGL_Tools.h"
#include "ImportOBJ.h"

#include <iostream>

void switchToMain(GLFWwindow* window);
void switchToTitle(GLFWwindow* window);
void mainLoopTitleScreen(GLFWwindow* window);
void mainLoopInteractive(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mousePosCallback(GLFWwindow* window, double mX, double mY);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void mouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
void processInput(GLFWwindow* window);

GLFWwindow* setupOpenGL(int width, int height, std::string name);

// Global Variables
Shader stdProgram = Shader("vShadTex.glsl", "fShadTex.glsl");
Shader textShader = Shader("vShadText.glsl", "fShadText.glsl");
Shader lightShader = Shader("vShadLight.glsl", "fShadLight.glsl");
Shader stdProgramNorm = Shader("vShadNorm.glsl", "fShadNorm.glsl");

ImportOBJ importer;
Cat cat1;
Mouse mouse;
House house;
Camera cam = Camera();

double deltaTime = 0.0;
double lastFrame = 0.0;
double camSpeed = 3.0;

float mouseX = 400;
float mouseY = 300;
float mouseSensitivity = 0.05;

int titleVAO;
int titleTex;

int displayMode = 1;
// 1 = title, 0 = interactive


int main() {

    GLFWwindow* window = setupOpenGL(800, 800, "CS473 ICE15 - Model Loader");
    if (window == NULL) return -1;

    glfwSetWindowPos(window, 450, 50);
    glViewport(0, 0, 800, 800);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mousePosCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetScrollCallback(window, mouseScrollCallback);

    cat1.initialize(importer);
    mouse.initialize(importer);
    house.initialize(importer);
    stdProgram.initialize();
    stdProgramNorm.initialize();

    cam.setPos(glm::vec3(0.0, 0.0, 3.0));

    titleVAO = rectangleTex(0.0, 10.0, 10.0, 10.0);
    titleTex = loadTexture("textures/title.png", GL_RGBA);

    while (!glfwWindowShouldClose(window)) {
        if (displayMode == 1) mainLoopTitleScreen(window);
        else mainLoopInteractive(window);
    }

    glfwTerminate();
    return 0;
}

/** Switches display mode to Interactive and makes the necessary adjustments
  * such as setting the initial camera position and disabling the cursor */
void switchToMain(GLFWwindow* window) {
    lastFrame = glfwGetTime();
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    //glfwGetCursorPos(window, &mouseX, &mouseY);
    /*if (firstTime) {
        cam.setPos(glm::vec3(0.0, 0.0, 3.0));
        cam.setLookAt(glm::vec3(0.0, 0.0, 0.0));
        cam.setRollPitchYaw(glm::vec3(0.0, 0.0, 0.0));
        firstTime = false;
    }*/

    displayMode = 0;
}

/** Makes necessary changes to the OpenGL state to revert to the title screen */
void switchToTitle(GLFWwindow* window) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glDisable(GL_DEPTH_TEST);
    stdProgram.use();

    displayMode = 1;
}

void mainLoopTitleScreen(GLFWwindow* window) {
    processInput(window);
    //glClearColor(1.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    stdProgramNorm.use();
    stdProgramNorm.setDrawColor(WHITE);
    stdProgramNorm.setMatrix("view", glm::mat4(1.0f));
    stdProgramNorm.setMatrix("model", glm::mat4(1.0f));
    stdProgramNorm.setMatrix("projection", glm::ortho(.0, 10.0, .0, 10.0, -1.0, 1.0));

    stdProgramNorm.useOneTex();
    glBindTexture(GL_TEXTURE_2D, titleTex);
    glBindVertexArray(titleVAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void mainLoopInteractive(GLFWwindow* window) {
    glEnable(GL_DEPTH_TEST);
    double currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;
    lastFrame = currentFrame;

    processInput(window);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    stdProgram.use();
    stdProgram.setMatrix("view", glm::mat4(1.0f));
    stdProgram.setMatrix("projection", glm::perspective(glm::radians(45.0), 1.0, 0.1, 100.0));
    stdProgram.setMatrix("view", cam.genViewMatrix());
    stdProgram.setDrawColor(1.0, 0.0, 0.0, 1.0);
    stdProgram.noTextures();

    house.draw(stdProgram);
    mouse.draw(stdProgram);
    cat1.printPos();
    cat1.draw(stdProgram);
    glBindVertexArray(0);

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
    }

void mousePosCallback(GLFWwindow* window, double mX, double mY) {
    float xOffset = mX - mouseX;
    float yOffset = mY - mouseY;
    mouseX = mX;
    mouseY = mY;

    xOffset *= mouseSensitivity;
    yOffset *= mouseSensitivity;

    cam.changeRollPitchYawBy(glm::vec3(0.0, -yOffset, xOffset));
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    // Used the displayMousePos debug flag to determine what screen coordinates
    // correspond to the "Start" button on the texture.
    if (button == GLFW_MOUSE_BUTTON_1 && action == GLFW_RELEASE) {
        if (mouseX >= 283 && mouseX <= 520 && mouseY >= 630 && mouseY <= 697) {
            switchToMain(window);
        }
    }
    std::cout << "Mouse button: " << button << " with " << action << " and " << mods << " at pos " << mouseX << ", " << mouseY << "\n";
}

void mouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
    if (!cam.getFreeCam()) cam.goForward(camSpeed * deltaTime * yOffset * 50);
}

void processInput(GLFWwindow* window) {
    // Using a series of if statements instead of if/else if
    // allows multiple keypresses to be registered simultaneously
    // (e.g. rotate the turret and move the tank at the same time)
    // Deltas are very small since even short keypresses register many
    // times.
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_F1)) switchToTitle(window);
    if (glfwGetKey(window, GLFW_KEY_P)) switchToMain(window);

    if (glfwGetKey(window, GLFW_KEY_L)) {
        cam.lockOnTo(cat1.getPos(), 6.0);
        std::cout << "Lock on\n";
    }
    if (glfwGetKey(window, GLFW_KEY_C)) {
        cat1.changeLock(true);
        std::cout << "Cat Lock on\n";
    }
    if (glfwGetKey(window, GLFW_KEY_V)) {
        cat1.changeLock(false);
        std::cout << "Cat Lock off\n";
    }
    if (glfwGetKey(window, GLFW_KEY_U)) {
        cam.freeLook();
        std::cout << "Lock off\n";
    }
    if (cat1.getLock()){
        if (glfwGetKey(window, GLFW_KEY_W)) cat1.goForward(camSpeed * deltaTime * 1.5);
        if (glfwGetKey(window, GLFW_KEY_S)) cat1.goForward(-camSpeed * deltaTime * 1.5);
        if (glfwGetKey(window, GLFW_KEY_D)) cat1.strafeRight(camSpeed * deltaTime * 1.5);
        if (glfwGetKey(window, GLFW_KEY_A)) cat1.strafeRight(-camSpeed * deltaTime * 1.5);
        if (glfwGetKey(window, GLFW_KEY_UP)) mouse.goForward(camSpeed * deltaTime * 1.5);
        if (glfwGetKey(window, GLFW_KEY_DOWN)) mouse.goForward(-camSpeed * deltaTime * 1.5);
        if (glfwGetKey(window, GLFW_KEY_RIGHT)) mouse.strafeRight(camSpeed * deltaTime * 1.5);
        if (glfwGetKey(window, GLFW_KEY_LEFT)) mouse.strafeRight(-camSpeed * deltaTime * 1.5);
    }
    else {
        if (glfwGetKey(window, GLFW_KEY_W)) cam.goForward(camSpeed * deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S)) cam.goForward(-camSpeed * deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D)) cam.strafeRight(camSpeed * deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A)) cam.strafeRight(-camSpeed * deltaTime);
        if (glfwGetKey(window, GLFW_KEY_Q)) cam.goUp(camSpeed * deltaTime);
        if (glfwGetKey(window, GLFW_KEY_E)) cam.goUp(-camSpeed * deltaTime);
    }
}
