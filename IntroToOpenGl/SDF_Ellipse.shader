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

// Newton Raphson
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