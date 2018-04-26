#include "Cat.h"

#include "oGL_Tools.h"

Cat::Cat()
{
    this->pos = glm::vec3(0.0f, 0.0f, 0.0f);
    this->rotateAngle = -90.0;
    this->rotAxis = glm::vec3(0.0, 1.0, 0.0);
    this->scale = 1;
    this->lock = true;
}

void Cat::initialize(ImportOBJ importer) {
    this->VAO = importer.loadFiles("models/cat");
    this->numCombined = importer.getNumCombined();
}

void Cat::draw(Shader shadProgram) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, this->pos);
    model = glm::rotate(model, glm::radians(this->rotateAngle), this->rotAxis);
    model = glm::scale(model, glm::vec3(this->scale, this->scale, this->scale));
    shadProgram.setMatrix("model", model);
    // for lighting
    shadProgram.setBool("mat.useDiffuseMap", false);
    shadProgram.setBool("mat.useSpecularMap", false);

    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, this->numCombined);
}

void Cat::changeLock(bool val){
        this->lock = val;
    }

void Cat::moveCatBy(float X, float Z) {
    float newX = this->pos.x + X;
    float newZ = this->pos.z + Z;
    // Collision detection for the walls
    if (newX < -16) newX = -16;
    if (newX > 16) newX = 16;
    if (newZ < -16) newZ = -16;
    if (newZ > 14) newZ = 14;
    this->setPos(glm::vec3(newX, this->pos.y, newZ));
}

void Cat::goForward(float amt) {
    float newX = amt * -cos(glm::radians(this->rotateAngle));
    float newZ = amt * sin(glm::radians(this->rotateAngle));
    if (this->lock) moveCatBy(newX, newZ);
}

void Cat::rotateCatBy(float amt) {this->rotateAngle = normDegrees(this->rotateAngle + amt);}

void Cat::setPos(glm::vec3 newPos) {this->pos = newPos;}
void Cat::setRotation(float newAngle) {this->rotateAngle = newAngle;}
void Cat::setScale(float newScale) {this->scale = newScale;}
void Cat::reset() {
    this->pos = glm::vec3(0.0f, 0.0f, 0.0f);
    this->rotateAngle = -90.0;

}

bool Cat::getLock() {return this->lock;}
glm::vec3 Cat::getPos() {return this->pos;}
unsigned int Cat::getVAO() {return this->VAO;}
unsigned int Cat::getnumCombined() {return this->numCombined;}

void Cat::printPos(){
    std::cout << "Cat Pos: (" << pos.x << ", " << pos.y << ", " << pos.z << ")  \r";
}
