#version 330 core
in vec3 vColor;
out vec4 FragColor;
uniform float uMix;
void main() {
    vec3 mixed = mix(vColor, vec3(1.0, 1.0, 1.0), uMix);
    FragColor = vec4(mixed, 1.0);
}