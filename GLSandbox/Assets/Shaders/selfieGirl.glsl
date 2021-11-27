#shader vertex
#version 330 core


layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

void main()
{
    TexCoords = aTexCoords;
    gl_Position = vec4(aPos * 10, 1.0);
}

#shader fragment
#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D uvTex;
uniform float iTime;




float ndot(vec2 a, vec2 b ) { return a.x*b.x - a.y*b.y; }

// la,lb=semi axis, h=height, ra=corner
float sdRhombus(vec3 p, float la, float lb, float h, float ra)
{
    p = abs(p);
    vec2 b = vec2(la,lb);
    float f = clamp( (ndot(b,b-2.0*p.xz))/dot(b,b), -1.0, 1.0 );
	vec2 q = vec2(length(p.xz-0.5*b*vec2(1.0-f,1.0+f))*sign(p.x*b.y+p.z*b.x-b.x*b.y)-ra, p.y-h);
    return min(max(q.x,q.y),0.0) + length(max(q,0.0));
}


float map( in vec3 pos )
{
    return sdRhombus(pos, 0.6, 0.2, 0.02, 0.02 ) - 0.01;
}


// http://iquilezles.org/www/articles/normalsSDF/normalsSDF.htm
vec3 calcNormal( in vec3 pos )
{
    vec2 e = vec2(1.0,-1.0)*0.5773;
    const float eps = 0.0005;
    return normalize( e.xyy*map( pos + e.xyy*eps ) + 
					  e.yyx*map( pos + e.yyx*eps ) + 
					  e.yxy*map( pos + e.yxy*eps ) + 
					  e.xxx*map( pos + e.xxx*eps ) );
}



    
#define AA 2
void main()
{             
   // vec2 uv = gl_FragCoord.xy / vec2(1200, 700);

    float an = 0.5*(iTime-10.0);
	vec3 ro = vec3( 1.0*cos(an), 0.4, 1.0*sin(an) );
    vec3 ta = vec3( 0.0, 0.0, 0.0 );
    // camera matrix
    vec3 ww = normalize( ta - ro );
    vec3 uu = normalize( cross(ww,vec3(0.0,1.0,0.0) ) );
    vec3 vv = normalize( cross(uu,ww));

    
    vec2 iResolution = vec2(1200, 700);
    vec3 tot = vec3(0.0);
    
    #if AA>1
    for( int m=0; m<AA; m++ )
    for( int n=0; n<AA; n++ )
    {
        
        // pixel coordinates
        vec2 o = vec2(float(m),float(n)) / float(AA) - 0.5;
        vec2 p = (-iResolution.xy + 2.0*(gl_FragCoord.xy +o))/iResolution.y;
        #else    
        vec2 p = (-iResolution.xy + 2.0*gl_FragCoord.xy)/iResolution.y;
        #endif

	    // create view ray
        vec3 rd = normalize( p.x*uu + p.y*vv + 1.5*ww );

        // raymarch
        const float tmax = 5.0;
        float t = 0.0;
        for( int i=0; i<256; i++ )
        {
            vec3 pos = ro +  t * rd;
            float h = map(pos);
            if( h<0.0001 || t>tmax ) break;
            t += h;
        }
        
    
        // shading/lighting	
        vec3 col = vec3(0.0);
        if( t<tmax )
        {
            vec3 pos = ro + t*rd;
            vec3 nor = calcNormal(pos);
            float dif = clamp( dot(nor,vec3(0.57703)), 0.0, 1.0 );
            float amb = 0.5 + 0.5*dot(nor,vec3(0.0,1.0,0.0));
            col = vec3(0.2,0.3,0.4)*amb + vec3(0.8,0.7,0.5)*dif;
        }

        // gamma        
        col = sqrt( col );
	    tot += col;
    #if AA>1
    }
    tot /= float(AA*AA);
    #endif

	FragColor = vec4( tot, 1.0 );
   
    //vec3 col = 0.5 + 0.5 * cos(iTime + uv.xyx + vec3(0,2,4));

    // Output to screen
    //FragColor = vec4(col,1.0);
   // FragColor = vec4(1.0);
}
