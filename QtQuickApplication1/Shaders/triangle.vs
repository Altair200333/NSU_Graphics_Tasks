#version 330

in vec3 posAttr;
in vec3 colAttr;
in vec3 normalAttr;

out vec3 col;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
    col = colAttr;
    gl_Position =  projection * view * model * vec4(posAttr, 1.0f);
}