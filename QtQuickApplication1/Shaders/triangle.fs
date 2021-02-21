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
uniform sampler2D texture_diffuse;

uniform sampler2D texture_background;
uniform bool useBackground;

const float PI = 3.14159265359;

vec3 getBaseColor()
{
   if(mode == 1)
      return col;

   if(albedoCount != 0)
     return texture(texture_diffuse, TexCoords).xyz;

   return color.xyz;
}
//-----------------------------------------------------------------------------
//from https://learnopengl.com/code_viewer_gh.php?code=src/2.lighting/6.multiple_lights/6.multiple_lights.fs
// ----------------------------------------------------------------------------
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}

float lAttenuation(float dist)
{
   return 1.0f;
   return 1.0f / (1.0f + 0.004f * dist*dist);
}

vec2 SampleSphericalMap(vec3 direction)
{
   float theta = atan(direction.z, direction.x) * 180 / PI + 180;
   float alpha = atan(direction.y, sqrt(direction.x * direction.x + direction.z * direction.z)) * 180 / PI + 90;  
   float x = theta / 360;
   float y = alpha / 180;

   return vec2(x, 1-y);
}

vec3 getLighting()
{
   vec3 V = normalize(cameraPos - FragPos);
   vec3 N = normalize(Normal);
   
   vec3 result = vec3(0);
   vec3 albedo = getBaseColor();
   float metallic  = 0.01f;
   float roughness = 0.1f;
   // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
   // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
   vec3 F0 = vec3(0.04); 
   F0 = mix(F0, albedo, metallic);

   // reflectance equation
   vec3 Lo = vec3(0.0);

   if(isLightSource)
      return albedo;

   albedo = pow(albedo, vec3(2.2));
   for(int i=0; i < lightsCount; ++i)
   {
      vec3 L = normalize(lights[i].position - FragPos);
      vec3 H = normalize(V + L);
      float dist = length(lights[i].position - FragPos);
      float attenuation = lAttenuation(dist);// / (distance * distance);
      vec3 radiance = vec3(lights[i].color) * attenuation;

      // Cook-Torrance BRDF
      float NDF = DistributionGGX(N, H, roughness);   
      float G   = GeometrySmith(N, V, L, roughness);      
      vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);
         
      vec3 nominator    = NDF * G * F; 
      float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; // 0.001 to prevent divide by zero.
      vec3 specular = nominator / denominator;
      
      // kS is equal to Fresnel
      vec3 kS = F;
      // for energy conservation, the diffuse and specular light can't
      // be above 1.0 (unless the surface emits light); to preserve this
      // relationship the diffuse component (kD) should equal 1.0 - kS.
      vec3 kD = vec3(1.0) - kS;
      // multiply kD by the inverse metalness such that only non-metals 
      // have diffuse lighting, or a linear blend if partly metal (pure metals
      // have no diffuse light).
      kD *= 1.0 - metallic;	  

      // scale light by NdotL
      float NdotL = max(dot(N, L), 0.0);        

      // add to outgoing radiance Lo
      Lo += (kD * albedo / PI + specular) * radiance * NdotL;
   }
   vec3 ambient = vec3(0.03) * albedo;

   if(useBackground)
   {
      Lo = Lo*(1-roughness)+roughness*texture(texture_background, SampleSphericalMap(reflect(-V, N))).xyz;
   }
   result = ambient + Lo;
   result = result / (result + vec3(1.0));
   result = pow(result, vec3(1.0/2.2)); 

   return result;
}
void main() 
{
   fragColor = vec4(getLighting(), 1.0f);
}