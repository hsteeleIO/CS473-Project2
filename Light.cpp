#include "Light.h"

Light::Light()
{
    this->pos = glm::vec3(-17.5f, 2.5f, 14.5f);
    this->color = glm::vec3(1.0, 1.0, 1.0);
    this->scale = 1;

    this->ambientStrength = .80;
    this->diffuseStrength = .60;
    this->specularStrength = .60;
}

void Light::initialize(ImportOBJ importer){
    this->lightVAO = importer.loadFiles("models/light");
    this->numCombined = importer.getNumCombined();
}

void Light::draw(Shader shadProgram){
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, this->pos);
    //model = glm::rotate(model, glm::radians(this->rotateAngle), this->rotAxis);
    model = glm::scale(model, glm::vec3(this->scale, this->scale, this->scale));
    shadProgram.setMatrix("model", model);

    glBindVertexArray(this->lightVAO);
    glDrawArrays(GL_TRIANGLES, 0, this->numCombined);
}

void Light::setUniforms(Shader shadProgram, Camera cam){
    shadProgram.setVec3("camPos", cam.getPos());
    //sets light uniform
    shadProgram.setVec3("light.color", color);
    shadProgram.setVec3("light.pos", pos);
    shadProgram.setFloat("light.ambientStrength", ambientStrength);
    shadProgram.setFloat("light.diffuseStrength", diffuseStrength);
    shadProgram.setFloat("light.specularStrength", specularStrength);
}

void Light::setColor(glm::vec3 newColor) {this->color = newColor;}
void Light::setPos(glm::vec3 newPos) {this->pos = newPos;}
void Light::setAmbStrength(float newAmbient) {this->ambientStrength = newAmbient;}
void Light::setDifStrength(float newDiffuse) {this->diffuseStrength = newDiffuse;}
void Light::setSpecStrength(float newSpecular) {this->specularStrength = newSpecular;}

glm::vec3 Light::getPos() {return this->pos;}
glm::vec3 Light::getColor() {return this->color;}
