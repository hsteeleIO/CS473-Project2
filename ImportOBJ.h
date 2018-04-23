#ifndef IMPORTOBJ_H
#define IMPORTOBJ_H

#include <string>
#include <vector>
#include <map>
#include <glm/glm.hpp>



class ImportOBJ{
    public:
        ImportOBJ();

        /** Returns a VAO after loading the .OBJ/.MTL files
          * Only provide the base name (without .OBJ/.MTL extension) */
        int loadFiles(std::string name_without_file_extension);
        bool debugOutput = false;

        int getNumCombined();
        int getTextID(int num);

    private:
        struct CompleteVertex {
            glm::vec3 Position;
            glm::vec3 Normal;
            glm::vec2 TexCoords;
            glm::vec3 DiffuseColor;
            glm::vec3 SpecularColor;
            float Shininess;
        };

        void readMTLFile(std::string fName);
        void readOBJFile(std::string fName);
        int genOBJ_VAO();
        void reset();

        int curMat = -1;

        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> textCoords;
        std::vector<CompleteVertex> combinedData;
        std::map<std::string, int> matAbbrev;
        std::vector<glm::vec3> matDiffuse;
        std::vector<glm::vec3> matSpecular;
        std::vector<float> matShiny;
        std::vector<int> map_Kd;

        void readLineFace(std::string line);
        void readFace(std::string lineSegment);
        glm::vec3 getVec3(std::string line);
        glm::vec2 getVec2(std::string line);
};



#endif // IMPORTOBJ_H
