#version 410 core

struct Transform {
    mat4 M;
    mat4 V;
    mat4 P;
    
    mat4 S;  // tranform something to light space
} ;

struct Vertex {
    vec4 pos;
    vec3 normal;
    vec2 texCoord;
    
    vec4 shadowCoord; // in light
} ;

struct Light {
    vec4 color;
   	vec3 pos;
    float cutoff;
    vec3 direction;
    int type;
} ;

struct Material {
    float ambientIntensity;
    float specularIntensity;
    float shininess;
} ;

in Vertex vVertex;
in Transform vTrans;
in mat3 vTBN;

uniform int useShadow;
uniform int lightNum;
uniform Light light[10];

uniform Material mtl;

uniform float dispMapScale;
uniform float dispMapOffset;
uniform sampler2D normalMapSampler;
uniform sampler2D dispMapSampler;
uniform sampler2D defaultSampler;
uniform sampler2D shadowMapSampler;

out vec4 color;

float calcNormalShadowFactor(sampler2D shadowMap, vec2 coords, float compare) {
    // step(x,y)   retun 0 if y<x
    return step(compare-0.0005f, texture(shadowMap, coords).r);
}

float calcShadowMapLinear(sampler2D shadowMap, vec2 coords, float compare, vec2 texelSize)
{
    // [0 + 0.5, texelSize + 0.5]
    vec2 pixelPos = coords/texelSize + vec2(0.5);
    // Return x - floor(x)
    vec2 fracPart = fract(pixelPos);
    vec2 startTexel = (pixelPos - fracPart) * texelSize;
    
    
    float blTexel = calcNormalShadowFactor(shadowMap, startTexel, compare);
    float brTexel = calcNormalShadowFactor(shadowMap, startTexel + vec2(texelSize.x, 0.0), compare);
    float tlTexel = calcNormalShadowFactor(shadowMap, startTexel + vec2(0.0, texelSize.y), compare);
    float trTexel = calcNormalShadowFactor(shadowMap, startTexel + texelSize, compare);
    
    // mix(x,y,a)   return x*(1-a) + y*a
    float mixA = mix(blTexel, tlTexel, fracPart.y);
    float mixB = mix(brTexel, trTexel, fracPart.y);
    
    return mix(mixA, mixB, fracPart.x);
}

float calcPCFShadowFactor(sampler2D shadowMap, vec2 coords, float compare) {
    const float numSamples = 3.0f;
    const float sampleStart = (numSamples-1.0f)/2.0f;
    const float numSampleSquared = numSamples*numSamples;
    vec2 texelSize = 1.0f / textureSize(shadowMap, 0);
    float results = 0.0;
    for (float y = -sampleStart; y <= sampleStart ; y++) {
        for (float x = -sampleStart; x <= sampleStart; x++) {
            vec2 offsets = vec2(x, y) * texelSize;
            results += calcShadowMapLinear(shadowMap, coords + offsets, compare, texelSize);
        }
    }
    return results / numSampleSquared;
}

float linstep(float low, float high, float value) {
    return clamp((value-low) / (high-low), 0.0,1.0);
}

float calcVarianceShadow(sampler2D shadowMap, vec2 coords, float compare) {
    vec2 moments = texture(shadowMap, coords).xy;
    // 期望
    float p = step(compare, moments.x);
    // 方差
    float variance = max(moments.y - moments.x * moments.x, 0.00002);
    float d = compare - moments.x;
    // 概率上限
    float pMax = linstep(0.2, 1.0, variance / (variance + d*d));
    
    return min(max(p, pMax), 1.0);
}

