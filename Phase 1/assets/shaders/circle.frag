#version 330 core

out vec4 frag_color;

//TODO: Define uniforms for the center and the radius
uniform vec2 center;
uniform float radius;

uniform vec4 inside_color = vec4(1.0, 0.0, 0.0, 1.0);
uniform vec4 outside_color = vec4(0.0, 0.0, 0.0, 1.0);

void main(){
    //TODO: Write code that will draw the circle
    if((pow(gl_FragCoord.y-center.y,2.0)+pow(gl_FragCoord.x-center.x,2.0)) <= pow(radius,2.0)){
        frag_color = inside_color;
    } else {
        frag_color = outside_color;
    }
}