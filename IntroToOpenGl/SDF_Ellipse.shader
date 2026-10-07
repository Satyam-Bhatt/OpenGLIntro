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
    // theta = atan(py * a, px * b)
    // What it does - its not the closest point on the ellipse but its the closest point after the space has been squashed into a circle and then mapped back. 

    // INTERIOR CASE
    // So basically we draw a straight line with a slope a/b and then compare it with our slope (p.y - b)/(p.x - a) and then check where does the point lie and snap to the relevant location - 0 or PI/2 = 1.5707963
    // a/b < (p.y - b)/(p.x - a)
    // a(p.x - a) < b(p.y - b)  
    // a*p.x - a^2 < b*p.y - b^2
    // a*p.x - b*p.y < a^2 - b^2
    // if p.x = a and p.y = b then we get
    // a^2 - b^2 = a^2 - b^2 -> Hence proving that this line passes through a and b both
    float w = s ? atan(p.y*ab.x, p.x*ab.y) : 
                  ((ab.x*(p.x-ab.x)<ab.y*(p.y-ab.y))? 1.5707963 : 0.0);
    
    // Newton Raphson Method
    // Formula - Xn+1 = Xn - (f(Xn) / f'(Xn)')
    // Its an iterative method to approximate roots of a polynomial. Here we know that the shortest distance to point from the circumference is perpendicular to the slope at that point. 
    // If we get f(w) to be 0 then the second term becomes 0 and w stops changing
    // u = a*cos(w), b*sin(w) gives us a point on the ellipse.
    // v = -a*sin(w), b*cos(w) is the derivative. This gives us the slope at that point. The shortest distance would be perpendicular to this tangent
    // Xn = Current guess of roots
    // Xn+1 = Next root
    // f(Xn): The value of the function at your current guess
    // f'(Xn): The derivative (slope) of the function at your current guess
    // So here we have the point and we are trying to find the w value where the distance would be the shortest. So what we want is that the dot product of the perpendicular from point p and tangent of the elipse to be 0. We can get the value of w from it but it would form a polynomial and would be hard to compute plus we would have more than 1 root. In Newton Raphson We
    // -> Compute f(w) = dot(p-u, v) at the current guess
    // -> Compute f'(w) = -(dot(p-u,u) + dot(v,v)) at the current guess (Vector derivative)
    // -> Move w by -f(w)/f'(w)
    // Derivation done
    for( int i=0; i<5; i++ )
    {
        // Direction
        vec2 cs = vec2(cos(w),sin(w));
        // Point on the elipse in that Direction
        vec2 u = ab*vec2( cs.x,cs.y);
        // Derivative of the point to get its tangent
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