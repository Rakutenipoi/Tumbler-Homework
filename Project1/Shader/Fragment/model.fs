#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 fragPos;
in vec3 normal;

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform sampler2D texture_diffuse1;
uniform vec3 viewPos;
uniform PointLight pointLight;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{    
    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - fragPos);

    vec3 result = CalcPointLight(pointLight, norm, fragPos, viewDir);

    // FragColor = texture(texture_diffuse1, TexCoords);
    FragColor = vec4(result, 1.0);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * vec3(1.0f, 0.0f, 0.0f) * 2;
    vec3 diffuse = light.diffuse * diff * vec3(1.0f, 0.0f, 0.0f);
    vec3 specular = light.specular * spec * vec3(1.0f, 0.0f, 0.0f);
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}