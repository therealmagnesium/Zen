#version 450 core
in vec2 fragCoord;

out vec4 finalColor;

uniform sampler2D screenTexture;

void main()
{
    const vec3 tint = vec3(1.f);

    vec3 texel = texture(screenTexture, fragCoord).xyz;
    vec3 result = texel * tint;

    finalColor = vec4(result, 1.f);
}