vec4 calcPointLight(Light light, vec3 vertexPos, vec3 vertexNormal) {
    vec3 lightPos = (vTrans.V * vec4(light.pos, 1.0)).xyz;
    vec3 posToLightDirection = lightPos - vertexPos;  // pos to light source
    float lightDistance = length(posToLightDirection);
    posToLightDirection = posToLightDirection / lightDistance;
    
    float attenuation = 1.0 / (1.0 + 0.09*lightDistance+0.032*lightDistance*lightDistance);
    
    float diff = max(dot(vertexNormal, posToLightDirection), 0.0);
    
    // Calculate view vector (simply the negative of the view-space position)
    vec3 viewDirection = normalize(-vertexPos);
    vec3 halfVector = normalize(posToLightDirection + viewDirection);
    //vec3 reflectDirection = reflect(-lightDirection, normal);
    float spec = pow(max(dot(vertexNormal, halfVector), 0.0), mtl.shininess);

    vec4 diffuseColor = diff * light.color * attenuation;
    vec4 specularColor = mtl.specularIntensity * spec * light.color * attenuation;
    
    return (diffuseColor + specularColor);
}

vec4 calcSpotLight(Light light, vec3 vertexPos, vec3 vertexNormal) {
    vec3 lightPos = (vTrans.V * vec4(light.pos, 1.0)).xyz;
    vec3 posToLightDirection = lightPos - vertexPos;  // pos to light source
    float lightDistance = length(posToLightDirection);
    posToLightDirection = posToLightDirection / lightDistance;
    
    float attenuation = 1.0 / (1.0 + 0.09*lightDistance+0.032*lightDistance*lightDistance);
    // the angle spot cone direction with diection of pos to light source.
    float spotFactor = max(dot(light.direction, posToLightDirection), 0.0);
    float angle = acos(spotFactor);
    if (angle > light.cutoff) {
        attenuation = 0;
    }
    
    float diff = max(dot(vertexNormal, posToLightDirection), 0.0);
    
    // Calculate view vector (simply the negative of the view-space position)
    vec3 viewDirection = normalize(-vertexPos);
    vec3 halfVector = normalize(posToLightDirection + viewDirection);
    //vec3 reflectDirection = reflect(-lightDirection, normal);
    float spec = pow(max(dot(vertexNormal, halfVector), 0.0), mtl.shininess);
    
    vec4 diffuseColor = diff * light.color * attenuation;
    vec4 specularColor = mtl.specularIntensity * spec * light.color * attenuation;
    
    // [x, 1.0] map to [0, 1.0]
    // d = 1.0 / 1.0-x;
    // f(Xcurr) = 1.0 - (1.0-Xcurr) * d
    return (diffuseColor + specularColor)* (1.0 - (1.0 - spotFactor) * 1.0/(1.0 - cos(light.cutoff)));
}

vec4 calcLight(Light light, vec3 vertexPos, vec3 vertexNormal) {
    // shadow factor
    float shadowFactor = 1;
    if (useShadow == 1) {
        vec3 proj = (vVertex.shadowCoord.xyz / vVertex.shadowCoord.w) * vec3(0.5f) + vec3(0.5f);
//        shadowFactor = calcPCFShadowFactor(shadowMapSampler, proj.xy, proj.z);
        shadowFactor = calcVarianceShadow(shadowMapSampler, proj.xy, proj.z);
    }
    
    vec4 result = vec4(0);
    if (light.type == 0) { // Point light
        result = calcPointLight(light, vertexPos, vertexNormal) * shadowFactor;
    } else if (light.type == 1) { // Spot light
        result = calcSpotLight(light, vertexPos, vertexNormal) * shadowFactor;
    } else  {  // Direction light
        
    }
    
    vec4 ambientColor = mtl.ambientIntensity * vec4(1.0);
    
    return min((ambientColor+ result), vec4(1.0));
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
    
    // Tranform the normal in tangent space to camera space.
    vec3 normal = normalize(vTBN * (2*texture(normalMapSampler, texCoord).xyz-1));
    
    vec4 texColor = texture(defaultSampler, texCoord);
    
    vec4 lightColor = vec4(0);
    for (int i=0; i<lightNum; ++i) {
        lightColor +=  calcLight(light[i], vVertex.pos.xyz, normal);
    }
    
    color = texColor * lightColor;
}
