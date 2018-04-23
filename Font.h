#ifndef FONT_H
#define FONT_H

#include "Shader.h"

#include <string>
#include <glm/glm.hpp>

class Font
{
    public:
        Font(std::string fontBMP, std::string fontCSV);
        void initialize();

        // Draws the string starting at a given X/Y coordinate (lower left hand)
        void drawText(std::string s, glm::vec2 start, Shader sProgram);

        void setScale(float newScale);

        int getEndNum();
        int getStartNum();
        unsigned int getTexNum();
        int getCharVAO(int index);
        int getCharWidth(int index);

    private:
        glm::vec2 getTexCoords(unsigned char c);
        int genCharVAO(unsigned char c);

        std::string BMPfilename;
        std::string CSVfilename;

        float scale;
        int charVAOs[256] = {0};
        int charWidth[256] = {0};

        int texWidth;
        int texHeight;
        int cellWidth;
        int cellHeight;
        int cellsRow;       // Number of cells per row
        int fontHeight;
        int startNum;       // Lowest ASCII number covered
        int endNum;         // Higher ASCII number covered

        unsigned int texNumber;     // OpenGL Texture # for the bitmap font
};

#endif // FONT_H
