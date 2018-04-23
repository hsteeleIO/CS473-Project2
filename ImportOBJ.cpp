#include "ImportOBJ.h"
#include <stdlib.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include "oGL_Tools.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

ImportOBJ::ImportOBJ() {
}

int ImportOBJ::loadFiles(std::string baseName) {
    this->reset();
    std::string matName = baseName + ".mtl";
    std::string objName = baseName + ".obj";
    this->readMTLFile(matName);
    this->readOBJFile(objName);

    return this->genOBJ_VAO();
}

void ImportOBJ::readMTLFile(std::string fName) {
    std::ifstream infile(fName.c_str());
    if (infile.fail()) {
        std::cout << "ERROR: File " << fName << " does not exist.\n";
        return;
    }

    std::string curLine;
    int matCount = 0;   // Index of next material to be added

    while (std::getline(infile, curLine)) {
        std::string linePrefix;
        std::string curMatName;
        std::istringstream iss(curLine, std::istringstream::in);
        iss >> linePrefix;


        if (linePrefix == "newmtl") {
            curMatName = curLine.substr(7);
            //std::cout << "Material: >>>" << curMatName << "<<< is mat " << matCount << "\n";
            this->matAbbrev.insert(std::pair<std::string, int>(curMatName, matCount));
            matCount += 1;
        }

        // Diffuse color
        else if (linePrefix == "Kd") {
            glm::vec3 color = getVec3(curLine);
            this->matDiffuse.push_back(color);
            //std::cout << "Kd is: " << curLine << "\n";
        }
        else if (linePrefix == "Ks") {
            glm::vec3 color = getVec3(curLine);
            this->matSpecular.push_back(color);
            //std::cout << "Ks is: " << curLine << "\n";
        }
        else if (linePrefix == "Ns") {
            float shiny = strtof(curLine.substr(7).c_str(), NULL);
            this->matShiny.push_back(shiny);
            //std::cout << "Ni is: " << curLine << "\n";
        }
        else if (linePrefix == "map_Kd") {
            std::string line = curLine;
            int lastSlashPos = line.rfind("\\");
            std::string name = "textures\\";
            std::string fileName = line.substr(lastSlashPos+1, line.length()-lastSlashPos).c_str();
            int texture = loadTexture(name + fileName);
            this->map_Kd.push_back(texture);
            //std::cout << "Ni is: " << curLine << "\n";
        }
    }
}


/** Loads .OBJ file into the ImportOBJ data structures */
void ImportOBJ::readOBJFile(std::string fName) {
    std::ifstream infile(fName.c_str());
    if (infile.fail()) {
        std::cout << "ERROR: File " << fName << " does not exist.\n";
        return;
    }

    std::string curLine;

    // Push placeholder vec3s to our data structures
    // Allows index numbers to directly align with vertex# or
    // normal# in the OBJ file (OBJ vertex numbers start at 1)
    this->vertices.push_back(glm::vec3(-1.0, -1.0, -1.0));
    this->normals.push_back(glm::vec3(-1.0, -1.0, -1.0));
    this->textCoords.push_back(glm::vec2(-1.0, -1.0));


    while (std::getline(infile, curLine)) {
        // .OBJ lines are prefixed to indicate what information they contain
        // Designate an input string stream to capture it
        std::string linePrefix;
        std::istringstream iss(curLine, std::istringstream::in);
        iss >> linePrefix;

        // std::cout << "Reading file: " << linePrefix << " || " << curLine << "\n";

        // Standard vertex coordinate
        if (linePrefix == "v") {
            this->vertices.push_back(this->getVec3(curLine));
        }

        // Vertex normal
        else if (linePrefix == "vn") {
            this->normals.push_back(this->getVec3(curLine));
        }

        // Vertex texture
        else if (linePrefix == "vt") {
            this->textCoords.push_back(this->getVec2(curLine));
        }

        // Changes the material being used
        else if (linePrefix == "usemtl") {
            std::string matName = curLine.substr(7);
            //std::cout << "CHK rial: >>>" << matName << "<<< is " << this->matAbbrev.find(matName)->second << "\n";
            this->curMat = this->matAbbrev.find(matName)->second;
            //std::cout << "Now using material: " << matName << " is index " << this->curMat << this->matDiffuse.at(this->curMat).x << ", " << this->matDiffuse.at(this->curMat).y << ", " << this->matDiffuse.at(this->curMat).z << ")\n";
        }

        // Face
        else if (linePrefix == "f") {
            this->readLineFace(curLine);
        }
    }

    if (debugOutput) {
        std::cout << fName << " file parsed.\n";
        std::cout << vertices.size() << " vertices parsed.\n";
        std::cout << normals.size() << " normals parsed.\n";
        std::cout << textCoords.size() << " texture coordinates parsed.\n";
        std::cout << combinedData.size() << " combined points.\n";
        std::cout << matDiffuse.size() << " diffuse colors.\n";

        infile.close();
    }
}

