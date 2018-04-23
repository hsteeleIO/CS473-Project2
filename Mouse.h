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
        void rotateMouseBy(float amt);
        void moveMouseBy(float X, float Z);

        void setPos(glm::vec3 newPos);
        void setRotation(float newAngle);
        void setScale(float newScale);
        void setCheckEscape(bool check);
        void checkEscape();
        void reset();

        glm::vec3 getPos();
        bool hasEscaped();
        unsigned int getVAO();
        unsigned int getnumCombined();
        void printPos();

    protected:

    private:
        glm::vec3 pos;
        float rotateAngle;
        glm::vec3 rotAxis;
        float scale;
        bool Escape;

        unsigned int VAO;
        unsigned int numCombined;
};

#endif // MOUSE_H
