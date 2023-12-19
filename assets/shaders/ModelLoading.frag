#version 310 es
//from https://learnopengl.com/Lighting/Basic-Lighting
precision highp float;

struct Material {
   sampler2D diffuse;
    vec3 specular;
    float shininess;
}; 
  
struct Light {
     vec3 position;
  
     vec3 ambient;
     vec3 diffuse;
     vec3 specular;
};
in  vec2 TexCoords;
in  vec3 Normal;  
in  vec3 FragPos; 
in  vec3 Color; 

uniform Light light; 

uniform Material material;
uniform  vec3 viewPos;
uniform  vec3 lightPos;  

out vec4 FragColor;

void main()
{    
//    // ambient
//  
//    highp vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
//    
//     // diffuse 

//     float diff = max(dot(norm, lightDir), 0.0);
//     vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));  
//
//     
    vec3 textureColor = texture(material.diffuse, TexCoords).xyz;
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    //this should be shininess
    float spec = pow(max(dot(viewDir, reflectDir), 0.0),  material.shininess);
    // vec3 specular = light.specular * (spec * material.specular);   
    vec3 specular = light.specular * (spec * textureColor.r);   
         
    vec3 bake = Color * textureColor;

    
    FragColor = vec4(bake+specular, 1.0);
}
