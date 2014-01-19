#version 130

uniform sampler2D texture;
uniform vec4 distortionKs;
uniform float distortionScale;
uniform float lensCenter;

in vec2 f_viewportC;

out vec3 fragColor;


vec2 HmdWarp(vec2 linear) {
    float radiusSquared = linear.x * linear.x + linear.y * linear.y;
    vec2 rvector = linear * (
        distortionKs.x +
        distortionKs.y * radiusSquared +
        distortionKs.z * radiusSquared * radiusSquared +
        distortionKs.w * radiusSquared * radiusSquared * radiusSquared
    ) / distortionScale;
    return (rvector + 1.0) / 2.0;
}

void main() {
    vec2 lensC = f_viewportC - vec2(lensCenter, 0);
    vec2 texC = HmdWarp(f_viewportC);

    if(texC.x < 0 || texC.x > 1) {
        discard;
    }
    if(texC.y < 0 || texC.y > 1) {
        discard;
    }

    fragColor = texture2D(texture, texC);
}

