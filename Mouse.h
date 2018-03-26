#ifndef MOUSE_H
#define MOUSE_H

#include <glm/glm.hpp>
#include "Shader.h"
#include "ImportOBJ.h"

class Mouse
{
    public:
        Mouse();
        void initialize(ImportOBJ importer);
        void draw(Shader shadProgram);

        void goForward(float amt);
        void strafeRight(float amt);

        void setPos(glm::vec3 newPos);
        void setRotation(float newAngle);
        void setScale(float newScale);

        glm::vec3 getPos();
        unsigned int getVAO();
        unsigned int getnumCombined();
        void printPos();

    protected:

    private:
        glm::vec3 pos;
        float scale;

        unsigned int VAO;
        unsigned int numCombined;
};

#endif // MOUSE_H
