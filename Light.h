#ifndef LIGHT_H
#define LIGHT_H

#include <glm/glm.hpp>

#include "Shader.h"
#include "Camera.h"
#include "ImportOBJ.h"
#include <string>

class Light
{
    public:
        Light();

        void initialize(ImportOBJ importer);

        void setUniforms(Shader shadProgram, Camera cam);

        void setColor(glm::vec3 newColor);
        void setPos(glm::vec3 newPos);
        void setAmbStrength(float newAmbient);
        void setDifStrength(float newDiffuse);
        void setSpecStrength(float newSpecular);
        void setSpecHighlight(int newSpecHighlight);

        void changeColorBy(glm::vec3 deltaColor);
        void changePosBy(glm::vec3 deltaPos);
        void changePosBy(double x, double y, double z);
        void changeAmbStrengthBy(float deltaAmb);
        void changeDifStrengthBy(float deltaDif);
        void changeSpecStrengthBy(float deltaSpec);


        void draw(Shader shadProgram);

        glm::vec3 getPos();
        glm::vec3 getColor();
        glm::vec4 getColorv4();

        float getAmbStrength();
        float getDifStrength();
        float getSpecStrength();


        std::string color3();

    protected:

    private:
        glm::vec3 pos;
        glm::vec3 color;

        float scale;

        float ambientStrength;
        float diffuseStrength;
        float specularStrength;

        unsigned int lightVAO;
        unsigned int numCombined;
};

#endif // LIGHT_H
