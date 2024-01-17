#version 310 es
//from https://learnopengl.com/Lighting/Basic-Lighting
precision highp float;


struct Material {
    vec3 specular;
    float shininess;
}; 
  struct DirLight {
    vec3 direction;
  
//we add this in the baking phase
//    vec3 ambient;
//    vec3 diffuse;
    vec3 specular;
};  
uniform DirLight dirLight;
struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
#define NR_POINT_LIGHTS 4  
uniform PointLight pointLights[NR_POINT_LIGHTS];

//in vec3 Normal;  
//in vec3 FragPos; 
in vec3 Color; 


uniform Material material;
uniform vec3 viewPos;
uniform vec3 lightPos;  

out vec4 FragColor;

//vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);  
//vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);  
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
//     	// diffuse
//			glm::vec3 norm = glm::normalize(vertex.Normal);
//			glm::vec3 lightDir = glm::normalize(Game::GetLightPos() - vertex.Position);
//			float diff = max(glm::dot(norm, lightDir), 0.0f);
//			//float distance = length(Game::GetLightPos() - (vertex.Position + worldPos));
//			//for point lights that are static
//			/*float distance = length(Game::GetLightPos() - (vertex.Position + worldPos));
//			float attenuation = 1.0 / (1.0 + 0.09 * distance + 0.032 * (distance * distance));*/
//			glm::vec3 diffuse = glm::vec3(1.0f) * diff;
//
//			vertex.Color = ambient + diffuse;
// specular
    // vec3 reflectDir = reflect(-lightDir, norm);
    // //this should be shininess
    // float spec = pow(max(dot(viewDir, reflectDir), 0.0),  material.shininess);
    // // vec3 specular = light.specular * (spec * material.specular);   
    // vec3 specular = light.specular * (spec * textureColor.r);   
      

//vec3 norm = normalize(Normal);
//    vec3 viewDir = normalize(viewPos - FragPos);
//
//    vec3 result=CalcDirLight(dirLight, norm, viewDir);
//    for(int i = 0; i < NR_POINT_LIGHTS; i++)
//        result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);    
//
//
// 
//    vec3 bake = Color ;

    
    FragColor = vec4(Color, 1.0);
}
//vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
//{
//    vec3 lightDir = normalize(light.position - fragPos);
//    // diffuse shading
//    float diff = max(dot(normal, lightDir), 0.0);
//    // specular shading
//    vec3 reflectDir = reflect(-lightDir, normal);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//    // attenuation
//    float distance    = length(light.position - fragPos);
//    float attenuation = 1.0 / (light.constant + light.linear * distance + 
//  			     light.quadratic * (distance * distance));    
//    // combine results
//    vec3 ambient  = light.ambient  * Color;
//    vec3 diffuse  = light.diffuse  * diff * Color;
//    vec3 specular = light.specular * spec * Color.r; //vec3(texture(material.specular, TexCoords)); this is advice from Remi
//    ambient  *= attenuation;
//    diffuse  *= attenuation;
//    specular *= attenuation;
//
//    return (ambient + diffuse);
//} 
////diffuse and ambient already calculated on bake
//vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
//{
//    vec3 lightDir = normalize(-light.direction);
//    // diffuse shading
//     float diff = max(dot(normal, lightDir), 0.0);
//    // specular shading
//    vec3 reflectDir = reflect(-lightDir, normal);
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//    // combine results
//     vec3 ambient  =  vec3(0.1f,0.1f,0.1f)  * Color;
//     vec3 diffuse  = vec3(1.0f, 0.71f, 0.75f)  * diff * Color;
//    vec3 specular = light.specular * spec * Color.r;
//    return (ambient+diffuse);
//}  