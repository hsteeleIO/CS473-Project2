#include "Mouse.h"

#include "oGL_Tools.h"

Mouse::Mouse()
{
    this->pos = glm::vec3(-3.0f, -1.0f, 3.0f);
    this->scale = 1;
    this->Escape = false;
}

void Mouse::initialize(ImportOBJ importer) {
    this->VAO = importer.loadFiles("models/mouse");
    std::cout << importer.getNumCombined() << " <-- inside Mouse \n";
    this->numCombined = importer.getNumCombined();
}

void Mouse::draw(Shader shadProgram) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, this->pos);
    //model = glm::rotate(model, glm::radians(this->rotateAngle), this->rotAxis);
    model = glm::scale(model, glm::vec3(this->scale, this->scale, this->scale));
    shadProgram.setMatrix("model", model);
    printPos();

    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, this->numCombined);
}

void Mouse::goForward(float amt) {
    float newX = this->pos.x - amt;
    if (newX < -14) newX = -14;
    if (newX > 12) newX = 12;
    setPos(glm::vec3(newX, this->pos.y, this->pos.z));
}

void Mouse::strafeRight(float amt) {
    float newZ = this->pos.z - amt;
    if (newZ > 14) newZ = 14;
    if (newZ < -14) newZ = -14;
    setPos(glm::vec3(this->pos.x, this->pos.y, newZ));
}

void Mouse::setPos(glm::vec3 newPos) {this->pos = newPos;}
void Mouse::setScale(float newScale) {this->scale = newScale;}
void Mouse::setCheckEscape(bool check) {this->Escape = check;}
void Mouse::checkEscape() {
    if (this->pos.z <= -14 && this->pos.x > -2.5 && this->pos.x < -1 ) this->Escape = true;
}
void Mouse::reset() {
    this->pos = glm::vec3(-3.0f, -1.0f, 3.0f);
    setCheckEscape(false);
}

glm::vec3 Mouse::getPos() {return this->pos;}
bool Mouse::getCheckEscape() {return this->Escape;}
unsigned int Mouse::getVAO() {return this->VAO;}
unsigned int Mouse::getnumCombined() {return this->numCombined;}

void Mouse::printPos(){
    std::cout << "Mouse Pos: (" << pos.x << ", " << pos.y << ", " << pos.z << ")  \r";
}
