#version 330 core
out vec4 FragColor;

struct Material {
    sampler2D diffuseMap;
    sampler2D specularMap; 
    bool useDiffuseMap;
    bool useSpecularMap;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
}; 

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{    
    // diffuse 
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(light.position - FragPos);
        float diff = max(dot(norm, lightDir), 0.0); 
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 ambient = light.ambient * material.diffuse * 2;
        vec3 diffuse =  light.diffuse * diff * material.diffuse;
        vec3 specular = light.specular * spec * material.specular;
        
    if(material.useDiffuseMap == true)
    {
        ambient = light.ambient * texture(material.diffuseMap, TexCoord).rgb;
        diffuse = light.diffuse * diff * texture(material.diffuseMap, TexCoord).rgb;  
    }

    // specular
    if(material.useSpecularMap == true)
    {
        specular = light.specular * spec * texture(material.specularMap, TexCoord).rgb; 
    }
        
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}