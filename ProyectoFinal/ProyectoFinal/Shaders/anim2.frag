#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
uniform sampler2D texture1;

void main()
{
 
  vec4 texColor= texture(texture1,TexCoords)*vec4(0.28f,0.28f,0.28f,1.0f);
    if(texColor.a < 0.1)
        discard;
    FragColor = texColor;

}