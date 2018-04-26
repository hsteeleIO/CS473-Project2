#include "Font.h"

#include "oGL_Tools.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>

/////////////////////////////// Initialization functions
Font::Font(std::string fontBMP, std::string fontCSV)
{
    this->BMPfilename = fontBMP;
    this->CSVfilename = fontCSV;
    this->scale = 1.0;
}

void Font::initialize() {
    this->texNumber = loadTexture(this->BMPfilename);
    std::ifstream infile(this->CSVfilename.c_str());
    std::string line;
    int lineNum = 0;
    while (std::getline(infile, line)) {
        lineNum += 1;
        std::string field = line.substr(0, line.find(","));
        std::string amountStr = line.substr(line.find(",") + 1);
        int amount = (int)strtol(amountStr.c_str(), NULL, 10);

        if (lineNum <= 8) {
            switch(lineNum) {
                case 1: this->texWidth = amount; break;
                case 2: this->texHeight = amount; break;
                case 3: this->cellWidth = amount; break;
                case 4: this->cellHeight = amount; break;
                case 5: this->startNum = amount; break;
                case 7: this->fontHeight = amount; break;
            }
        }
        else if (lineNum <= 264) {
            this->charWidth[lineNum - 9] = amount;
        }
    }
    infile.close();

    this->cellsRow = this->texWidth / this->cellWidth;
    this->endNum = std::min(this->startNum - 1 + this->cellsRow * (this->texHeight / this->cellHeight), 255);

    for (int i = 0; i < 256; i++) {
        if (i >= this->startNum && i <= this->endNum) {
            charVAOs[i] = this->genCharVAO((char)i);
        }
    }
}



/////////////////////////////// Drawing text
void Font::drawText(std::string s, glm::vec2 start, Shader sProgram) {
    float x = start.x;
    float y = start.y;
    int c;
    int charVAO;
    for (int i = 0; i < s.length(); i++) {
        c = (int)s.at(i);
        int width = getCharWidth(c);
        charVAO = getCharVAO(c);
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(x, y, 1.0));
        model = glm::scale(model, glm::vec3(this->scale));

        sProgram.setMatrix("model", model);
        glBindTexture(GL_TEXTURE_2D, texNumber);
        glBindVertexArray(charVAO);
        //glDrawArrays(GL_TRIANGLES_FAN, 0, 4);
        x += width;
    }
}


/////////////////////////////// Setter functions
void Font::setScale(float newScale) {
    if (newScale > 0) this->scale = newScale;
}

/////////////////////////////// Getter functions
int Font::getEndNum() {return this->endNum;}
int Font::getStartNum() {return this->startNum;}
unsigned int Font::getTexNum() {return this->texNumber;}
int Font::getCharVAO(int index) {
    if (index < 0 || index > 255) throw std::invalid_argument("Invalid index to Font::getCharVAO");
    return this->charVAOs[index];
}
int Font::getCharWidth(int index) {
    if (index < 0 || index > 255) throw std::invalid_argument("Invalid index to Font::getCharWidth");
    return this->charWidth[index];
}


/////////////////////////////// Private functions

/** Returns texture coordinates for a given character */
glm::vec2 Font::getTexCoords(unsigned char c) {
    int ascii = (int)c;
    int col = ascii % this->cellsRow;
    int row = (ascii - this->startNum) / this->cellsRow;
    double texX = (col * this->cellWidth - 1) / (double)this->texWidth;
    double texY = 1 - (row * this->cellHeight / (double)this->texHeight) ;

    return glm::vec2(texX, texY);
}

int Font::genCharVAO(unsigned char c) {
    glm::vec2 ulh = this->getTexCoords(c);
    glm::vec2 lrh = glm::vec2(ulh.x + ((this->cellWidth) / (double)this->texWidth),
                     ulh.y - (this->fontHeight / (double)this->texHeight));

    float aspectRatio = (float)this->fontHeight / this->cellWidth;

    float vertices[] = {
        0.0f, aspectRatio, 0.0f, ulh.x, ulh.y,
        1.0f, aspectRatio, 0.0f, lrh.x, ulh.y,
        1.0f, 0.0f, 0.0f, lrh.x, lrh.y,
        0.0f, 0.0f, 0.0f, ulh.x, lrh.y
    };

    int finalVAO = setupVAOTex(vertices, sizeof(vertices), GL_STATIC_DRAW);

    return finalVAO;
}
