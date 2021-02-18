#version 330

in vec3 posAttr;
in vec3 colAttr;
in vec3 normalAttr;

out vec3 col;
out vec3 FragPos;
out vec3 Normal;

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
    
    gl_Position =  projection * view * model * vec4(atr, 1.0f);
}