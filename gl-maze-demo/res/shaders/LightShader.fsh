#shader fragment
#version 300 es

precision highp float;
    
out vec4 o_fragColor;

struct Light {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    float constant;
    float linear;
    float quadratic;
};

in vec4 fragPos;
in vec3 normalPos;
in vec2 texCoords;

uniform vec3 viewPosition;
uniform sampler2D texSampler;
uniform float shininess;
uniform Light light;

uniform bool useFog;

void main()
{
    // Ambient
    vec3 ambient = light.ambient * texture(texSampler, texCoords).rgb;
    
    // Diffuse
    vec3 norm = normalize(normalPos);
    vec3 lightDir = normalize(light.position - vec3(fragPos));
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * texture(texSampler, texCoords).rgb;
    
    // Specular
    vec3 viewDir = normalize(viewPosition - vec3(fragPos));
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    vec3 specular = light.specular * spec * texture(texSampler, texCoords).rgb;
    
    // Prevent shining through geometry
    if (spec < 0.0) {
        specular = vec3(0.0, 0.0, 0.0);
    }
    
    // Spotlight
    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = (light.cutOff - light.outerCutOff);
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    
    diffuse *= intensity;
    specular *= intensity;
    
    // Attenuation
    float distance = length(light.position - vec3(fragPos));
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    
    o_fragColor = vec4(ambient, 0.0) + vec4(diffuse, 0.0) + vec4(specular, 0.0);
    
    if (useFog) {
        // Fog effect added (linear)
        float fogDist = (gl_FragCoord.z / gl_FragCoord.w);
        vec4 fogColour = vec4(1.0, 1.0, 1.0, 1.0);
        float fogFactor = (10.0 - fogDist) / (10.0 - 1.0);
        fogFactor = clamp(fogFactor, 0.0, 1.0);
        o_fragColor = mix(fogColour, o_fragColor, fogFactor);
    }
    
    o_fragColor.a = 1.0;
}
