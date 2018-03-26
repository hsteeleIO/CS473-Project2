#ifndef CUBE_H
#define CUBE_H

#include <glm/glm.hpp>
#include "Shader.h"

class Cube
{
    public:
        Cube();
        void initializeMaster();
        void initializeClone(unsigned int newVAO, unsigned int newTex1, unsigned int newTex2);
        void initializeClone(Cube *otherCube);
        void draw(Shader shadProgram);

        void setPos(glm::vec3 newPos);
        void setRotation(float newAngle);
        void setScale(float newScale);

        glm::vec3 getPos();

        unsigned int getTex1();
        unsigned int getTex2();
        unsigned int getVAO();
        unsigned int getnumCombined();


    protected:

    private:
        glm::vec3 pos;
        glm::vec3 rotAxis;
        float rotateAngle;
        float scale;


        unsigned int VAO;
        unsigned int tex1;
        unsigned int tex2;
};

#endif // CUBE_H
