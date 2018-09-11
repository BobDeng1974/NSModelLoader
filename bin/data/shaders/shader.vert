#version 120

varying vec4 globalColor;

uniform float time;
uniform float amount;
uniform float height;

void main()
{
    
    gl_FrontColor = gl_Color; // using no color
    
    vec4 position = ftransform();
    
    float displacementHeight = height;
    float displacementY = position.x * sin(cos(time) + (position.z / amount )) * displacementHeight;
    
    
    vec4 modifiedPosition = position;
    modifiedPosition.y += displacementY;
    
    gl_Position = modifiedPosition;
    
}

//float displacementY = sin(position.x + time) * cos(position.z / amount*100) * displacementHeight;
