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

    vec3 ambient_light_color = vec3(0.2);
    vec3 lightColor = vec3(0.2,0.2,0.15);
    vec3 light_dir = vec3(1.0,1.0,0.0);

    vec4 world_position = uModel * vec4(a_Position, 1.0);

    vec3 normal_dir = mat3(transpose(inverse(uModel))) * a_Normal;

    float diff = max(dot(normal_dir, -light_dir), 0.0);
    vec3 diffuse = lightColor * diff;


    v_Color = ambient_light_color + diffuse;
    gl_Position = uProj * uView * world_position;
}