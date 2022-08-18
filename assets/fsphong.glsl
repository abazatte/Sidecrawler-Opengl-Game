#version 400

const int MAX_LIGHTS = 14;
struct Light {
    int Type;
    vec3 Color;
    vec3 Position;
    vec3 Direction;
    vec3 Attenuation;
    vec3 SpotRadius;
    int ShadowIndex;
};

uniform Lights {
    int LightCount;
    Light lights[MAX_LIGHTS];
};

in vec3 Position;
in vec3 Normal;
in vec2 Texcoord;
// Aufgabe 3.3
in vec3 Tangent;
in vec3 Bitangent;

out vec4 FragColor;

uniform vec3 LightPos;
uniform vec3 LightColor;

uniform vec3 EyePos;
uniform vec3 DiffuseColor;
uniform vec3 SpecularColor;
uniform vec3 AmbientColor;
uniform float SpecularExp;
uniform sampler2D DiffuseTexture;
// neu...
uniform sampler2D NormalTexture;
uniform sampler2D ShadowMapTexture[MAX_LIGHTS];
uniform mat4 ShadowMapMat[MAX_LIGHTS];

// Vorwaertsdeklaration um Fehler zu Vermeiden
vec3 N, H;
float sat(in float a);
vec3 PointLight(in Light light);
vec3 SpotLight(in Light light);
float specularBlinn(in vec3 L);

// Spielerei: Nebel
float dmin = 0;
float dmax = 35;
float a = 1;
float d = length(vec3(Position.x, Position.y, Position.z) - EyePos - 10);
float n = pow(((d - dmin) / (dmax - dmin)), a);
float s = 0; // Kein Nebel
//float s = sat(n);  // mit Nebel
vec3 colorFog = vec3(0.1, 0.1, 0.1);
// Nebel: Ende


void main() {
    // Aufgabe 3.3
    mat3 NormalMat = mat3(Tangent, -Bitangent, Normal);
    vec4 NormTex = texture(NormalTexture, Texcoord);
    N.x = NormTex.r * 2 - 1;
    N.y = NormTex.g * 2 - 1;
    N.z = NormTex.b * 2 - 1;
    N = NormalMat * N;  // Weltraum
    N = normalize(N);

    // gegeben
    vec4 DiffTex = texture(DiffuseTexture, Texcoord);

    if (DiffTex.a < 0.3f) discard;
    vec3 N = normalize(Normal);
    vec3 L = normalize(LightPos - Position);
    vec3 E = normalize(EyePos - Position);
    vec3 R = reflect(-L, N);

    vec3 DiffuseComponent;
    vec3 SpecularComponent;
    vec3 LightColor;


    for (int i = 0; i < LightCount; i++) {
        Light l = lights[i];


        // Bonus Aufgabe Schatten
        vec4 PosSM = ShadowMapMat[i] * vec4(Position.xyz, 1);
        PosSM.xyz /= PosSM.w;  // Perspektivische Teilung
        PosSM.xy =
        PosSM.xy * 0.5 +
        0.5;  // Kooridnaten von norm. Bildraum [-1,1] in TextCord [0,1]
        vec4 DepthSM = texture(ShadowMapTexture[i], PosSM.xy);
        // Vergleiche ob DepthSM < PosSM.z ist, wenn ja, Fragment im Schatten,
        // sonst nicht

        if (PosSM.z > DepthSM.x) {  // einfach durchprobiert loool
            continue;
        }


        switch (l.Type) {
            case 0: {  // Point Light
                L = normalize(l.Position - Position);
                LightColor = l.Color;
                LightColor = PointLight(l);
                DiffuseComponent += LightColor * DiffuseColor * sat(dot(N, L));
                SpecularComponent +=
                LightColor * SpecularColor * specularBlinn(L);
                break;
            }

            case 1: {                         // Directional Light
                L = -normalize(l.Direction);  // - aus Formel
                LightColor = l.Color;
                DiffuseComponent += LightColor * DiffuseColor * sat(dot(N, L));
                SpecularComponent +=
                LightColor * SpecularColor * specularBlinn(L);
                break;
            }

            case 2: {  // Spot Light
                L = normalize(l.Position + Position);
                LightColor = SpotLight(l);
                DiffuseComponent += LightColor * DiffuseColor * sat(dot(N, L));
                SpecularComponent +=
                LightColor * SpecularColor * specularBlinn(L);
                break;
            }
        }
    }

    // Optional ding um zu gucken ob die Tangenten, Bitangenten Legit sind
    // FragColor = vec4(N, 1);


    FragColor =  (1 - s) * vec4((DiffuseComponent + AmbientColor) * DiffTex.rgb +SpecularComponent,DiffTex.a) + vec4(s * colorFog, 1);

}

vec3 PointLight(Light light) {
    float r = length(light.Position - Position);
    return light.Color / (light.Attenuation.x + light.Attenuation.y * r +
    light.Attenuation.z * r * r);
}

vec3 SpotLight(Light light) {
    vec3 L = -normalize(light.Position - Position);  // Spotlight umdrehen
    float ohm = acos(dot(L, normalize(light.Direction)));
    float innerPhi = light.SpotRadius.x;
    float outerPhi = light.SpotRadius.y;

    return (PointLight(light)) *
    (1 - sat((ohm - innerPhi) /
    (outerPhi - innerPhi)));  // Skalarprodukt aus Formel.
}

float specularBlinn(vec3 L) {
    vec3 E = normalize(EyePos - Position);
    vec3 H =
    normalize(E + L);  // Wurde von Aufgabe 1 gefixt. Statt 1/2, normieren!
    float angle = max(0.0, dot(N, H));
    return pow(angle, SpecularExp);
}

float sat(in float a) { return clamp(a, 0.0, 1.0); }