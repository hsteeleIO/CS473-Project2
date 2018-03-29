#include "Light.h"

Light::Light()
{
    this->pos = glm::vec3(0.0f, 5.0f, 0.0f);
    this->color = glm::vec3(0.0, 0.0, 0.0);
    this->scale = 1;

    this->ambientStrength = 1.0;
    this->diffuseStrength = 1.0;
    this->specularStrength = 0.5;
}

void initialize(){

}

void Light::draw(Shader shadProgram){

}

void Light::setColor(glm::vec3 newColor) {this->color = newColor;}
void Light::setPos(glm::vec3 newPos) {this->pos = newPos;}
void Light::setAmbStrength(float newAmbient) {this->ambientStrength = newAmbient;}
void Light::setDifStrength(float newDiffuse) {this->diffuseStrength = newDiffuse;}
void Light::setSpecStrength(float newSpecular) {this->specularStrength = newSpecular;}

glm::vec3 Light::getPos() {return this->pos;}
glm::vec3 Light::getColor() {return this->color;}
