#version 330

out vec4 fragColor;
in vec3 col;

in vec3 FragPos;
in vec3 LocalPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 cameraPos;

uniform vec4 color;
uniform int mode;

struct LightSource
{
   vec3 position;
   vec4 color;
};

uniform LightSource lights[10];
uniform int lightsCount;
uniform bool isLightSource;

uniform sampler2D texture_background;
uniform bool useBackground;

uniform bool wireframe;

const float PI = 3.14159265359;

vec3 getBaseColor()
{
   return color.xyz;
}

#define MARCHINGITERATIONS 64

#define MARCHINGSTEP 0.5
#define SMALLESTSTEP 0.1

#define DISTANCE 10.0

#define MAXMANDELBROTDIST 1.5
#define MANDELBROTSTEPS 64

vec2 SampleSphericalMap(vec3 direction)
{
   float theta = atan(direction.z, direction.x) * 180 / PI + 180;
   float alpha = atan(direction.y, sqrt(direction.x * direction.x + direction.z * direction.z)) * 180 / PI + 90;  
   float x = theta / 360;
   float y = alpha / 180;

   return vec2(x, 1-y);
}

// cosine based palette, 4 vec3 params
vec3 cosineColor( in float t, in vec3 a, in vec3 b, in vec3 c, in vec3 d )
{
    return a + b*cos( 6.28318*(c*t+d) );
}
vec3 palette (float t) {
    return cosineColor( t, vec3(0.5,0.5,0.5),vec3(0.5,0.5,0.5),vec3(0.01,0.01,0.01),vec3(0.00, 0.15, 0.20) );
}

// distance estimator to a mandelbulb set
// returns the distance to the set on the x coordinate 
// and the color on the y coordinate
vec2 DE(vec3 pos) 
{
   float Power = 3.0+4.0*(sin(1/30.0)+1.0);
	vec3 z = pos;
	float dr = 1.0;
	float r = 0.0;
	for (int i = 0; i < MANDELBROTSTEPS ; i++) {
		r = length(z);
		if (r>MAXMANDELBROTDIST) break;
		
		// convert to polar coordinates
		float theta = acos(z.z/r);
		float phi = atan(z.y,z.x);
		dr =  pow( r, Power-1.0)*Power*dr + 1.0;
		
		// scale and rotate the point
		float zr = pow( r,Power);
		theta = theta*Power;
		phi = phi*Power;
		
		// convert back to cartesian coordinates
		z = zr*vec3(sin(theta)*cos(phi), sin(phi)*sin(theta), cos(theta));
		z+=pos;
	}
	return vec2(0.5*log(r)*r/dr,50.0*pow(dr,0.128/float(MARCHINGITERATIONS)));
}

// MAPPING FUNCTION ... 
// returns the distance of the nearest object in the direction p on the x coordinate 
// and the color on the y coordinate
vec2 map( in vec3 p )
{
    //p = fract(p); 
   	vec2 d = DE(p);

   	return d;
}


// TRACING A PATH : 
// measuring the distance to the nearest object on the x coordinate
// and returning the color index on the y coordinate
vec2 trace  (vec3 origin, vec3 ray) {
	
    //t is the point at which we are in the measuring of the distance
    float t =0.0;
    float c = 0.0;
    
    for (int i=0; i<MARCHINGITERATIONS; i++) {
    	vec3 path = origin + ray * t;	
    	vec2 dist = map(path);
    	// we want t to be as large as possible at each step but not too big to induce artifacts
        t += MARCHINGSTEP * dist.x;
        c += dist.y;
        if (dist.y < SMALLESTSTEP) break;
    }
    
    return vec2(t,c);
}

void main() 
{
   vec3 V = normalize(FragPos - cameraPos);
   vec3 N = normalize(Normal);

   //tracing the ray (getting the distance of the closest object in the ray direction)
	vec2 depth = trace(LocalPos - reflect(V, N)*5, reflect(V, N));
	
    //rendering with a fog calculation (further is darker)
	float fog = 1.0 / (1.0 + depth.x * depth.x * 0.01);
	
    //frag color
   vec3 fc = vec3(fog);
   //texture(texture_background, SampleSphericalMap(reflect(V, N))).xyz
   vec3 color = palette(depth.y)*fog;
   if(color.x+color.y+color.z<0.01f)
   {
      if(wireframe)
         fragColor = vec4(1, 0.682, 0, 1.0f);
      else
         fragColor = vec4(texture(texture_background, SampleSphericalMap(V)).xyz, 1.0f);
   }
   else
   {
      fragColor = vec4(color, 1.0f);
   }
}