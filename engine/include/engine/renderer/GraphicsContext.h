#pragma once

struct GLFWwindow;

namespace se {

    class GraphicsContext {
    public:
        GraphicsContext(GLFWwindow* windowHandle);
        ~GraphicsContext();

        void Init();
        void SwapBuffers();

    private:
        GLFWwindow* windowHandle_;
    };

} // namespace se