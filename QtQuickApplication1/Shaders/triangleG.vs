#version 330

in vec3 posAttr;
in vec3 colAttr;
in vec3 normalAttr;

out VS_OUT {
    vec3 normalAttr;
    vec3 posAttr;
} vs_out;

void main() 
{    
    vs_out.normalAttr = normalAttr;
    vs_out.posAttr = posAttr;

    gl_Position = vec4(posAttr, 1.0f);
}