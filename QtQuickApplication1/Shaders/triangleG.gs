#version 330
layout (triangles) in;
layout (triangle_strip, max_vertices = 12) out;

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

int getNext(int id, int offset)
{
    return (id+offset)%3;
}
void genVertex(vec3 pos, vec3 originalNormal)
{
    vec3 dir = normalize(pos);
    vec3 atr = pos*(1-ratio) + dir*ratio;
    FragPos = vec3(model * vec4(atr, 1.0));

    vec3 normal = originalNormal*(1-ratio)+dir*ratio;
    Normal = mat3(transpose(inverse(model))) * normal;  
    
    gl_Position =  projection * view * model * vec4(atr, 1.0f);

    EmitVertex();
}
void main()
{
    genVertex(gs_in[0].posAttr, gs_in[0].normalAttr);
    genVertex((gs_in[0].posAttr+gs_in[1].posAttr)*0.5f, gs_in[0].normalAttr);
    genVertex((gs_in[0].posAttr+gs_in[2].posAttr)*0.5f, gs_in[0].normalAttr);

    genVertex(gs_in[1].posAttr, gs_in[1].normalAttr);
    genVertex((gs_in[0].posAttr+gs_in[1].posAttr)*0.5f, gs_in[0].normalAttr);
    genVertex((gs_in[2].posAttr+gs_in[1].posAttr)*0.5f, gs_in[0].normalAttr);

    genVertex(gs_in[2].posAttr, gs_in[2].normalAttr);
    genVertex((gs_in[2].posAttr+gs_in[1].posAttr)*0.5f, gs_in[0].normalAttr);
    genVertex((gs_in[2].posAttr+gs_in[0].posAttr)*0.5f, gs_in[0].normalAttr);


    genVertex((gs_in[0].posAttr+gs_in[1].posAttr)*0.5f, gs_in[0].normalAttr);
    genVertex((gs_in[1].posAttr+gs_in[2].posAttr)*0.5f, gs_in[0].normalAttr);
    genVertex((gs_in[2].posAttr+gs_in[0].posAttr)*0.5f, gs_in[0].normalAttr);

    EndPrimitive();
}