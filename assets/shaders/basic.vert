#version 330 core
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Color;
layout(location = 2) in vec3 a_Normal;

uniform mat4 uView;
uniform mat4 uProj;
uniform mat4 uModel;
uniform mat4 uLightSpaceMatrix;

out vec3 v_Color;
out vec3 v_Normal;
out vec4 v_LightSpacePos;

void main() {
    vec4 world_position = uModel * vec4(a_Position, 1.0);
    v_Normal = mat3(transpose(inverse(uModel))) * a_Normal;
    v_Color = a_Color;
    v_LightSpacePos = uLightSpaceMatrix * world_position;
    gl_Position = uProj * uView * world_position;
}