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

void main() {
    col = colAttr;

    FragPos = vec3(model * vec4(posAttr, 1.0));
    Normal = mat3(transpose(inverse(model))) * normalAttr;  

    gl_Position =  projection * view * model * vec4(posAttr, 1.0f);
}