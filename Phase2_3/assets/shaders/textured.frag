#version 330 core

in Varyings {
    vec4 color;
    vec2 tex_coord;
} fs_in;

out vec4 frag_color;

uniform vec4 tint;
uniform sampler2D tex;

void main(){
    //TODO: (Req 6) Modify the following line to compute the fragment color
    // by multiplying the tint with the vertex color and with the texture color 
    frag_color = texture(tex, fs_in.tex_coord) * tint * fs_in.color;// msh fahem 7aga - where is glEnableVertexAttribArray(where does color come from????) - why multiply all of this?
}