#version 330 core
// @author: yahiaetman
// @from: https://github.com/yahiaetman/OpenGL-Examples/blob/master/assets/shaders/ex27_postprocessing/fog.frag
in vec2 tex_coord;

// This will be used to sample a color from the off-screen framebuffer on which we draw our scene.
uniform sampler2D tex;
// We will also use the scene depth to determine the amount of fog to apply.
uniform sampler2D depth_sampler;

// We will use the inverse projection to get the fragment coordinate in the camera (view/eye) space.
uniform mat4 inverse_projection;

out vec4 frag_color;

void main() {
    // This determines how much the fog affect the scene (0: no fog, 1: background will no longer be visible). Range [0,1]
    float fog_power = 0.7;
    // The fog exponent is 1/D where D is the distance at which the fog color will dominate 63.2% of the output color. This used to affect the density of the fog.
    // Since D ranges from 0 to infinity, fog exponent ranges from infinity (nothing will show up) to 0 (no fog).
    float fog_exponent = 1.0/30.0;
    // vec3 fog_color = vec3(0.75, 0.5, 0.25);
    vec3 fog_color = vec3(0.25, 0.5, 0.75);
    float depth = texture(depth_sampler, tex_coord).r; // First we sample the pixel depth (value ranges from 0 to 1).
    vec4 ndc_position = vec4(2.0f * tex_coord - 1.0f, 2.0f * depth - 1.0f, 1.0f); // Convert pixel from screenspace to NDC.
    vec4 view_position = inverse_projection * ndc_position; // transform from NDC to camera (view/eye) space.
    float distance = length(view_position.xyz / view_position.w); // Normalize w then calculate distance in view space.
    float fog_mix_factor = fog_power * (1.0f - exp(- distance * fog_exponent)); // Calculate the fog factor using the exponential fog formula.
    vec4 color = texture(tex, tex_coord); // Sample the scene color. This could be further optimized to use "texelFetch".
    color.rgb = mix(color.rgb, fog_color, fog_mix_factor); // Mix the fog and the scene color.
    frag_color = color; // Output the result.
}