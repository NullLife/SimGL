#version 410 core

struct Transform {
    mat4 M;
    mat4 V;
    mat4 P;
} ;

struct Vertex {
    vec4 pos;
    vec3 normal;
    vec2 texCoord;
} ;

struct Light {
    vec4 color;
   	vec3 pos;
} ;

struct Material {
    float ambientIntensity;
    float specularIntensity;
    float shininess;
} ;

in Vertex vVertex;
in Transform vTrans;
in mat3 vTBN;

uniform Light light;
uniform Material mtl;
uniform float dispMapScale;
uniform float dispMapOffset;
uniform sampler2D normalMapSampler;
uniform sampler2D dispMapSampler;
uniform sampler2D defaultSampler;

out vec4 color;


vec4 calcLight(Light light, vec3 vertexPos, vec3 vertexNormal) {
    vec4 ambientLight = mtl.ambientIntensity * light.color;

    vec3 lightPos = (vTrans.V * vec4(light.pos, 1.0)).xyz;
    vec3 lightDirection = lightPos - vertexPos;
    
    float lightDistance = length(lightDirection);
    lightDirection = lightDirection / lightDistance;
    
    float attenuation = 1.0 / (1.0 + 0.09*lightDistance+0.032*lightDistance*lightDistance);
    
    float diff = max(dot(vertexNormal, lightDirection), 0.0);
    vec4 diffuseLight = diff * light.color * attenuation;
    
    // Calculate view vector (simply the negative of the view-space position)
    vec3 viewDirection = normalize(-vertexPos);
    
    vec3 halfVector = normalize(lightDirection + viewDirection);
    //vec3 reflectDirection = reflect(-lightDirection, normal);
    float spec = pow(max(dot(vertexNormal, halfVector), 0.0), mtl.shininess);
    vec4 specularLight = mtl.specularIntensity * spec * light.color * attenuation;
    
    return min((ambientLight + diffuseLight + specularLight), vec4(1.0));
}

vec2 calcParallaxMapOffset(Vertex vertex) {
    vec3 directionToEye = normalize(-vertex.pos.xyz);
    float height = texture(dispMapSampler, vertex.texCoord).r;
    height = height * dispMapScale + dispMapOffset;
    return vertex.texCoord + (vTBN * directionToEye).xy * height;
}

vec2 calcPreciseParallaxMap(Vertex vertex) {
    vec3 directionToEye = normalize(-vertex.pos.xyz);
    
    float numLayers = 8.0f;
    float layerHeight = 1.0f / numLayers;
    vec2 dtex = dispMapScale * directionToEye.xy/directionToEye.z / numLayers;
    
    float currentLayerHeight = 0;
    vec2 currentTexCoord = vertex.texCoord;
    
    float height = texture(dispMapSampler, currentTexCoord).r;
    
    while (height > currentLayerHeight) {
        currentLayerHeight += layerHeight;
        currentTexCoord += dtex;
        height = texture(dispMapSampler, currentTexCoord).r;
    }
    
    return currentTexCoord;
}

void main() {
    vec2 texCoord = calcPreciseParallaxMap(vVertex);
//    texCoord = vVertex.texCoord;
    
    // tranform the normal in tangent space to camera space.
    vec3 normal = normalize(vTBN * (2*texture(normalMapSampler, texCoord).xyz-1));
    
    vec4 texColor = texture(defaultSampler, texCoord);
    
    color = texColor * calcLight(light, vVertex.pos.xyz, normal);
}
