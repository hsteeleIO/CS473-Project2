#ifndef HOUSE_H
#define HOUSE_H

#include <glm/glm.hpp>
#include "Shader.h"
#include "ImportOBJ.h"

class House
{
    public:
        House();
        void initialize(ImportOBJ importer);
        void draw(Shader shadProgram);

        unsigned int getVAO();
        unsigned int getnumCombined();

    protected:

    private:
        glm::vec3 pos;
        float scale;

        unsigned int VAO;
        unsigned int numCombined;
        unsigned int texID1;
        unsigned int texID2;
        bool hasTexture;
};

#endif // HOUSE_H
