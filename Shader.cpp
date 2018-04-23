#include "Shader.h"

using namespace std;
// Now we don't need to put std:: in front of methods from
// Don't put this in the header file unless it is of limited scope
// (e.g. within a function)

Shader::Shader() {
}

Shader::Shader(std::string vP, std::string fP)
{
    this->vertexPath = vP;
    this->fragmentPath = fP;
}

void Shader::initialize() {
    // Part I: Retrieve source code from filepath and convert to C strings
    string vertexCode;
    string fragmentCode;
    ifstream vShaderFile;
    ifstream fShaderFile;

    // Ensure we can throw exceptions on ifstream objects
    vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
    fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);

    try {
        // open files
        vShaderFile.open(vertexPath.c_str());
        fShaderFile.open(fragmentPath.c_str());
        stringstream vShaderStream, fShaderStream;

        // Read file buffer's contents into the streams
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        // Close file handlers
        vShaderFile.close();
        fShaderFile.close();

        // Convert stream into string
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch(ifstream::failure e) {
        cout << "ERROR: Shader Class - File not successfully read\n";
    }

    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();

    // Part II: Compile shaders
    unsigned int vertex, fragment;
    int success;
    char infoLog[512];

    // Vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    // print compile errors
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 512, NULL, infoLog);
        cout << "ERROR: Shader Class - Vertex Shader compilation error\n" << infoLog << "\n";
    }

    // Fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    // print compile errors
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 512, NULL, infoLog);
        cout << "ERROR: Shader Class - Fragment Shader compilation error\n" << infoLog << "\n";
    }

    // Shader program
    this->ID = glCreateProgram();
    glAttachShader(this->ID, vertex);
    glAttachShader(this->ID, fragment);
    glLinkProgram(this->ID);
    // print linking errors
    glGetProgramiv(this->ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(this->ID, 512, NULL, infoLog);
        cout << "ERROR: Shader Class - Program linking error\n" << infoLog << "\n";
    }

    this->setDrawColor(1.0f, 1.0f, 1.0f, 1.0f);
    // Delete the shaders since they are linked into our program now
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use() {
    glUseProgram(this->ID);
}

void Shader::setBool(const string &name, bool value) const {
    glUniform1i(glGetUniformLocation(this->ID, name.c_str()), (int)value);
}

void Shader::setInt(const string &name, int value) const {
    glUniform1i(glGetUniformLocation(this->ID, name.c_str()), value);
}

void Shader::setFloat(const string &name, float value) const {
    glUniform1f(glGetUniformLocation(this->ID, name.c_str()), value);
}

void Shader::setVec3(const std::string &name, glm::vec3 vec) const{
    glUniform3f(glGetUniformLocation(this->ID, name.c_str()), vec.x, vec.y, vec.z);
}

void Shader::setDrawColor(float r, float g, float b, float a) const {
    glUniform4f(glGetUniformLocation(this->ID, "drawColor"), r, g, b, a);
}

void Shader::setDrawColor(glm::vec4 rgba) const {
    glUniform4f(glGetUniformLocation(this->ID, "drawColor"), rgba.r, rgba.g, rgba.b, rgba.a);
}

void Shader::setMatrix(const string &name, glm::mat4 matrix) const {
    glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::useOneTex(unsigned int tex1) const {
    this->setInt("texture1", tex1);
    this->setInt("activeTex", 1);
}

void Shader::useTwoTex(float mixT2, unsigned int tex1, unsigned int tex2) const {
    this->setInt("texture1", tex1);
    this->setInt("texture2", tex2);
    this->setFloat("t2Mix", mixT2);
    this->setInt("activeTex", 2);
}

void Shader::noTextures() const {
    this->setInt("activeTex", 0);
}

