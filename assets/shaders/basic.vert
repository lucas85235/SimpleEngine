#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec3 aColor;

uniform mat4 uView;
uniform mat4 uProj;

out vec3 vColor;

void main() {
    vec4 worldPos = vec4(aPos, 0.0, 1.0);
    gl_Position = uProj * uView * worldPos;
    vColor = aColor;
}
