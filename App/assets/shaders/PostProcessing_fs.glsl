#version 450 core
in vec2 fragCoord;

out vec4 finalColor;

uniform sampler2D screenTexture;
uniform float gamma;
uniform float exposure;

vec3 ApplyGammaCorrection(vec3 color)
{
    vec3 newColor = pow(color, vec3(1.f / gamma));
    return newColor;
}

vec3 ApplyToneMapping(vec3 color)
{
    vec3 newColor = vec3(1.f) - exp(-color * exposure);
    return newColor;
}

void main()
{
    const vec3 tint = vec3(1.f);

    vec3 texel = texture(screenTexture, fragCoord).xyz;
    vec3 result = texel * tint;
    result = ApplyToneMapping(result);
    result = ApplyGammaCorrection(result);


    finalColor = vec4(result, 1.f);
}
