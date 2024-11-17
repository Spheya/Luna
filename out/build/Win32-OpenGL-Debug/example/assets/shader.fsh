#include <luna/forward.glsl>
#include <luna/core.glsl>

uniform sampler2D _MainTexture;

in vec4 color;
in vec2 uv;

in vec3 pos;
in vec3 normal;

out vec4 outColor;

void main() {
    vec3 n = normalize(normal);
    
    vec4 albedo = color * texture(_MainTexture, uv);
    albedo.rgb = pow(albedo.rgb, vec3(2.2));
    vec3 col = vec3(0.0);

    for(int i = 0; i < _NumLights; ++i) {
        Light light = _LightSources[i];
        vec3 toLight = normalize(light.position.xyz - pos * light.position.w);
        float diffuse = max(dot(toLight, n), 0.0);
        col += diffuse * light.color * albedo.rgb;
    }

    vec3 toCamera = normalize(_CameraPosition - pos);
    col += min(pow(vec3(1.0 - dot(toCamera, n)), vec3(5.0)), 1.0) * 0.02;
    col += 0.8 * albedo.rgb * (smoothstep(-1.0, 1.0, n.y) * 0.6 + 0.4);

    outColor = vec4(pow(col, vec3(1.0 / 2.2)), albedo.a);
}
