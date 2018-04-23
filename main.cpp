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
#include "Item.h"
#include "Camera.h"
#include "oGL_Tools.h"
#include "ImportOBJ.h"
#include "Light.h"
#include "Font.h"

#include <iostream>
#include <cstdlib>

void switchToMain(GLFWwindow* window);
void switchToTitle(GLFWwindow* window);
void switchToInstructions(GLFWwindow* window);
void mainLoopTitleScreen(GLFWwindow* window);
void mainLoopInstructionScreen(GLFWwindow* window);
void mainLoopInteractive(GLFWwindow* window);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mousePosCallback(GLFWwindow* window, double mX, double mY);
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void mouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset);
void catCatchMouseCheck();
void reset();
void processInput(GLFWwindow* window);

void processSpecularLightInput(GLFWwindow* window);
void processDiffuseLightInput(GLFWwindow* window);
void processAmbientLightInput(GLFWwindow* window);
void processLightColorInput(GLFWwindow* window);
void processLightPosInput(GLFWwindow* window);
void processShininess(GLFWwindow* window);

GLFWwindow* setupOpenGL(int width, int height, std::string name);

// Global Variables
Shader stdProgram = Shader("vShadTex.glsl", "fShadTex.glsl");
Shader textShader = Shader("vShadText.glsl", "fShadText.glsl");
Shader lightShader = Shader("vShadLight.glsl", "fShadLight.glsl");
Shader stdProgramNorm = Shader("vShadNorm.glsl", "fShadNorm.glsl");
Shader fontShader = Shader("vShadTex.glsl", "fShaderT.glsl");

Font testFont = Font("fonts/ArialBlackLarge.bmp", "fonts/ArialBlack.csv");
Camera cam = Camera();
Light lightSrc = Light();
ImportOBJ importer;
Cat cat1;
Mouse mouse;
House house;
Item couch;
Item chair;
Item table;


double deltaTime = 0.0;
double lastFrame = 0.0;
double camSpeed = 3.0;

float mouseX = 400;
float mouseY = 300;
float mouseSensitivity = 0.05;

bool titleScreen = true;
int titleVAO;
int titleTex;
int instructionVAO;
int instructionTex;
// for displayed numbers
int textVAO;
int mouseWin = 0;
int catWin = 0;

int displayMode = 1;
// 2 = instructions, 1 = title, 0 = interactive


int main() {

    GLFWwindow* window = setupOpenGL(800, 800, "Cat Simulator    (=🝦 ﻌ 🝦=)");
    if (window == NULL) return -1;

    glfwSetWindowPos(window, 450, 50);
    glViewport(0, 0, 800, 800);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mousePosCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetScrollCallback(window, mouseScrollCallback);


    lightSrc.initialize(importer);
    cat1.initialize(importer);
    mouse.initialize(importer);
    house.initialize(importer);
    couch.initialize(importer, "couch");
    couch.setPos(glm::vec3(-1.0f, -1.0f, 18.0f));
    couch.setHasTexture(true);
    chair.initialize(importer, "chair");
    chair.setPos(glm::vec3(-12.0f, -2.0f, 20.0f));
    chair.setRotation(-90.0);
    table.initialize(importer, "table");
    table.setPos(glm::vec3(-19.0f, -2.0f, 6.0f));
    table.setRotation(-180.0);
    stdProgram.initialize();
    stdProgramNorm.initialize();
    lightShader.initialize();
    fontShader.initialize();

    testFont.initialize();


    cam.setPos(glm::vec3(0.0, 0.0, 3.0));

    titleVAO = rectangleTex(0.0, 10.0, 10.0, 10.0);
    titleTex = loadTexture("textures/title.png", GL_RGBA);
    instructionVAO = rectangleTex(0.0, 10.0, 10.0, 10.0);
    instructionTex = loadTexture("textures/instructions.png", GL_RGBA);

    while (!glfwWindowShouldClose(window)) {
        if (displayMode == 1) mainLoopTitleScreen(window);
        else if (displayMode == 2) mainLoopInstructionScreen(window);
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
    displayMode = 0;
}

/** Makes necessary changes to the OpenGL state to revert to the title screen */
void switchToTitle(GLFWwindow* window) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glDisable(GL_DEPTH_TEST);
    stdProgram.use();

    displayMode = 1;
}

