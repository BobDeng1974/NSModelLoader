#version 120

varying vec4 globalColor;

uniform float time;
uniform float amount;
uniform float height;


float random (vec2 st) {
    return fract(sin(dot(st.xy,vec2(12.9898,78.233)))*43758.5453123);
}

float noise (vec2 st) {
    vec2 i = floor(st);
    vec2 f = fract(st);
    
    
    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));
    
    vec2 u = f * f * (3.0 - 2.0 * f);
    
    return mix(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
}

#define OCTAVES 6
float fbm (vec2 st) {
    
    float value = 0.0;
    float amplitude = -amount / 100;
    float frequency = 0.;
    
    for (int i = 0; i < OCTAVES; i++) {
        value += amplitude * noise(st);
        st *= 2.;
        amplitude *= .5;
        
    }
    return value;
}

void main()
{
    
    gl_FrontColor = gl_Color; // using no color
    
    vec4 position = ftransform();
    
    float displacementY = position.x * sin(cos(time) + (position.z / amount )) * height;
    
    vec4 modifiedPosition = position;
    //modifiedPosition.y += displacementY;
    
    
    float noiseY = fbm(position.xy * time / amount) * height * 100;
    modifiedPosition.y += noiseY;
    
    gl_Position = modifiedPosition;
    
}

//float displacementY = sin(position.x + time) * cos(position.z / amount*100) * displacementHeight;
