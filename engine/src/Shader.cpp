#include <SimpleEngine/Shader.h>
#include <stdexcept>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

// GLAD must be included before any GL headers usage
#include <glad/glad.h>

namespace se {

// helper to produce a short textual stage name
static const char* stageName(unsigned int type) {
    switch (type) {
        case GL_VERTEX_SHADER:   return "VERTEX";
        case GL_FRAGMENT_SHADER: return "FRAGMENT";
        case GL_GEOMETRY_SHADER: return "GEOMETRY";
        default: return "UNKNOWN";
    }
}

static std::string numberedSource(const char* src) {
    if (!src) return "<null>";
    std::istringstream iss(src);
    std::string line;
    int ln = 1;
    std::ostringstream out;
    while (std::getline(iss, line)) {
        out << ln << ": " << line << '\n';
        ++ln;
    }
    return out.str();
}

Shader::Shader(const char* vsSource, const char* fsSource) {
    if (!vsSource) throw std::invalid_argument("Vertex shader source is null");
    if (!fsSource) throw std::invalid_argument("Fragment shader source is null");

    unsigned int vs = 0;
    unsigned int fs = 0;
    program_ = 0;

    // compile stages first
    vs = compileStage(GL_VERTEX_SHADER, vsSource);
    fs = compileStage(GL_FRAGMENT_SHADER, fsSource);

    // create program and attach
    program_ = glCreateProgram();
    if (program_ == 0) {
        // cleanup compiled shaders
        if (vs) glDeleteShader(vs);
        if (fs) glDeleteShader(fs);
        throw std::runtime_error("glCreateProgram returned 0");
    }

    glAttachShader(program_, vs);
    glAttachShader(program_, fs);

    glLinkProgram(program_);
    try {
        checkCompile(program_, /*isProgram=*/true);
    } catch (...) {
        // ensure shaders are deleted even on error
        glDetachShader(program_, vs);
        glDetachShader(program_, fs);
        glDeleteShader(vs);
        glDeleteShader(fs);
        // delete program also
        glDeleteProgram(program_);
        program_ = 0;
        throw; // rethrow original exception
    }

    // On success, delete shader objects (they are no longer needed after linking)
    glDetachShader(program_, vs);
    glDetachShader(program_, fs);
    glDeleteShader(vs);
    glDeleteShader(fs);
}

Shader::~Shader() {
    if (program_) glDeleteProgram(program_);
}

void Shader::bind() const {
    if (program_) glUseProgram(program_);
}

void Shader::unbind() const {
    glUseProgram(0);
}

void Shader::setFloat(const char* name, float value) const {
    int loc = uniformLocation(name);
    if (loc >= 0) glUniform1f(loc, value);
}

unsigned int Shader::compileStage(unsigned int type, const char* src) {
    if (!src) throw std::invalid_argument("Shader source is null");

    unsigned int id = glCreateShader(type);
    if (!id) throw std::runtime_error(std::string("glCreateShader failed for ") + stageName(type));

    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    try {
        checkCompile(id, /*isProgram=*/false);
    } catch (const std::exception& e) {
        // include source with line numbers to help debugging
        std::string msg = std::string(e.what()) + "\n---- Shader Source (" + stageName(type) + ") ----\n" + numberedSource(src);
        // cleanup shader before rethrowing
        glDeleteShader(id);
        throw std::runtime_error(msg);
    }

    return id;
}

void Shader::checkCompile(unsigned int id, bool isProgram) {
    GLint success = 0;
    if (isProgram) {
        glGetProgramiv(id, GL_LINK_STATUS, &success);
        if (!success) {
            GLint len = 0;
            glGetProgramiv(id, GL_INFO_LOG_LENGTH, &len);
            std::vector<char> log((len > 0) ? (len + 1) : 1);
            glGetProgramInfoLog(id, (GLsizei)log.size(), nullptr, log.data());
            throw std::runtime_error(std::string("Program link error:\n") + log.data());
        }
    } else {
        glGetShaderiv(id, GL_COMPILE_STATUS, &success);
        if (!success) {
            GLint len = 0;
            glGetShaderiv(id, GL_INFO_LOG_LENGTH, &len);
            std::vector<char> log((len > 0) ? (len + 1) : 1);
            glGetShaderInfoLog(id, (GLsizei)log.size(), nullptr, log.data());
            throw std::runtime_error(std::string("Shader compile error:\n") + log.data());
        }
    }
}

int Shader::uniformLocation(const char* name) const {
    if (!program_) return -1;
    return glGetUniformLocation(program_, name);
}

} // namespace se
