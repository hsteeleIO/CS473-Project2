#include "Cat.h"

#include "oGL_Tools.h"

Cat::Cat()
{
    this->pos = glm::vec3(0.0f, 0.0f, 0.0f);
    this->lock = false;
    this->rotateAngle = 0.0;
    this->rotAxis = glm::vec3(0.0, 1.0, 0.0);
    this->scale = 1;
}

void Cat::draw(Shader shadProgram) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, this->pos);
    model = glm::rotate(model, glm::radians(this->rotateAngle), this->rotAxis);
    model = glm::scale(model, glm::vec3(this->scale, this->scale, this->scale));
    shadProgram.setMatrix("model", model);

    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, this->numCombined);
}

void Cat::initialize(ImportOBJ importer) {
    this->VAO = importer.loadFiles("models/cat");
    std::cout << importer.getNumCombined() << " <-- inside Cat \n";
    this->numCombined = importer.getNumCombined();
}

void Cat::changeLock(bool val){
        this->lock = val;
    }

void Cat::goForward(float amt) {
    float newX = this->pos.x - amt;
    if (newX < -14) newX = -14;
    if (newX > 12) newX = 12;
    if (this->lock) setPos(glm::vec3(newX, this->pos.y, this->pos.z));
}

void Cat::strafeRight(float amt) {
    float newZ = this->pos.z - amt;
    if (newZ > 14) newZ = 14;
    if (newZ < -14) newZ = -14;
    if (this->lock) setPos(glm::vec3(this->pos.x, this->pos.y, newZ));
}

void Cat::rotateCatBy(float amt) {this->rotateAngle = normDegrees(this->rotateAngle + amt);}

void Cat::setPos(glm::vec3 newPos) {this->pos = newPos;}
void Cat::setRotation(float newAngle) {this->rotateAngle = newAngle;}
void Cat::setScale(float newScale) {this->scale = newScale;}

glm::vec3 Cat::getPos() {return this->pos;}
unsigned int Cat::getVAO() {return this->VAO;}
unsigned int Cat::getnumCombined() {return this->numCombined;}
bool Cat::getLock() {return this->lock;}

void Cat::printPos(){
    //std::cout << "Cat Pos: (" << pos.x << ", " << pos.y << ", " << pos.z << ")  \r";
}
