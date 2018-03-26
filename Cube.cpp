#include "Cube.h"

#include "oGL_Tools.h"

Cube::Cube()
{
    this->pos = glm::vec3(0.0f, 0.0f, 0.0f);
    this->rotAxis = glm::vec3(1.0f, 0.3f, 0.5f);
    this->rotateAngle = 0.0;
    this->scale = 1.0;
}

void Cube::draw(Shader shadProgram) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->tex1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, this->tex2);

    shadProgram.useTwoTex(0.2);
    shadProgram.setDrawColor(WHITE);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, this->pos);
    model = glm::rotate(model, glm::radians(this->rotateAngle), this->rotAxis);
    model = glm::scale(model, glm::vec3(this->scale, this->scale, this->scale));
    shadProgram.setMatrix("model", model);

    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Cube::initializeClone(unsigned int newVAO, unsigned int newTex1, unsigned int newTex2) {
    this->VAO = newVAO;
    this->tex1 = newTex1;
    this->tex2 = newTex2;
}

void Cube::initializeClone(Cube *otherCube) {
    this->VAO = otherCube->getVAO();
    this->tex1 = otherCube->getTex1();
    this->tex2 = otherCube->getTex2();
}

void Cube::initializeMaster() {
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    this->VAO = setupVAOTex(vertices, sizeof(vertices), GL_STATIC_DRAW);
    this->tex1 = loadTexture("textures/container.jpg", GL_RGB);
    this->tex2 = loadTexture("textures/awesomeface.png", GL_RGBA);
}

void Cube::setPos(glm::vec3 newPos) {this->pos = newPos;}
void Cube::setRotation(float newAngle) {this->rotateAngle = newAngle;}
void Cube::setScale(float newScale) {this->scale = newScale;}

glm::vec3 Cube::getPos() {return this->pos;}
unsigned int Cube::getTex1() {return this->tex1;}
unsigned int Cube::getTex2() {return this->tex2;}
unsigned int Cube::getVAO() {return this->VAO;}
