#version 330

out vec4 fragColor;
in vec3 col;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 cameraPos;

uniform vec4 color;
uniform int mode;

struct LightSource
{
   vec3 position;
   vec4 color;
};

uniform LightSource lights[10];
uniform int lightsCount;
uniform bool isLightSource;

uniform int albedoCount;
uniform sampler2D texture;

const float PI = 3.14159265359;

void main() 
{
   fragColor = vec4(1,1,1, 0.3f);
}