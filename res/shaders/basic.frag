#version 430 core

out vec4 FragColor;

in vec2 tex;
in vec3 normal;
in vec3 crntPos;

uniform sampler2D tex0;
uniform vec3 lightPos;
uniform vec3 camPos;

uniform bool dirEnabled;
uniform vec3 dirDir;

uniform mat4 lightModel;
uniform bool pointEnabled;

uniform vec3 spotPos[2];
uniform vec3 spotDir[2];
uniform bool spotEnabled[2];

vec4 spotLight(int i)
{
    // controls how big the area that is lit up is
    float outerCone = 0.90f;
    float innerCone = 0.95f;

    // ambient lighting
    float ambient = 0.20f;

    // diffuse lighting
    vec3 normalVec = normalize(normal);
    vec3 lightDirection = normalize(spotPos[i] - crntPos);
    float diffuse = max(dot(normalVec, lightDirection), 0.0f);

    // specular lighting
    float specular = 0.0f;
    if (diffuse != 0.0f)
    {
        float specularLight = 0.50f;
        vec3 viewDirection = normalize(camPos - crntPos);
        vec3 halfwayVec = normalize(viewDirection + lightDirection);
        float specAmount = pow(max(dot(normalVec, halfwayVec), 0.0f), 16.0);
        specular = specAmount * specularLight;
    };

    // calculates the intensity of the crntPos based on its angle to the center of the light cone
    float angle = dot(spotDir[i], -lightDirection);
    float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.0f, 1.0f);

    if(spotEnabled[i]) {
        return (texture(tex0, tex) * (diffuse * inten + specular * inten)) * vec4(1.0f, 1.0f, 1.0f, 1.0f);
    }
    else {
        return vec4(0.0f);
    }
}


vec4 pointLight()
{
    // used in two variables so I calculate it here to not have to do it twice
    vec3 lightPos2 = vec3(lightModel * vec4(lightPos, 1.0f));
    //lightPos = vec3(lightModel * vec4(lightPos, 1.0f));
    vec3 lightVec = lightPos2 - crntPos;

    // intensity of light with respect to distance
    float dist = length(lightVec);
    float a = 0.05;
    float b = 0.01;
    float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);

    // ambient lighting
    float ambient = 0.15f;

    // diffuse lighting
    vec3 normalVec = normalize(normal);
    vec3 lightDirection = normalize(lightVec);
    float diffuse = max(dot(normalVec, lightDirection), 0.0f);

    // specular lighting
    float specular = 0.0f;
    if (diffuse != 0.0f)
    {
        float specularLight = 0.50f;
        vec3 viewDirection = normalize(camPos - crntPos);
        vec3 halfwayVec = normalize(viewDirection + lightDirection);
        float specAmount = pow(max(dot(normal, halfwayVec), 0.0f), 16.0);
        specular = specAmount * specularLight;
    };
    if(pointEnabled) {
        return (texture(tex0, tex) * (diffuse * inten  + specular * inten)) * vec4(1.0f, 1.0f, 1.0f, 1.0f);
    }
    else {
        return vec4(0.0f);
    }
}


vec4 directLight()
{
    // ambient lighting
    float ambient = 0.15f;

    // diffuse lighting
    vec3 normal = normalize(normal);
    vec3 lightDirection = normalize(-dirDir);
    float diffuse = max(dot(normal, lightDirection), 0.0f);

    // specular lighting
    float specular = 0.0f;
    if (diffuse != 0.0f)
    {
        float specularLight = 0.50f;
        vec3 viewDirection = normalize(camPos - crntPos);
        vec3 halfwayVec = normalize(viewDirection + lightDirection);
        float specAmount = pow(max(dot(normal, halfwayVec), 0.0f), 16.0);
        specular = specAmount * specularLight;
    };

    //return (texture(tex0, texCoord) * (diffuse + ambient) + texture(tex1, texCoord).r * specular) * lightColor;
    if(dirEnabled) {
        return (texture(tex0, tex) * (diffuse + specular)) * vec4(1.0f, 1.0f, 1.0f, 1.0f);
    }
    else {
        return vec4(0.0f);
    }

}

void main()
{
    //FragColor = texture(tex0, tex);
    //FragColor = directLight() + pointLight() + spotLight() + vec4(0.2f) * texture(tex0, tex);
    FragColor = directLight() + pointLight() + spotLight(0) + spotLight(1) + vec4(0.2f) * texture(tex0, tex);
}