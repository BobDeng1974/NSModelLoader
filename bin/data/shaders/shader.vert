#version 120

varying vec4 globalColor;

// the time value is passed into the shader by the OF app.
uniform float time;
uniform float amount;

void main()
{
    vec4 position = ftransform();
    
    
    float displacementHeight = 150;
    float displacementY = position.x * sin(cos(time) + (position.z / amount )) * displacementHeight;
    
    //vec4 modifiedPosition = modelViewProjectionMatrix * position; ///GL3
    
    
    vec4 modifiedPosition = position;
    modifiedPosition.y += displacementY;
    
    gl_Position = modifiedPosition;
    
    globalColor = gl_Color;
}
