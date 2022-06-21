#version 330 core 
#extension GL_ARB_separate_shader_objects : enable
layout(location = 0) out vec4 FragColor;

layout(location = 0) in vec2 inTexCoords;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inFragPos;
layout(location = 3) in vec4 inFragPosLightSpace;

uniform sampler2D DiffuseMap;
uniform sampler2D ShadowMap;
uniform vec3 LightPos;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide - useless for ortographic projection but necessary for perspective
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;


    // Fix Over Sampling
    if (projCoords.z > 1.0)
        return 0.0;

    // Transform ndc coords(from [-1, 1]) to tex coords([0, 1]) 
    projCoords = projCoords * 0.5 + 0.5;

    // Get depth value of first hit fragment from shadow map
    float closestDepth = texture(ShadowMap, projCoords.xy).r;

    // Get current fragment's depth value
    float currentDepth = projCoords.z;

    vec3 lightDir = normalize(LightPos - inFragPos);
    float bias = max(0.05 * (1.0 - dot(normalize(inNormal), lightDir)), 0.005);

    // PCF
    float shadowValue = 0.0;
    vec2 texelSize = 1.0 / textureSize(ShadowMap, 0);
    for (int x = -1; x <= 1; ++x)
    {
        for (int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(ShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadowValue += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
        }
    }
    shadowValue /= 9.0;

    return shadowValue;
}

void main()
{
    vec4 color = texture(DiffuseMap, inTexCoords);

    float shadowValue = ShadowCalculation(inFragPosLightSpace);

    //shadowValue = abs(1.0 - shadowValue);
    shadowValue = (shadowValue + 1.0) / 2.0;
    color *= 1.0/shadowValue;

	FragColor = color;
}