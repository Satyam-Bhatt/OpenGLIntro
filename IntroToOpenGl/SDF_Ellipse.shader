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
    // theta = atan2(py * a, px * b)
    // What it does - its not the closest point on the ellipse but its the closest point after the space has been squashed into a circle and then mapped back. 

    // INTERIOR CASE
    // a*(p.x-a) < b*(p.y-b)
   //  Left < right (true): x is further from a than y is from b, in the weighted sense. The point is far to the left and relatively high, so the top of the ellipse is closest, and the seed is π/2.
    // Left ≥ right (false): y is further from b than x is from a. The point is far down and relatively far right, so the right end is closest, and the seed is 0.
    // So basically we draw a straight line with a slope a/b and then compare it with our slope (p.y - b)/(p.x - a) and then check where does the point lie and snap to the relevant location - 0 or PI/2 = 1.5707963
    // a/b < (p.y - b)/(p.x - a)
    // a(p.x - a) < b(p.y - b)  
    // a*p.x - a^2 < b*p.y - b^2
    // a*p.x - b*p.y < a^2 - b^2
    // if p.x = a and p.y = b then we get
    // a^2 - b^2 = a^2 - b^2
    // true  -> above the line -> seed at PI/2 (near the b-vertex)
    // false -> below the line -> seed at 0    (near the a-vertex)
    // ANOTHER APPROACH
    // If you take the terms to one side and compute a value like this
    // g(p) = a(p.x - a) - b(p.y - b)
    // Its basically a dot product with a,-b - normal to the line
    // and sign of dot porduct tells if its behind or in the front of the normal
    // We also have direction vector (b,a). wedge product with that gives us if the point is to the left or right
    float w = s ? atan(p.y*ab.x, p.x*ab.y) : 
                  ((ab.x*(p.x-ab.x)<ab.y*(p.y-ab.y))? 1.5707963 : 0.0);
    
    // Newton Raphson Method
    // Formula - Xn+1 = Xn - (f(Xn) / f'(Xn))
    // Its an iterative method to approximate roots of a polynomial. It just finds one root depending on the seed. Here we know that the shortest distance to point from the circumference is perpendicular to the slope at that point. 
    // If we get f(w) to be 0 then the second term becomes 0 and w stops changing
    // u = a*cos(w), b*sin(w) gives us a point on the ellipse.
    // v = -a*sin(w), b*cos(w) is the derivative. This gives us the tangent at that point. The shortest distance would be perpendicular to this tangent
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
        // This is the Newton Raphson equation.
        // We take the function -> dot(p-u, v)
        // and derivative of that function -> (dot(p-u),u)+dot(v,v)
        // and shift the w accordingly. Now how did we derive it
        // p is a point in space that we need to find minimum closest distance from the circumference of the elipse
        // u is a point on the elipse at angle w
        // p-u vector is the direction from u to p and to minimize the distance we want it to be exactly on the line of the vector from origin to p and also want it to be perpendicular to the tangent. If its not we move the w
        // v vectors is the tangent of the point u which is basically a derivative of u
        // and p-u should be perpendicular to v and at the w where both are perpendicular we get the shortest distance
        // perpendicular means dot product to be 0
        // dot(p-u,v) = 0 and that is what we are finding w for.
        // As we are using newton Raphson we need the derivative of dot(p-u, v)
        // Using product rule d/dw [A·B] = A'·B + A·B' 
        // f'(w) = dot(p-u,v') + dot((p-u)',v))
        // Left side -> u' = v || v' = u'' || u = a cos(w), b sin(w) || u' = -a sin(w), b cos(w) || u'' = - a cos(w), -b sin(w) || u'' = -u --> dot(p-u, -u)
        // Right sidde -> (p-u)' || p is constant so 0 || -u' = -v --> -dot(v,v)
        // Hence we get -dot(p-u,u)-dot(v,v) 
        // as newton Raphson has negative in the equation it becomes plus as we have 2 negative sign and we take their common
        w = w + dot(p-u,v)/(dot(p-u,u)+dot(v,v));
    }
    
    // compute final point and distance
    // After we get the w this means that we are almost close to the point where the distance is minimum. So we compute the distance
    // We verify if the point is in or outside the elipse using s and then we change the sign accordingly so that distance starts from negative - goes to 0 and then positive
    return length(p-ab*vec2(cos(w),sin(w))) * (s?1.0:-1.0);
}

// Newton Raphson - Rotation
// Here we track the unit vector cs = cos w, sin w -> which removes the sin, cos and atan call
float sdEllipse( vec2 p, vec2 ab )
{
    // symmetry
    p = abs( p );
    
    // Similar to what we did above for interior case
    // a*(p.x-a) < b*(p.y-b)
    // As per which is greater it picks up the seed - either 0.01,1 if x is further away from a than y is from b. and vice versa if its the other way around
    // This time we don't care about if its inside or outside
    vec2 q = ab*(p-ab);
    vec2 cs = normalize( (q.x<q.y) ? vec2(0.01,1) : vec2(1,0.01) );
    
    // find root with Newton solver
    // 
    for( int i=0; i<5; i++ )
    {
        // u is the general point lying on the elipse as its being multiplied by elipse radii
        vec2 u = ab*vec2( cs.x,cs.y);
        // v is perpendicular to u and is a derivative of It
        vec2 v = ab*vec2(-cs.y,cs.x);
        // The function we want to equate to 0 (dot product)
        float a = dot(p-u,v); // sind
        // derivative of the function above derivied in the method above
        float c = dot(p-u,u) + dot(v,v); // common division factor
        // Here we don't calculate the angle but rotate the vector. 2D vector rotation is by multiplying by 2D matrix of sin cos which comes out to be
        // Matrix = [cosd -sind]
        //          [sind  cosd]
        // cs = (cs.x cosd - cs.y sind, cs.y cosd + cs.x sind)
        // Now we need the sind and cosd without trignometry 
        // as Newton Raphson works on small delta we can assume that sind = d when d is small
        // the value of d = a/c as newton raphson tells that w = w0 + f(x)/f'(x). So delta is basically f(x)/f'(x) which in our case is a/c
        // we will find cos using sin^2 d + cos^2 d = 1 and we get cos d = sqrt(1 - sin^2 d)
        // replacing sin^2 d with a/c gives us => cos d = sqrt(c^2 - a^2)/c hence the value of b (we removed the c as c is common and we put it back in cs)
        float b = sqrt(c*c-a*a); // cosd
        // as sind is a and cosd is b (c is common so we divide later) we replace it in the matrix
        // Matrix = [cosd -sind]
        //          [sind  cosd]
        // which makes it
        // Matrix = [b -a]
        //          [a  b]
        // multiply it with cs vector to get the result below and divide by common divisor c
        cs = vec2( cs.x*b-cs.y*a, cs.y*b+cs.x*a )/c;
    }
    
    // compute final point and distance
    // Simiar as above
    float d = length(p-ab*cs);
    
    // return signed distance
    // Similar as above method
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