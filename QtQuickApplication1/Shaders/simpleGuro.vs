#version 330

in vec3 posAttr;
in vec3 colAttr;
in vec3 normalAttr;
in vec2 aTexCoords;
in vec3 aTangent;
in vec3 aBitangent;

out vec3 col;
out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec4 color;
uniform vec4 ambient;
uniform vec4 specular;


const float PI = 3.14159265359;

struct LightSource
{
   vec3 position;
   vec4 color;
};

uniform LightSource lights[10];
uniform int lightsCount;


struct SpotLightSource
{
   vec3 position;
   vec3 direction;
   vec4 color;
   float cutOff;
   float outerCutOff;
   float intensity;
};

uniform SpotLightSource spotLights[10];
uniform int spotLightsCount;

uniform vec3 cameraPos;


vec3 getDiffuseColor()
{
   return color.xyz;
}
float attenuation(float dist)
{
   return 1.0f / (1.0f + 0.01f * dist + 0.01f * dist*dist);
}
vec3 getLighting()
{
   vec3 dirToFrag = normalize(FragPos - cameraPos);
   vec3 norm = normalize(Normal);
   
   vec3 result = vec3(0);
   vec3 baseColor = getDiffuseColor();
   
   for(int i=0; i < lightsCount; ++i)
   {
      vec3 dirToLight = normalize(lights[i].position - FragPos);

      vec3 diffuse = baseColor * vec3(lights[i].color);
      diffuse = diffuse * max(dot(dirToLight, norm), 0.0f);

      vec3 reflectDir = reflect(-dirToLight, norm);
      float spec = pow(max(dot(-dirToFrag, reflectDir), 0.0), 8.0);
      vec3 lightSpecular = specular.rgb * spec; 
      result +=  (diffuse + lightSpecular) * attenuation(length(lights[i].position - FragPos));
   }
   for(int i=0; i < spotLightsCount; ++i)
   {
      vec3 dirToLight = normalize(spotLights[i].position - FragPos);
      vec3 diffuse = baseColor * spotLights[i].color.rgb;
      diffuse = diffuse * max(dot(dirToLight, norm), 0.0f);
         
      vec3 reflectDir = reflect(-dirToLight, norm);  
      float spec = pow(max(dot(-dirToFrag, reflectDir), 0.0), 32.0f);
      vec3 lightSpecular = specular.rgb * spec;

      float theta = dot(dirToLight, normalize(-spotLights[i].direction)); 
      float epsilon = (spotLights[i].cutOff - spotLights[i].outerCutOff);
      float intensity = clamp((theta - spotLights[i].outerCutOff) / epsilon, 0.0, 1.0);
      diffuse  *= intensity;
      lightSpecular *= intensity;

      result += (diffuse + lightSpecular) * attenuation(length(spotLights[i].position - FragPos))*spotLights[i].intensity;
   }
   result = result + ambient.rgb*0.089f;
   return result;
}
out vec3 guroColor;
void main() 
{
    col = colAttr;

    FragPos = vec3(model * vec4(posAttr, 1.0));

    Normal = mat3(transpose(inverse(model))) * normalAttr;  
    
    TexCoords = aTexCoords;

    guroColor = getLighting();
    gl_Position =  projection * view * model * vec4(posAttr, 1.0f);
}