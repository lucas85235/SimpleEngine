#version 330 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Color;

uniform mat4 uView;
uniform mat4 uProj;
uniform mat4 uModel;
uniform vec3 uLightDirection;

out vec3 v_Color;

void main() {
    v_Color = a_Color;
    vec4 world_position = uModel * vec4(a_Position, 1.0);
    gl_Position = uProj * uView * world_position;
}
