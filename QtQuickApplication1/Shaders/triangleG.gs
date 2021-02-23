#version 330
layout (triangles) in;
layout (triangle_strip, max_vertices = 48) out;

in VS_OUT {
    vec3 normalAttr;
    vec3 posAttr;
} gs_in[];

uniform float ratio;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform int subdivLevel;

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
struct Triangle
{
    vec3 vertices[3];
    vec3 normals[3];
};
Triangle triangles[100];
int bottom = 0;
int top = 0;

void addTriangle(vec3 _vertices[3], vec3 _normals[3])
{
    Triangle t;
    t.vertices = _vertices;
    t.normals = _normals;
    triangles[top] = t;
    top++;
}
void main()
{
    bottom = top = 0;
    addTriangle(vec3[3](gs_in[0].posAttr, gs_in[1].posAttr, gs_in[2].posAttr), vec3[3](gs_in[0].normalAttr, gs_in[0].normalAttr, gs_in[0].normalAttr));

    for(int i = 0; i<subdivLevel; i++)
    {
        int oldTop = top;
        for(int j = bottom; j < oldTop; j++)
        {
            Triangle current = triangles[j];
            bottom++;
            Triangle t1;
            t1.vertices[0] = current.vertices[0];
            t1.vertices[1] = (current.vertices[0] + current.vertices[1])*0.5;
            t1.vertices[2] = (current.vertices[0] + current.vertices[2])*0.5;
            t1.normals[0] = current.normals[0];
            t1.normals[1] = current.normals[0];
            t1.normals[2] = current.normals[0];
            addTriangle(t1.vertices, t1.normals);

            Triangle t2;
            t2.vertices[0] = current.vertices[1];
            t2.vertices[1] = (current.vertices[2] + current.vertices[1])*0.5;
            t2.vertices[2] = (current.vertices[0] + current.vertices[1])*0.5;
            t2.normals[0] = current.normals[0];
            t2.normals[1] = current.normals[0];
            t2.normals[2] = current.normals[0];
            addTriangle(t2.vertices, t2.normals);

            Triangle t3;
            t3.vertices[0] = current.vertices[2];
            t3.vertices[1] = (current.vertices[2] + current.vertices[0])*0.5;
            t3.vertices[2] = (current.vertices[2] + current.vertices[1])*0.5;
            t3.normals[0] = current.normals[0];
            t3.normals[1] = current.normals[0];
            t3.normals[2] = current.normals[0];
            addTriangle(t3.vertices, t3.normals);

            Triangle t4;
            t4.vertices[0] = (current.vertices[0] + current.vertices[1])*0.5;
            t4.vertices[1] = (current.vertices[1] + current.vertices[2])*0.5;
            t4.vertices[2] = (current.vertices[2] + current.vertices[0])*0.5;
            t4.normals[0] = current.normals[0];
            t4.normals[1] = current.normals[0];
            t4.normals[2] = current.normals[0];
            addTriangle(t4.vertices, t4.normals);
        }
    }

    for(int i = bottom; i<top; i++)
    {
        Triangle t = triangles[i];
        for(int j =0; j<3; j++)
        {
            genVertex(t.vertices[j], t.normals[j]);
        }
        EndPrimitive();
    }
}