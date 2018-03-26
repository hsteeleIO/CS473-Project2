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
#include "House.h"
#include "Camera.h"
#include "oGL_Tools.h"
#include "ImportOBJ.h"

#include <iostream>

Shader stdProgram("vShadNorm.glsl", "fShadNorm.glsl");
ImportOBJ importer;
Cat cat1;
House house;
Camera cam = Camera();

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

double deltaTime = 0.0;
double lastFrame = 0.0;
double camSpeed = 3.0;

float mouseX = 400;
float mouseY = 300;
float mouseSensitivity = 0.05;

int titleVAO;
int titleTex;

int displayMode = 0;
// 1 = title, 0 = interactive


int main() {
    GLFWwindow* window = setupOpenGL(800, 800, "CS473 ICE15 - Model Loader");
    if (window == NULL) return -1;

    glfwSetWindowPos(window, 450, 50);
    glViewport(0, 0, 800, 800);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mousePosCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetScrollCallback(window, mouseScrollCallback);

    cat1.initialize(importer);
    house.initialize(importer);
    //stdProgram.initialize();


    //Shader stdProgram("vShadNorm.glsl", "fShadNorm.glsl");
    stdProgram.use();

    glEnable(GL_DEPTH_TEST);

    int gridVAO = grids(-10.0, -10.0, 0.5, 40, GL_STATIC_DRAW);

    stdProgram.setMatrix("view", glm::mat4(1.0f));

    stdProgram.setMatrix("projection", glm::perspective(glm::radians(45.0), 1.0, 0.1, 100.0));
    cam.setPos(glm::vec3(0.0, 0.0, 3.0));

    titleVAO = rectangleTex(0.0, 10.0, 10.0, 10.0);
    titleTex = loadTexture("textures/title.png", GL_RGBA);



    while (!glfwWindowShouldClose(window)) {
        double currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        stdProgram.setMatrix("view", cam.genViewMatrix());
        stdProgram.setDrawColor(1.0, 0.0, 0.0, 1.0);
        stdProgram.noTextures();

        house.draw(stdProgram);
        cat1.printPos();
        //cam.genViewMatrix();
        cat1.draw(stdProgram);
        //std::cout << "VAO: " << catVAO << " sz: " << sz << "ENDOFLINE\n\n";
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
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
    glClear(GL_COLOR_BUFFER_BIT);

    stdProgram.setDrawColor(WHITE);
    stdProgram.setMatrix("view", glm::mat4(1.0f));
    stdProgram.setMatrix("model", glm::mat4(1.0f));
    stdProgram.setMatrix("projection", glm::ortho(0.0, 10.0, 0.0, 10.0, -1.0, 1.0));

    stdProgram.useOneTex();
    glBindTexture(GL_TEXTURE_2D, titleTex);
    glBindVertexArray(titleVAO);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void mainLoopInteractive(GLFWwindow* window) {
    double currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        stdProgram.setMatrix("view", cam.genViewMatrix());
        stdProgram.setDrawColor(1.0, 0.0, 0.0, 1.0);
        stdProgram.noTextures();

        house.draw(stdProgram);
        cat1.printPos();
        //cam.genViewMatrix();
        cat1.draw(stdProgram);
        //std::cout << "VAO: " << catVAO << " sz: " << sz << "ENDOFLINE\n\n";
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
    std::cout << "Mouse button: " << button << " with " << action << " and " << mods << "\n";
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
    if (glfwGetKey(window, GLFW_KEY_1)) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (glfwGetKey(window, GLFW_KEY_2)) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    if (glfwGetKey(window, GLFW_KEY_F1)) switchToTitle(window);

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
