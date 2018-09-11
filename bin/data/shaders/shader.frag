#version 120

uniform vec4 globalColor;


void main()
{
    //gl_FragColor = globalColor;
    gl_FragColor = gl_Color;
}
