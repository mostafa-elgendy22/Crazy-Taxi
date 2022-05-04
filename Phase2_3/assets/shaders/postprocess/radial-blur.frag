#version 330

// The texture holding the scene pixels
uniform sampler2D tex;

// Read "assets/shaders/fullscreen.vert" to know what "tex_coord" holds;
in vec2 tex_coord;
out vec4 frag_color;

// The number of samples we read to compute the blurring effect
#define STEPS 16
// The strength of the blurring effect
#define STRENGTH 0.2

void main(){
    // To apply radial blur, we compute the direction outward from the center to the current pixel
    vec2 step_vector = (tex_coord - 0.5) * (STRENGTH / STEPS);
    // Then we sample multiple pixels along that direction and compute the average
    for(int i = 0; i < STEPS; i++){
        frag_color += texture(tex, tex_coord + step_vector * i);    
    }
    frag_color /= STEPS;
}