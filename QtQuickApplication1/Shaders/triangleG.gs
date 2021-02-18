#version 330
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
    vec3 normalAttr;
    vec3 posAttr;
} gs_in[];

uniform float ratio;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 FragPos;
out vec3 Normal;

void main()
{
    for(int i=0; i<3; i++)
    {
        vec3 dir = normalize(gs_in[i].posAttr);
        vec3 atr = gs_in[i].posAttr*(1-ratio) + dir*ratio;
        FragPos = vec3(model * vec4(atr, 1.0));

        vec3 normal = gs_in[i].normalAttr*(1-ratio)+dir*ratio;
        Normal = mat3(transpose(inverse(model))) * normal;  
        
        gl_Position =  projection * view * model * vec4(atr, 1.0f);

        EmitVertex();
    }
    EndPrimitive();
}