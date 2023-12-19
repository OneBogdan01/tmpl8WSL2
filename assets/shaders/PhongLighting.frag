#version 310 es
//from https://learnopengl.com/Lighting/Basic-Lighting


struct Material {
   sampler2D diffuse;
   highp vec3 specular;
   highp float shininess;
}; 
  
uniform Material material;
struct Light {
    highp vec3 position;
  
    highp vec3 ambient;
    highp vec3 diffuse;
    highp vec3 specular;
};





uniform Light light; 

out highp vec4 FragColor;

in highp vec2 TexCoords;

uniform sampler2D texture_diffuse1;

in highp vec3 Normal;  
uniform highp vec3 lightPos;  

in highp vec3 FragPos; 

uniform highp vec3 viewPos;
void main()
{    
    // ambient
  
    highp vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    
     // diffuse 
    highp vec3 norm = normalize(Normal);
    highp vec3 lightDir = normalize(lightPos - FragPos);
    highp float diff = max(dot(norm, lightDir), 0.0);
    highp vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));  

     
     // specular
    highp vec3 viewDir = normalize(viewPos - FragPos);
    highp vec3 reflectDir = reflect(-lightDir, norm);  
    highp float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    highp vec3 specular = light.specular * (spec * material.specular);   
         
    highp vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}