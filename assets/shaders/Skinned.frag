#version 310 es
precision  highp float;
//from https://learnopengl.com/Lighting/Basic-Lighting

in vec2 TexCoord0;
in vec3 Normal0;

struct Material {
   sampler2D diffuse;
   highp vec3 specular;
   highp float shininess;
}; 
  
uniform Material material;
struct Light {
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light; 

out vec4 FragColor;

uniform highp vec3 lightPos;  

in highp vec3 FragPos; 

uniform highp vec3 viewPos;

void main()
{    
    // ambient

  
    highp vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoord0));
    
     // diffuse 
    highp vec3 norm = normalize(Normal0);
    highp vec3 lightDir = normalize(lightPos - FragPos);
    highp float diff = max(dot(norm, lightDir), 0.0);
    highp vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoord0));  

     
     // specular
    highp vec3 viewDir = normalize(viewPos - FragPos);
    highp vec3 reflectDir = reflect(-lightDir, norm);  
    highp float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    highp vec3 specular = light.specular * (spec * material.specular);   
         
    highp vec3 result = ambient + diffuse + specular;
   
  
     FragColor = vec4(result,1.0);
}
