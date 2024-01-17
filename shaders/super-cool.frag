#version 150 core
// Parametrics by nimitz (twitter @stormoid)
// https://www.shadertoy.com/view/XdSSRw
// License Creative Commons Attribution-NonCommercial-ShareAlike 3.0 Unported License
// Contact the author for other licensing options

/*
	The problem with the drawing parametric curves in 2d (or 3d) can be seen as a mathematical
	optimization problem: Trying to find the closest distance between a point and a given
	parametric curve.

	The goal is then to find the global minimum of the distance between a pixel and said curve.
	The approach I am taking here is to stepping through the curve's parameter and trying to 
	find a coarse global minimum. Then I use the bisection method on the derivative of the
	distance to find the exact minimum.

	I am sure the main iterator can be optimized, or that my method could be improved.
	Let me know if you can help.
*/

uniform vec3    iResolution;
uniform float   iTime;
uniform float   iTimeDelta;
uniform int     iFrame;  

out vec4 oColour;

#define time iTime
#define pi 3.14159265

//might need to be adjusted depending on the curve
#define STRIDE 0.035

//for animations
float sfloor(in float x, in float w)
{
    float fx = floor(x);
    return fx+smoothstep(fx+w,fx+1.-w,x);
}

//parametric equation
vec2 f(in float t)
{   
    //Epitrochoid
    float cam= sfloor(time*0.4,.1)*pi*2.;
    const float R = 2.8;
    const float r = .4;
    float d = sin(time*0.21)*2.+2.6;
    float x = (R+r)*cos(t-cam)-d*cos((R+r)/r*t);
    float y = (R+r)*sin(t)-d*sin((R+r)/r*t);
    
    return vec2(x,y);
}

//squared distance from point (pixel) to curve
float fd(in vec2 p, in float t)
{
    p = p+f(t);
    return dot(p,p);
}


//root finding on the derivative
float bisect(in vec2 p, in float near, in float far)
{
    float mid = 0.;
    float mdrv = 0.;
    float ep = 0.02;
    for (int i = 0; i <= 5; i++)
    {
        mid = (near + far)*.5;
        mdrv = fd(p, mid+ep)-fd(p, mid-ep);
        if (abs(mdrv) < 0.001)break;
        if (mdrv > 0.) {far=mid;} else {near=mid;}
    }    
    //once the root is found. return the actual length (not squared)
    return length(p+ f((near + far)*.5));
}


//this function can most definitely be optimized
float intersect(in vec2 p, in float near, in float far)
{
    float t = near;
    float told = near;
    float nr = 0., fr = 1.;
    float mn = 10000.;
    
    for (int i = 0; i <= 120; i++)
    {
        float d = fd(p, t);
        if (d < mn)
        {
            mn = d;
            nr = told;
            fr = t+.05;
        }
        if (t > far)break;
        told = t;
        //t += log(d1+1.15)*.03;
        t += log(d+1.15)*STRIDE;
    }
    
   	return bisect(p, nr,fr);
}


//Reinhard based tone mapping
vec3 tone(vec3 color, float gamma)
{
	float white = 2.;
	float luma = dot(color, vec3(0.2126, 0.7152, 0.0722));
	float toneMappedLuma = luma * (1. + luma / (white*white)) / (1. + luma);
	color *= toneMappedLuma / luma;
	color = pow(color, vec3(1. / gamma));
	return color;
}

void main()
{
    vec2 fragCoord = gl_FragCoord.xy - 0.5;
	vec2 p = fragCoord.xy / iResolution.xy-0.5;
    vec2 bp = p  +0.5;
	p.x *= iResolution.x / iResolution.y;
    p *= 19.;
    float rz = intersect(p, 0., 30.);
    
    vec3 rgb = vec3(cos(iTime), cos(iTime + 2 * pi / 3), cos(iTime + 4 * pi / 3));
    rgb = (rgb + 1) / 2;

    rz = pow(rz * 50., .7);
    vec3 col = rgb / (rz + 1.5) * 9.;
    col = tone(col, 1.6);
    
    
    //vign from iq (very nice!)
	col *= pow(16.0*bp.x*bp.y*(1.0-bp.x)*(1.0-bp.y),.45);
    
	oColour = vec4(col,1.0);
}