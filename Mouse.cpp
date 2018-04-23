#include "Mouse.h"

#include "oGL_Tools.h"

Mouse::Mouse()
{
    this->pos = glm::vec3(-3.0f, -1.0f, 3.0f);
    this->rotateAngle = -90.0;
    this->rotAxis = glm::vec3(0.0, 1.0, 0.0);
    this->Escape = false;
    this->scale = 1;
}

void Mouse::initialize(ImportOBJ importer) {
    this->VAO = importer.loadFiles("models/mouse");
    this->numCombined = importer.getNumCombined();
}

void Mouse::draw(Shader shadProgram) {
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

void Mouse::goForward(float amt) {
    float newX = amt * -cos(glm::radians(this->rotateAngle));
    float newZ = amt * sin(glm::radians(this->rotateAngle));
    moveMouseBy(newX, newZ);
}

void Mouse::rotateMouseBy(float amt){this->rotateAngle = normDegrees(this->rotateAngle + amt);}

void Mouse::moveMouseBy(float X, float Z) {
    float newX = this->pos.x + X;
    float newZ = this->pos.z + Z;
    // Collision detection for the walls
    if (newX < -16) newX = -16;
    if (newX > 16) newX = 16;
    if (newZ < -16) newZ = -16;
    if (newZ > 14) newZ = 14;
    this->setPos(glm::vec3(newX, this->pos.y, newZ));
}

void Mouse::setPos(glm::vec3 newPos) {this->pos = newPos;}
void Mouse::setScale(float newScale) {this->scale = newScale;}
void Mouse::setCheckEscape(bool check) {this->Escape = check;}
void Mouse::checkEscape() {if (this->pos.z <= -14.5 && this->pos.x > -1.2 && this->pos.x < 2.5 ) this->Escape = true;}

void Mouse::reset() {
    this->pos = glm::vec3(-3.0f, -1.0f, 3.0f);
    this->rotateAngle = -90.0;
    setCheckEscape(false);
}

glm::vec3 Mouse::getPos() {return this->pos;}
bool Mouse::hasEscaped() {return this->Escape;}
unsigned int Mouse::getVAO() {return this->VAO;}
unsigned int Mouse::getnumCombined() {return this->numCombined;}

void Mouse::printPos(){
    std::cout << "Mouse Pos: (" << pos.x << ", " << pos.y << ", " << pos.z << ")  \r";
}
