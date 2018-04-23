#include "House.h"

#include "oGL_Tools.h"


House::House()
{
    this->pos = glm::vec3(0.0f, -1.5f, 0.0f);
    this->scale = 4;
    this->hasTexture = true;
}

void House::initialize(ImportOBJ importer) {
    this->VAO = importer.loadFiles("models/house");
    this->numCombined = importer.getNumCombined();
    if (hasTexture) this->texID1 = importer.getTextID(0);
    if (hasTexture) this->texID2 = importer.getTextID(1);
}

void House::draw(Shader shadProgram) {
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, this->pos);
    //model = glm::rotate(model, glm::radians(this->rotateAngle), this->rotAxis);
    model = glm::scale(model, glm::vec3(this->scale, this->scale, this->scale));
    shadProgram.setMatrix("model", model);

    // for lighting
    shadProgram.setBool("mat.useSpecularMap", false);

    if (this->hasTexture){
        shadProgram.setBool("mat.useDiffuseMap", true);
        shadProgram.setInt("mat.DiffuseMap", 0);
        glActiveTexture(GL_TEXTURE0);
        shadProgram.useOneTex();
        glBindTexture(GL_TEXTURE_2D, this->texID1);
        //std::cout << "TextID = " << this->texID;
    }
    else {
         shadProgram.setBool("mat.useDiffuseMap", false);
    }
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, this->numCombined);
}

unsigned int House::getVAO() {return this->VAO;}
unsigned int House::getnumCombined() {return this->numCombined;}
