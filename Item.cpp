#include "Item.h"

#include "oGL_Tools.h"

Item::Item()
{
    this->pos = glm::vec3(0.0f, 0.0f, 0.0f);
    this->rotateAngle = 0.0;
    this->rotAxis = glm::vec3(0.0, 1.0, 0.0);
    this->scale = 2.5;
    this->hasTexture = false;
}

void Item::initialize(ImportOBJ importer, std::string name) {
    this->VAO = importer.loadFiles("models/" + name);
    this->numCombined = importer.getNumCombined();
    if (hasTexture) this->texID = importer.getTextID("models/" + name);
}

void Item::draw(Shader shadProgram) {



    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, this->pos);
    model = glm::rotate(model, glm::radians(this->rotateAngle), this->rotAxis);
    model = glm::scale(model, glm::vec3(this->scale, this->scale, this->scale));
    shadProgram.setMatrix("model", model);

    // for lighting
    shadProgram.setBool("mat.useSpecularMap", false);

    if (this->hasTexture){
        shadProgram.setBool("mat.useDiffuseMap", true);
        shadProgram.setInt("mat.DiffuseMap", this->texID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->texID);
        //std::cout << "TextID = " << this->texID;
    }
    else {
         shadProgram.setBool("mat.useDiffuseMap", false);
    }
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, this->numCombined);

}

void Item::setPos(glm::vec3 newPos) {this->pos = newPos;}
void Item::setRotation(float newAngle) {this->rotateAngle = newAngle;}
void Item::setScale(float newScale) {this->scale = newScale;}
void Item::setHasTexture(bool val) {this->hasTexture = val;}

glm::vec3 Item::getPos() {return this->pos;}
unsigned int Item::getVAO() {return this->VAO;}
unsigned int Item::getnumCombined() {return this->numCombined;}

void Item::printPos(){
    std::cout << "Cat Item: (" << pos.x << ", " << pos.y << ", " << pos.z << ")  \r";
}
