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

uniform float ratio;

void main() 
{
    col = colAttr;

    vec3 dir = normalize(posAttr);
    vec3 atr = posAttr*(1-ratio) + dir*ratio;
    FragPos = vec3(model * vec4(atr, 1.0));

    vec3 normal = normalAttr*(1-ratio)+dir*ratio;
    Normal = mat3(transpose(inverse(model))) * normal;  
    
    TexCoords = aTexCoords;
    gl_Position =  projection * view * model * vec4(atr, 1.0f);
}