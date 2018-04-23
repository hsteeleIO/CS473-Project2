#ifndef CAT_H
#define CAT_H

#include <glm/glm.hpp>
#include "Shader.h"
#include "ImportOBJ.h"

class Cat
{
    public:
        Cat();
        void initialize(ImportOBJ importer);
        void draw(Shader shadProgram);

        void changeLock(bool val);
        void moveCatBy(float X, float Z);
        void goForward(float amt);
        void rotateCatBy(float amt);

        void setPos(glm::vec3 newPos);
        void setRotation(float newAngle);
        void setScale(float newScale);
        void reset();

        bool getLock();
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
        bool lock;

        unsigned int VAO;
        unsigned int numCombined;
};

#endif // CAT_H