int ImportOBJ::getNumCombined() {
    return this->combinedData.size();
}

int ImportOBJ::getTextID() {
    return this->map_Kd.at(0);
}

/** Generates VAO from stored vertices and texture coordinates. */
int ImportOBJ::genOBJ_VAO() {

    unsigned int VBO, VAO;
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, this->combinedData.size() * sizeof(CompleteVertex), &this->combinedData[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(CompleteVertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(CompleteVertex), (void*)offsetof(CompleteVertex, Normal));
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(CompleteVertex), (void*)offsetof(CompleteVertex, TexCoords));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(CompleteVertex), (void*)offsetof(CompleteVertex, DiffuseColor));
    glEnableVertexAttribArray(3);

    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(CompleteVertex), (void*)offsetof(CompleteVertex, SpecularColor));
    glEnableVertexAttribArray(4);

    glVertexAttribPointer(5, 1, GL_FLOAT, GL_FALSE, sizeof(CompleteVertex), (void*)offsetof(CompleteVertex, Shininess));
    glEnableVertexAttribArray(5);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

    return VAO;
}

/** Clears all internal data structures */
void ImportOBJ::reset() {
    this->vertices.clear();
    this->normals.clear();
    this->textCoords.clear();
    this->combinedData.clear();
    this->matAbbrev.clear();
    this->matDiffuse.clear();
}

/** Only works with faces broken down into triangles */
void ImportOBJ::readLineFace(std::string line) {
    int firstVertexStart = 2;
    int secondVertexStart = line.find(" ", firstVertexStart) + 1;
    int thirdVertexStart = line.find(" ", secondVertexStart) + 1;

    this->readFace(line.substr(firstVertexStart, secondVertexStart - 1));
    this->readFace(line.substr(secondVertexStart, thirdVertexStart - 1));
    this->readFace(line.substr(thirdVertexStart));
}

// Given a string in the format:
// xx/yy/zz returns a ivec3 of (xx, yy, zz)
void ImportOBJ::readFace(std::string lineSegment) {
    int indexY = lineSegment.find("/", 0) + 1;
    int indexZ = lineSegment.find("/", indexY) + 1;

    int x = strtol(lineSegment.substr(0, indexY - 1).c_str(), NULL, 10);
    int y = strtol(lineSegment.substr(indexY, indexZ - 1).c_str(), NULL, 10);
    int z = strtol(lineSegment.substr(indexZ).c_str(), NULL, 10);

    CompleteVertex newVert;
    newVert.Position = this->vertices.at(x);
    newVert.TexCoords = this->textCoords.at(y);
    newVert.Normal = this->normals.at(z);
    if (this->curMat != -1) newVert.DiffuseColor = this->matDiffuse.at(this->curMat);
    else newVert.DiffuseColor = glm::vec3(1.0, 1.0, 1.0);
    newVert.SpecularColor = this->matSpecular.at(this->curMat);
    newVert.Shininess = this->matShiny.at(this->curMat);

    this->combinedData.push_back(newVert);
}

glm::vec3 ImportOBJ::getVec3(std::string line) {
    // Line format, where _ is a space and XX designates the info on the line
    // mtllib = file name for materials
    // v = vertex coordinate
    // vn = vertex normal
    // vt = vertex texture coordinate
    // <x>, <y>, <z> are the x, y, and z coordinates

    // XX_<x>_<y>_<z>

    // Finds index of start of x, y, and z coordinates
    int indexX = line.find(" ", 0) + 1;
    int indexY = line.find(" ", indexX) + 1;
    int indexZ = line.find(" ", indexY) + 1;
    float x = strtof(line.substr(indexX, indexY-1).c_str(), NULL);
    float y = strtof(line.substr(indexY, indexZ-1).c_str(), NULL);
    float z = strtof(line.substr(indexZ).c_str(), NULL);
    return glm::vec3(x, y, z);
}

glm::vec2 ImportOBJ::getVec2(std::string line) {
    // As getVec3, except only gets x and y
    // Used primarily for vt coordinates
    int indexX = line.find(" ", 0) + 1;
    int indexY = line.find(" ", indexX) + 1;
    float x = strtof(line.substr(indexX, indexY-1).c_str(), NULL);
    float y = strtof(line.substr(indexY).c_str(), NULL);
    return glm::vec2(x, y);
}



