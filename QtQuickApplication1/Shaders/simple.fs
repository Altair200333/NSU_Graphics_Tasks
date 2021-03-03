#version 330

out vec4 fragColor;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 cameraPos;

uniform vec4 color;
uniform int mode;
uniform float roughness;

uniform sampler2D texture_background;
uniform bool useBackground;

const float PI = 3.14159265359;

struct LightSource
{
   vec3 position;
   vec4 color;
};

uniform LightSource lights[10];
uniform int lightsCount;

uniform bool wireframe;

vec2 SampleSphericalMap(vec3 direction)
{
   float theta = atan(direction.z, direction.x) * 180 / PI + 180;
   float alpha = atan(direction.y, sqrt(direction.x * direction.x + direction.z * direction.z)) * 180 / PI + 90;  
   float x = theta / 360;
   float y = alpha / 180;

   return vec2(x, 1-y);
}

vec3 getBaseColor()
{
   return color.xyz;
}
float attenuation(float dist)
{
   return 1.0f / (1.0f + 0.1f * dist + 0.35f * dist*dist);
}
vec3 getLighting()
{
   vec3 dirToFrag = normalize(FragPos - cameraPos);
   vec3 norm = normalize(Normal);
   
   vec3 result = vec3(0);
   vec3 baseColor = getBaseColor();
   vec3 color = texture(texture_background, SampleSphericalMap(reflect(dirToFrag, norm))).xyz;

   for(int i=0; i < lightsCount; ++i)
   {
      vec3 dirToLight = lights[i].position - FragPos;
      dirToLight = normalize(dirToLight);
      vec3 mColor = baseColor * vec3(lights[i].color);
      result +=  ((1-roughness)*mColor + roughness * color) * max(dot(dirToLight, norm), 0.05f) * attenuation(length(dirToLight));
   }

   return result;
}
void main() 
{
   if(wireframe)
   {
      fragColor = vec4(1, 0.682, 0, 1.0f);
   }
   else
   {
      fragColor = vec4(getLighting(), 1.0f);
   }
}