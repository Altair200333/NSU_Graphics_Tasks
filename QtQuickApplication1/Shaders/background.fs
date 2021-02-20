#version 330
out vec4 fragColor;

in vec2 texcoord;
uniform sampler2D background;

const float PI = 3.14159265359;

uniform vec3 origin;
uniform vec3 front;
uniform vec3 right;
uniform vec3 up;
uniform float fov;
uniform float aspectRatio;

uniform int width;
uniform int height;

vec3 getDirection()
{
   float h = 2.0f * tan(fov * 0.5f * PI / 180.0f);
   float x = (texcoord.x-0.5f)*aspectRatio*h;
   float y = (texcoord.y-0.5f)*h;
   return front+right*x+up*y;
}

vec2 SampleSphericalMap(vec3 direction)
{
   float theta = atan(direction.z, direction.x) * 180 / PI + 180;
   float alpha = atan(direction.y, sqrt(direction.x * direction.x + direction.z * direction.z)) * 180 / PI + 90;  
   float x = theta / 360;
   float y = alpha / 180;

   return vec2(x, 1-y);
}
void main() 
{
   vec3 direction = normalize(getDirection());
   vec2 uv = SampleSphericalMap(direction);

   vec3 color = texture(background, uv).rgb;

   fragColor = vec4(color, 1.0f);
}