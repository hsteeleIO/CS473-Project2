#ifndef ITEM_H
#define ITEM_H

#include <glm/glm.hpp>
#include "Shader.h"
#include "ImportOBJ.h"
#include <string>

class Item
{
    public:
        Item();
        void initialize(ImportOBJ importer, std::string name);
        void draw(Shader shadProgram);

        void setPos(glm::vec3 newPos);
        void setRotation(float newAngle);
        void setScale(float newScale);
        void setHasTexture(bool val);

        glm::vec3 getPos();
        unsigned int getVAO();
        unsigned int getnumCombined();
        void printPos();

    protected:

    private:
        glm::vec3 pos;
        glm::vec3 rotAxis;
        float rotateAngle;
        float scale;

        unsigned int VAO;
        unsigned int numCombined;
        unsigned int texID;
        bool hasTexture;
};

#endif // ITEM_H
