#version 120

varying vec4 globalColor;

uniform float time;
uniform float amount;
uniform float height;


vec2 random2( vec2 p ) {
    return fract(sin(vec2(dot(p,vec2(127.1,311.7)),dot(p,vec2(269.5,183.3))))*43758.5453);
}

#define ANIMATE
vec3 voronoi( in vec2 x, float rnd ) {
    vec2 n = floor(x);
    vec2 f = fract(x);
    
    // first pass: regular voronoi
    vec2 mg, mr;
    float md = 8.0;
    for (int j=-1; j<=1; j++ ) {
        for (int i=-1; i<=1; i++ ) {
            vec2 g = vec2(float(i),float(j));
            vec2 o = random2( n + g )*rnd;
#ifdef ANIMATE
            o = 0.5 + 0.5*sin( time + 6.2831 * o );
#endif
            vec2 r = g + o - f;
            float d = dot(r,r);
            
            if( d<md ) {
                md = d;
                mr = r;
                mg = g;
            }
        }
    }
    
    // second pass: distance to borders
    md = 8.0;
    for (int j=-2; j<=2; j++ ) {
        for (int i=-2; i<=2; i++ ) {
            vec2 g = mg + vec2(float(i),float(j));
            vec2 o = random2(n + g)*rnd;
#ifdef ANIMATE
            o = 0.5 + 0.5*sin(time + 6.2831*o );
#endif
            vec2 r = g + o - f;
            
            if( dot(mr-r,mr-r)>0.00001 )
                md = min( md, dot( 0.5*(mr+r), normalize(r-mr) ) );
        }
    }
    return vec3( md, mr );
}


void main()
{
    gl_FrontColor = gl_Color; // using no color
    
    vec4 position = ftransform();
    
    vec4 modifiedPosition = position;
    //.5
   
    /*
    vec2 res = vec2(1920,1080);
    vec2 st = position.xy/res.xy;
    st = (st-height)*.75+0.5;
     */
    
    //float d = dot(st-height,st-height);
    //height 20.0
    //vec3 c = voronoi( 20. * st, pow(d,.4) );
    
    // borders
    
    //modifiedPosition = vec4(smoothstep( 0.01, 0.02, c ),1.0);
    //modifiedPosition.z = c.z
    
    //float displacementY = position.x * sin(cos(sin(time / amount / 100 * position.z / amount))) * height;
    float displacementY = position.x * voronoi(position.xy * amount / 1000, time).x * height;
    modifiedPosition.y += displacementY;
    
    gl_Position = modifiedPosition;
    
}


