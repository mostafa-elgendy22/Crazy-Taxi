#version 330 core

out vec4 frag_color;

//TODO: Define uniforms for the slope and the intercept

uniform vec4 inside_color = vec4(1.0, 0.0, 0.0, 1.0);
uniform vec4 outside_color = vec4(0.0, 0.0, 0.0, 1.0);

void main(){
    //TODO: Write code that will draw the square
    if(gl_FragCoord.x <= 256){
        frag_color = inside_color;
    } else {
        frag_color = outside_color;
    }
}