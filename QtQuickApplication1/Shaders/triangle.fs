#version 330

out vec4 fragColor;
in vec3 col;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 cameraPos;

void main() 
{
   vec3 dirToFrag = normalize(FragPos - cameraPos);
   vec3 norm = normalize(Normal);
   float angle = abs(dot(dirToFrag, norm));
   
   fragColor = vec4(angle, angle, 1, 1.0f);
}