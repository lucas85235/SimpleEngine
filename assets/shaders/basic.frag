#version 330 core
layout(location = 0) out vec4 color;

in vec3 v_Color;
in vec3 v_Normal;
in vec4 v_LightSpacePos;

uniform vec3 uLightDirection;
uniform vec3 uLightColor;
uniform float uLightIntensity;
uniform sampler2D uShadowMap;
uniform float uReceiveShadows;
uniform float uShadowsEnabled;

float CalculateShadow(vec4 lightSpacePos, vec3 normal, vec3 lightDir) {
    vec3 projCoords = lightSpacePos.xyz / lightSpacePos.w;
    projCoords = projCoords * 0.5 + 0.5;

    if (projCoords.z > 1.0 || projCoords.z < 0.0)
    return 0.0;

    if (projCoords.x < 0.0 || projCoords.x > 1.0 || projCoords.y < 0.0 || projCoords.y > 1.0)
    return 0.0;

    float ndotl = max(dot(normal, lightDir), 0.0);
    float bias = max(0.0025, 0.05 * (1.0 - ndotl));
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(uShadowMap, 0);
    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(uShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += projCoords.z - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;
    return shadow;
}

void main() {
    vec3 normal = normalize(v_Normal);
    vec3 lightDir = normalize(uLightDirection);

    float diff = max(dot(normal, lightDir), 0.0);

    float shadow =
    (uReceiveShadows > 0.5 && uShadowsEnabled > 0.5) ?
    CalculateShadow(v_LightSpacePos, normal, lightDir) :
    0.0;

    vec3 lightRadiance = uLightColor * max(uLightIntensity, 0.0);
    vec3 ambient = v_Color * 0.1;
    vec3 diffuse = v_Color * lightRadiance * diff;
    vec3 lighting = ambient + (1.0 - shadow) * diffuse;

    color = vec4(lighting, 1.0);
}