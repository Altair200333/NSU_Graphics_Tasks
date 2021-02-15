#version 330

out vec4 fragColor;
in vec3 col;

in vec3 FragPos;
in vec3 Normal;

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

vec3 getBaseColor()
{
   if(mode == 1)
      return col;
   return color.xyz;
}
vec3 getLighting()
{
   vec3 dirToFrag = normalize(FragPos - cameraPos);
   vec3 norm = normalize(Normal);
   
   vec3 result = vec3(0);
   vec3 baseColor = getBaseColor();
   if(isLightSource)
      return baseColor;
   for(int i=0; i < lightsCount; ++i)
   {
      vec3 dirToLight = lights[i].position - FragPos;
      dirToLight = normalize(dirToLight);
      vec3 mColor = vec3(baseColor.x * lights[i].color.x, baseColor.y * lights[i].color.y, baseColor.z * lights[i].color.z);
      result = result + mColor * max(dot(dirToLight, norm), 0);
   }
   return result;
}
void main() 
{
   fragColor = vec4(getLighting(), 1.0f);
}