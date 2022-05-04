#version 330

// The texture holding the scene pixels
uniform sampler2D tex;

// Read "assets/shaders/fullscreen.vert" to know what "tex_coord" holds;
in vec2 tex_coord;
out vec4 frag_color;

void main(){
    // To apply the grayscale effect, we compute the average of the red/blue/green channels
    // and set that average value to all the channels
    frag_color = texture(tex, tex_coord);
    float gray = dot(frag_color.rgb, vec3(1.0/3.0, 1.0/3.0, 1.0/3.0));
    frag_color.rgb = vec3(gray);
}