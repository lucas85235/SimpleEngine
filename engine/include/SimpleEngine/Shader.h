#pragma once
#include <string>
#include <cstdint>

namespace se {

class Shader {
public:
    Shader(const char* vsSource, const char* fsSource); // build from source strings
    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    void bind() const;
    void unbind() const;

    // Minimal uniform helper (float)
    void setFloat(const char* name, float value) const;

private:
    unsigned int program_ = 0;

    static unsigned int compileStage(unsigned int type, const char* src);
    static void checkCompile(unsigned int id, bool isProgram);
    int uniformLocation(const char* name) const;
};

} // namespace se
