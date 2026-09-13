#Satyam vertex

#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aUV;
layout(location = 3) in vec4 aColor;

out vec2 UV;

void main()
{
	UV = aUV;
	gl_Position = vec4(aPos, 1.0);
}

#Satyam fragment
#version 330 core

// Newton Raphson - Trig
float sdEllipse( vec2 p, vec2 ab )
{
    // symmetry
    p = abs( p );
    
    // determine in/out and initial omega value
    bool s = dot(p/ab,p/ab)>1.0; // Equations of ellipse (x/a)^2 + (y/b)^2 = 1 -> = 1 point on ->  < 1 inside -> > 1 outside

    // EXTERIOR CASE
    // on a unit circle closest point to any point is that point normalized
    // Xnorm = px/a || Ynorm = py/b
    // theta = atan2(Ynorm, Xnorm)
    // multiplying both the arguments by the constant ab doesn't change the angle. Also swap in the valye of Xnorm and Ynorm
    // theta = atan(py * a, px & b)
    // What it does - its not the closest point on the ellipse but its the closest point after the space has been squashed into a circle and then mapped back. 

    float w = s ? atan(p.y*ab.x, p.x*ab.y) : 
                  ((ab.x*(p.x-ab.x)<ab.y*(p.y-ab.y))? 1.5707963 : 0.0);
    
    // find root with Newton solver
    for( int i=0; i<5; i++ )
    {
        vec2 cs = vec2(cos(w),sin(w));
        vec2 u = ab*vec2( cs.x,cs.y);
        vec2 v = ab*vec2(-cs.y,cs.x);
        w = w + dot(p-u,v)/(dot(p-u,u)+dot(v,v));
    }
    
    // compute final point and distance
    return length(p-ab*vec2(cos(w),sin(w))) * (s?1.0:-1.0);
}

// Newton Raphson - Rotation
float sdEllipse( vec2 p, vec2 ab )
{
    // symmetry
    p = abs( p );
    
    // initial value
    vec2 q = ab*(p-ab);
    vec2 cs = normalize( (q.x<q.y) ? vec2(0.01,1) : vec2(1,0.01) );
    
    // find root with Newton solver
    for( int i=0; i<5; i++ )
    {
        vec2 u = ab*vec2( cs.x,cs.y);
        vec2 v = ab*vec2(-cs.y,cs.x);
        float a = dot(p-u,v);
        float c = dot(p-u,u) + dot(v,v);
        float b = sqrt(c*c-a*a);
        cs = vec2( cs.x*b-cs.y*a, cs.y*b+cs.x*a )/c;
    }
    
    // compute final point and distance
    float d = length(p-ab*cs);
    
    // return signed distance
    return (dot(p/ab,p/ab)>1.0) ? d : -d;
}

in vec2 UV;

out vec4 FragColor;

uniform float _Time;

void main()
{
	vec2 centerUV = UV * 2.0 - 1.0;
    float sdfElip = sdEllipse(centerUV, vec2(0.8,sin(_Time * 0.5) * 0.5 + 0.5));
    sdfElip = 1.0 - smoothstep(-0.01, 0.01, sdfElip);
	FragColor = vec4(vec3(sdfElip), 1.0);
}