#version 330 core

in Varyings {
    vec3 position;
    vec4 color;
    vec2 tex_coord;
    vec3 normal;
} fs_in;

out vec4 frag_color;

uniform int output_type;

void main(){
    // We select the fragment color based on output_type
    // 0 = Position, 1 = Color, 2 = Texture Coordinates, 3 = Normal
    switch(output_type){
        case 0:
        frag_color = vec4(0.5 * fs_in.position + 0.5, 1.0);
        break;

        case 1:
        frag_color = fs_in.color;
        break;
        
        case 2:
        frag_color = vec4(fs_in.tex_coord, 0.0, 1.0);
        break;
        
        default:
        frag_color = vec4(0.5 * fs_in.normal + 0.5, 1.0);
        break;
    }
}