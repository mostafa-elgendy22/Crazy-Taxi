#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 tex_coord;

out Varyings {
    vec4 color;
    vec2 tex_coord;
} vs_out;

uniform mat4 transform;

void main(){
    //TODO: (Req 6) Change the next line to apply the transformation matrix
    gl_Position = vec4(position, 1.0);
    vs_out.color = color;
    vs_out.tex_coord = tex_coord;
}