void switchToInstructions(GLFWwindow* window) {
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glDisable(GL_DEPTH_TEST);
    stdProgram.use();

    displayMode = 2;
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

void mainLoopInstructionScreen(GLFWwindow* window) {
    processInput(window);
    glClear(GL_COLOR_BUFFER_BIT);

    stdProgramNorm.use();
    stdProgramNorm.setDrawColor(WHITE);
    stdProgramNorm.setMatrix("view", glm::mat4(1.0f));
    stdProgramNorm.setMatrix("model", glm::mat4(1.0f));
    stdProgramNorm.setMatrix("projection", glm::ortho(.0, 10.0, .0, 10.0, -1.0, 1.0));

    stdProgramNorm.useOneTex();
    glBindTexture(GL_TEXTURE_2D, instructionTex);
    glBindVertexArray(instructionVAO);
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


    lightShader.use();
    lightShader.setMatrix("view", cam.genViewMatrix());
    lightShader.setMatrix("projection", glm::perspective(glm::radians(45.0), 1.0, 0.1, 100.0));
    lightSrc.setUniforms(lightShader, cam);

    if (!mouse.hasEscaped()) mouse.draw(lightShader);
    cat1.draw(lightShader);
    couch.draw(lightShader);
    chair.draw(lightShader);
    table.draw(lightShader);
    house.draw(lightShader);

    stdProgram.use();
    stdProgram.setMatrix("view", glm::mat4(1.0f));
    stdProgram.setMatrix("projection", glm::perspective(glm::radians(45.0), 1.0, 0.1, 100.0));
    stdProgram.setMatrix("view", cam.genViewMatrix());
    stdProgram.setDrawColor(1.0, 0.0, 0.0, 1.0);
    stdProgram.noTextures();

    lightSrc.draw(stdProgram);

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
        if (mouseX >= 475 && mouseX <= 636 && mouseY >= 630 && mouseY <= 700) {
            switchToMain(window);
        }
        else if (mouseX >= 103 && mouseX <= 451 && mouseY >= 630 && mouseY <= 700) {
            if (titleScreen) switchToInstructions(window);
            else switchToTitle(window);
            titleScreen = !titleScreen;
        }
    }
    //std::cout << "Mouse button: " << button << " with " << action << " and " << mods << " at pos " << mouseX << ", " << mouseY << "\r";
}

void mouseScrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
    if (!cam.getFreeCam()) cam.goForward(camSpeed * deltaTime * yOffset * 50);
}

void catCatchMouseCheck(){
    glm::vec3 mP = mouse.getPos();
    glm::vec3 cP = cat1.getPos();
    float diff = std::abs(mP.x-cP.x)+std::abs(mP.y-cP.y)+std::abs(mP.z-cP.z);
    if (diff < 2) mouse.setCheckEscape(true);
    //std::cout << "diff: " << diff << "\r";
}

void reset(){
    mouse.reset();
    cat1.reset();
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
        cam.setPos(glm::vec3(-17.7, 20.7, 34.5));
        if (glfwGetKey(window, GLFW_KEY_W)) cat1.goForward(camSpeed * deltaTime * 4.0);
        if (glfwGetKey(window, GLFW_KEY_S)) cat1.goForward(-camSpeed * deltaTime * 4.0);
        if (glfwGetKey(window, GLFW_KEY_D)) cat1.rotateCatBy(-camSpeed * deltaTime * 60.0);
        if (glfwGetKey(window, GLFW_KEY_A)) cat1.rotateCatBy(camSpeed * deltaTime * 60.0);
        if (glfwGetKey(window, GLFW_KEY_UP)) mouse.goForward(camSpeed * deltaTime * 5.5);
        if (glfwGetKey(window, GLFW_KEY_DOWN)) mouse.goForward(-camSpeed * deltaTime * 5.5);
        if (glfwGetKey(window, GLFW_KEY_RIGHT)) mouse.rotateMouseBy(-camSpeed * deltaTime * 60.5);
        if (glfwGetKey(window, GLFW_KEY_LEFT)) mouse.rotateMouseBy(camSpeed * deltaTime * 60.5);
        if (glfwGetKey(window, GLFW_KEY_R)) reset();
        cam.setLookAt(glm::vec3(0.0, 0.0, 0.0));
        mouse.checkEscape();
        catCatchMouseCheck();
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
