#include <SimpleEngine/Shader.h>
#include <stdexcept>
#include <vector>
#include <string>

// GLAD must be included before OpenGL headers usage
#include <glad/glad.h>

namespace se {

Shader::Shader(const char* vsSource, const char* fsSource) {
    unsigned int vs = compileStage(GL_VERTEX_SHADER,   vsSource);
    unsigned int fs = compileStage(GL_FRAGMENT_SHADER, fsSource);

    program_ = glCreateProgram();
    glAttachShader(program_, vs);
    glAttachShader(program_, fs);
    glLinkProgram(program_);
    checkCompile(program_, /*isProgram=*/true);

    glDeleteShader(vs);
    glDeleteShader(fs);
}

Shader::~Shader() {
    if (program_) glDeleteProgram(program_);
}

void Shader::bind() const {
    glUseProgram(program_);
}

void Shader::unbind() const {
    glUseProgram(0);
}

void Shader::setFloat(const char* name, float value) const {
    int loc = uniformLocation(name);
    if (loc >= 0) glUniform1f(loc, value);
}

unsigned int Shader::compileStage(unsigned int type, const char* src) {
    unsigned int id = glCreateShader(type);
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
    checkCompile(id, /*isProgram=*/false);
    return id;
}

void Shader::checkCompile(unsigned int id, bool isProgram) {
    int success = 0;
    if (isProgram) {
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if (!success) {
            int len = 0; glGetProgramiv(id, GL_INFO_LOG_LENGTH, &len);
            std::string log(len, '\0');
            glGetProgramInfoLog(id, len, nullptr, log.data());
            throw std::runtime_error("Program link error:\n" + log);
        }
    } else {
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success) {
            int len = 0; glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
            std::string log(len, '\0');
            glGetShaderInfoLog(id, len, nullptr, log.data());
            throw std::runtime_error("Shader compile error:\n" + log);
        }
    }
}

int Shader::uniformLocation(const char* name) const {
    return glGetUniformLocation(program_, name);
}

} // namespace se
