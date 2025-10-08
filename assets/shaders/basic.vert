#version 330 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Color;
layout(location = 2) in vec3 a_Normal;

uniform mat4 uView;
uniform mat4 uProj;
uniform mat4 uModel;
uniform vec3 uLightDirection;

out vec3 v_Color;

void main() {
    vec3 normal_dir = -a_Normal;
    vec4 world_position = uModel * vec4(a_Position, 1.0);
    normal_dir = mat3(transpose(inverse(uModel))) * normal_dir;
    v_Color = normal_dir;
    gl_Position = uProj * uView * world_position;
}