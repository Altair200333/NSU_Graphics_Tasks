#version 330

out vec4 fragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 cameraPos;

uniform vec4 color;
uniform vec4 ambient;
uniform vec4 specular;

uniform float roughness;

uniform sampler2D texture_background;
uniform bool useBackground;

const float PI = 3.14159265359;

uniform bool wireframe;

vec2 SampleSphericalMap(vec3 direction)
{
   float theta = atan(direction.z, direction.x) * 180 / PI + 180;
   float alpha = atan(direction.y, sqrt(direction.x * direction.x + direction.z * direction.z)) * 180 / PI + 90;  
   float x = theta / 360;
   float y = alpha / 180;

   return vec2(x, 1-y);
}

in vec3 guroColor;

void main() 
{
   if(wireframe)
   {
      fragColor = vec4(1, 0.682, 0, 1.0f);
   }
   else
   {
      fragColor = vec4(guroColor, 1.0f);
   }
}