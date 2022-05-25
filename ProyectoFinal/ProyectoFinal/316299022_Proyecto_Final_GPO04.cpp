#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"

// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Texture.h"

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();
void animacion();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.0f, 3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
glm::vec3 PosIni(-95.0f, 0.0f, -45.0f);
bool active;
bool active2;
bool flag = false;
bool flag2 = false;
bool flag3 = false;
bool anim = false;
bool anim2 = false;
bool anim3 = false;
float rot = -90.0f;
float rot2 = 0.0f;
float rot3 = -90.0f;
float i;
float j;
float k;
float l;
float tiempo;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(-0.2f, 7.7f, 0.0f), //Lampara 1
    glm::vec3(-0.2f, 7.7f, -8.0f), //Lampara2
    glm::vec3(-6.35f, 9.518f, 12.35f), //Letra A
    glm::vec3(-6.45f, 10.6f, 12.35f), //Letra I
    glm::vec3(-6.45f, 11.725f, 12.35f), //Letra H
    glm::vec3(0.0f, 9.518f, 12.3f),
    glm::vec3(0.0f, 10.6f, 12.3f),
    glm::vec3(0.0f, 11.725f, 12.3f),
    glm::vec3(6.35f, 9.518f, 12.3f),
    glm::vec3(6.45f, 10.6f, 12.3f),
    glm::vec3(6.45f, 11.725f, 12.3f),
    glm::vec3(10.2f, 5.0f, -11.70f), //Letrero (Staff only)
    glm::vec3(-8.9f, 5.5f, -16.2f), //Refrigerador 1
    glm::vec3(-8.9f, 5.5f, -20.2f), //Refrigerador 2
    glm::vec3(10.5f, 0.2f, -20.5f), //Congelador (contenedor 1)
    glm::vec3(10.5f, 0.2f, -18.5f), //Congelador (contendor 2)
    glm::vec3(0.0f, 7.8f, -18.5f) //Lampara (bodega)
};

glm::vec3 spotLightPosition[] = {
    glm::vec3(0.0f, 5.0f, 0.0f)
};

float vertices[] = {
     -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
       -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
       -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

       -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
       -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
       -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

       -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
       -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
       -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
       -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
       -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
       -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

       -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
       -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

       -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
       -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
       -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};
//Vectores para modificar el color de la luz de los pointlights
glm::vec3 Light1 = glm::vec3(0);
glm::vec3 Light2 = glm::vec3(0);
glm::vec3 Light3 = glm::vec3(0);
glm::vec3 Light4 = glm::vec3(1);

//Animación botellas
float movKitX = 0.0;
float movKitY = 0.0;
float rotKit = 0.0;
bool caida = false;
bool levantar = false;
bool recorrido1 = true;
bool recorrido2 = false;
bool recorrido3 = true;
bool recorrido4 = false;

// Deltatime
GLfloat deltaTime = 0.0f;   // Time between current frame and last frame
GLfloat lastFrame = 0.0f;   // Time of last frame

int main()
{
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    /*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "316299022_PROYECTOFINAL_GPO04", nullptr, nullptr); //Titulo del programa

    if (nullptr == window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();

        return EXIT_FAILURE;
    }

    glfwMakeContextCurrent(window);

    glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

    // Set the required callback functions
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);

    // GLFW Options
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (GLEW_OK != glewInit())
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }

    // Define the viewport dimensions
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    //Carga de los shaders (vertexshader y fragmentshader)
    Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag"); //Shader de iluminacion
    Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag"); //Shader sencillo
    Shader Anim("Shaders/anim2.vs", "Shaders/anim2.frag"); //Shader de animacion
    Shader SkyBoxshader("Shaders/SkyBox.vs", "Shaders/SkyBox.frag"); //Shader del SkyBox
    
    Model Floor((char*)"Models/Bar/Piso.obj"); //Piso del bar
    Model Floor_W((char*)"Models/Bodega/Piso.obj"); //Piso de la bodega
    Model Roof1((char*)"Models/Bar/Techo.obj"); //Techo del bar (parte cubica)
    Model Roof2((char*)"Models/Bar/Techo2.obj"); //Techo del bar (parte cilindrica)
    Model Roof_W((char*)"Models/Bodega/Techo.obj"); //Techo de la bodega
    Model Lamp1((char*)"Models/Lampara/Lampara1.obj"); //Lampara 1 (bar)
    Model Lamp2((char*)"Models/Lampara/Lampara2.obj"); //Lampara 2 (bar)
    Model Lamp3((char*)"Models/Lampara/Lampara_bodega.obj"); //Lampara (bodega)
    Model Wall1((char*)"Models/Bar/Pared_izq.obj"); //Pared izquierda del bar
    Model Wall2((char*)"Models/Bar/Pared_trasera2.obj"); //Pared trasera del bar
    Model Wall3((char*)"Models/Bar/Pared_der.obj"); //Pared derecha del bar
    Model Wall4((char*)"Models/Bar/Pared_frontal.obj"); //Pared frontal del bar
    Model Wall1_W((char*)"Models/Bodega/Pared_izq.obj"); //Pared izquierda bodega
    Model Wall2_W((char*)"Models/Bodega/Pared_trasera.obj"); //Pared trasera bodega
    Model Wall3_W((char*)"Models/Bodega/Pared_der.obj"); //Pared derecha bodega
    Model Bush1((char*)"Models/Bar/Arbusto.obj"); //Arbusto1 (decoracion)
    Model Bush2((char*)"Models/Bar/Arbusto2.obj"); //Arbusto2 (decoracion)
    Model Window1((char*)"Models/Bar/Ventana_izq.obj"); //Ventana izquierda
    Model Window2((char*)"Models/Bar/Ventana_der.obj"); //Ventana derecha
    Model Door((char*)"Models/Bar/Puerta.obj"); //Puerta de cristal (entrada)
    Model Door_W((char*)"Models/Bar/Puerta.obj"); //Puerta de cristal (bodega)
    Model Title1((char*)"Models/Bar/Titulo.obj"); //Fachada frontal del bar
    Model Title2((char*)"Models/Bar/Titulo2.obj"); //Titulo del bar (The Nim Bar)
    Model Sign((char*)"Models/Bodega/Letrero.obj"); //Letrero bodega (Staff only)
    Model Light_S((char*)"Models/Bodega/Luz_letrero.obj"); //Luz letrero
    Model Table((char*)"Models/Mesa/table1.obj"); //Mesa
    Model Chair((char*)"Models/Silla/chair2.obj"); //Silla
    Model Bench((char*)"Models/Banco/barstool3.obj"); //Banco
    Model Counter1((char*)"Models/Barra/barra1.obj"); //Parte mas larga de la barra
    Model Counter2((char*)"Models/Barra/barra2.obj"); //Parte mas corta de la barra
    Model Counter3((char*)"Models/Barra/puerta_barra.obj"); //Puerta de la barra
    Model Counter4((char*)"Models/Barra/techo_barra.obj"); //Techo de la barra
    Model Bottle1((char*)"Models/Bottles/Beer_bottle/beer2.obj"); //Botella de cerveza
    Model Bottle2((char*)"Models/Bottles/Whiskey_bottle/whisky.obj"); //Botella de whisky
    Model Bottle3((char*)"Models/Bottles/Wine_bottle/wine.obj"); //Botella de vino
    Model Rack((char*)"Models/Estante/estante.obj"); //Estante de licores (bar)
    Model Rack_W((char*)"Models/Estante/estante_bodega.obj"); //Estante de licores (bodega)
    Model Fridge((char*)"Models/Refrigerador/Refrigerador.obj"); //Refrigerador
    Model Door_F((char*)"Models/Refrigerador/Puerta_refri.obj"); //Perta del refrigerador
    Model Frame_F((char*)"Models/Refrigerador/Marco_refri.obj"); //Marco de la puerta del refrigerador
    Model Barrel((char*)"Models/Barril/Barril.obj"); //Barril
    Model Freezer((char*)"Models/Congelador/Congelador.obj"); //Congelador
    Model Door_Freezer((char*)"Models/Congelador/Puertas_congelador.obj"); //Puertas congelador

    GLfloat skyboxVertices[] = { //Vertices para construir el SkyBox
        // Positions
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
    };

    // First, set the container's VAO (and VBO)
    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //SkyBox
    GLuint skyboxVBO, skyboxVAO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

    // Load textures
    vector<const GLchar*> faces;
    faces.push_back("SkyBox/right.tga");
    faces.push_back("SkyBox/left.tga");
    faces.push_back("SkyBox/top.tga");
    faces.push_back("SkyBox/bottom.tga");
    faces.push_back("SkyBox/back.tga");
    faces.push_back("SkyBox/front.tga");

    GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);

    // Set texture units
    lightingShader.Use();
    glUniform1i(glGetUniformLocation(lightingShader.Program, "material.diffuse"), 0);
    glUniform1i(glGetUniformLocation(lightingShader.Program, "material.specular"),1);

    glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);

    // Game loop
    while (!glfwWindowShouldClose(window))
    {

        // Calculate deltatime of current frame
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();
        DoMovement();
        animacion();

        // Clear the colorbuffer
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
       
        // OpenGL options
        glEnable(GL_DEPTH_TEST);

        //Load Model
    
        // Use cooresponding shader when setting uniforms/drawing objects
        lightingShader.Use();
        GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
        glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);

        // Directional light
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), 0.0f, -1.0f, 0.0f); //Direccion hacia la que apunta la luz
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"),0.1f,0.1f,0.1f); //Valor de la componente ambiental
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.1f, 0.1f, 0.1f); //Valor de la componente difusa
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"),1.0f, 1.0f, 1.0f); //Valor de la componente especular

        // Point light 1
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z); //Indica la posicion (x,y,z) del pointlight
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), Light1.x, Light1.y, Light1.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), Light1.x, Light1.y, Light1.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.09f); //Las variables lineal y cuadratica indican
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"),0.032f); //el rango de cuadros que ilumina la luz

        // Point light 2
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].position"), pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].ambient"), Light1.x, Light1.y, Light1.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].diffuse"), Light1.x, Light1.y, Light1.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[1].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].linear"), 0.09f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[1].quadratic"), 0.032f);

        // Point light 3
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), Light2.x, Light2.y, Light2.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), Light2.x, Light2.y, Light2.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.22f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.20f);

        // Point light 4
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), Light2.x, Light2.y, Light2.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), Light2.x, Light2.y, Light2.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.22f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.20f);

        // Point light 5
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].position"), pointLightPositions[4].x, pointLightPositions[4].y, pointLightPositions[4].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].ambient"), Light2.x, Light2.y, Light2.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].diffuse"), Light2.x, Light2.y, Light2.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[4].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[4].linear"), 0.22f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[4].quadratic"), 0.20f);

        // Point light 6
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[5].position"), pointLightPositions[5].x, pointLightPositions[5].y, pointLightPositions[5].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[5].ambient"), Light2.x, Light2.y, Light2.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[5].diffuse"), Light2.x, Light2.y, Light2.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[5].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[5].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[5].linear"), 0.22f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[5].quadratic"), 0.20f);

        // Point light 7
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[6].position"), pointLightPositions[6].x, pointLightPositions[6].y, pointLightPositions[6].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[6].ambient"), Light2.x, Light2.y, Light2.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[6].diffuse"), Light2.x, Light2.y, Light2.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[6].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[6].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[6].linear"), 0.22f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[6].quadratic"), 0.20f);

        // Point light 8
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[7].position"), pointLightPositions[7].x, pointLightPositions[7].y, pointLightPositions[7].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[7].ambient"), Light2.x, Light2.y, Light2.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[7].diffuse"), Light2.x, Light2.y, Light2.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[7].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[7].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[7].linear"), 0.22f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[7].quadratic"), 0.20f);

        // Point light 9
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[8].position"), pointLightPositions[8].x, pointLightPositions[8].y, pointLightPositions[8].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[8].ambient"), Light2.x, Light2.y, Light2.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[8].diffuse"), Light2.x, Light2.y, Light2.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[8].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[8].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[8].linear"), 0.22f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[8].quadratic"), 0.20f);

        // Point light 10
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[9].position"), pointLightPositions[9].x, pointLightPositions[9].y, pointLightPositions[9].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[9].ambient"), Light2.x, Light2.y, Light2.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[9].diffuse"), Light2.x, Light2.y, Light2.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[9].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[9].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[9].linear"), 0.22f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[9].quadratic"), 0.20f);

        // Point light 11
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[10].position"), pointLightPositions[10].x, pointLightPositions[10].y, pointLightPositions[10].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[10].ambient"), Light2.x, Light2.y, Light2.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[10].diffuse"), Light2.x, Light2.y, Light2.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[10].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[10].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[10].linear"), 0.22f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[10].quadratic"), 0.20f);

        // Point light 12
        glm::vec3 lightColor2; //Permite variar el color del pointlight en funcion del tiempo (con una funcion seno)
        lightColor2.x = abs(sin(glfwGetTime() * Light3.x));
        lightColor2.y = abs(sin(glfwGetTime() * Light3.y));
        lightColor2.z = sin(glfwGetTime() * Light3.z);

        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[11].position"), pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[11].ambient"), lightColor2.x, lightColor2.y, lightColor2.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[11].diffuse"), lightColor2.x, lightColor2.y, lightColor2.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[11].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[11].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[11].linear"), 0.22f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[11].quadratic"), 0.20f);

        // Point light 13
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[12].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[12].ambient"), lightColor2.x, lightColor2.y, lightColor2.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[12].diffuse"), lightColor2.x, lightColor2.y, lightColor2.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[12].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[12].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[12].linear"), 0.22f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[12].quadratic"), 0.20f);

        // Point light 14
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[13].position"), pointLightPositions[4].x, pointLightPositions[4].y, pointLightPositions[4].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[13].ambient"), lightColor2.x, lightColor2.y, lightColor2.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[13].diffuse"), lightColor2.x, lightColor2.y, lightColor2.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[13].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[13].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[13].linear"), 0.22f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[13].quadratic"), 0.20f);

        // Point light 15
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[14].position"), pointLightPositions[5].x, pointLightPositions[5].y, pointLightPositions[5].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[14].ambient"), lightColor2.x, lightColor2.y, lightColor2.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[14].diffuse"), lightColor2.x, lightColor2.y, lightColor2.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[14].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[14].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[14].linear"), 0.22f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[14].quadratic"), 0.20f);

        // Point light 16
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[15].position"), pointLightPositions[6].x, pointLightPositions[6].y, pointLightPositions[6].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[15].ambient"), lightColor2.x, lightColor2.y, lightColor2.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[15].diffuse"), lightColor2.x, lightColor2.y, lightColor2.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[15].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[15].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[15].linear"), 0.22f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[15].quadratic"), 0.20f);

        // Point light 17
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[16].position"), pointLightPositions[7].x, pointLightPositions[7].y, pointLightPositions[7].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[16].ambient"), lightColor2.x, lightColor2.y, lightColor2.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[16].diffuse"), lightColor2.x, lightColor2.y, lightColor2.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[16].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[16].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[16].linear"), 0.22f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[16].quadratic"), 0.20f);

        // Point light 18
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[17].position"), pointLightPositions[8].x, pointLightPositions[8].y, pointLightPositions[8].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[17].ambient"), lightColor2.x, lightColor2.y, lightColor2.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[17].diffuse"), lightColor2.x, lightColor2.y, lightColor2.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[17].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[17].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[17].linear"), 0.22f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[17].quadratic"), 0.20f);

        // Point light 19
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[18].position"), pointLightPositions[9].x, pointLightPositions[9].y, pointLightPositions[9].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[18].ambient"), lightColor2.x, lightColor2.y, lightColor2.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[18].diffuse"), lightColor2.x, lightColor2.y, lightColor2.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[18].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[18].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[18].linear"), 0.22f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[18].quadratic"), 0.20f);

        // Point light 20
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[19].position"), pointLightPositions[10].x, pointLightPositions[10].y, pointLightPositions[10].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[19].ambient"), lightColor2.x, lightColor2.y, lightColor2.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[19].diffuse"), lightColor2.x, lightColor2.y, lightColor2.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[19].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[19].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[19].linear"), 0.22f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[19].quadratic"), 0.20f);

        // Point light 21
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[20].position"), pointLightPositions[11].x, pointLightPositions[11].y, pointLightPositions[11].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[20].ambient"), Light1.x, Light1.y, Light1.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[20].diffuse"), Light1.x, Light1.y, Light1.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[20].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[20].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[20].linear"), 0.22f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[20].quadratic"), 0.20f);

        // Point light 22
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[21].position"), pointLightPositions[12].x, pointLightPositions[12].y, pointLightPositions[12].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[21].ambient"), Light4.x, Light4.y, Light4.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[21].diffuse"), Light4.x, Light4.y, Light4.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[21].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[21].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[21].linear"), 0.35f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[21].quadratic"), 0.44f);

        // Point light 23
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[22].position"), pointLightPositions[13].x, pointLightPositions[13].y, pointLightPositions[13].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[22].ambient"), Light4.x, Light4.y, Light4.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[22].diffuse"), Light4.x, Light4.y, Light4.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[22].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[22].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[22].linear"), 0.35f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[22].quadratic"), 0.44f);

        // Point light 24
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[23].position"), pointLightPositions[14].x, pointLightPositions[14].y, pointLightPositions[14].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[23].ambient"), Light4.x, Light4.y, Light4.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[23].diffuse"), Light4.x, Light4.y, Light4.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[23].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[23].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[23].linear"), 0.35f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[23].quadratic"), 0.44f);

        // Point light 25
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[24].position"), pointLightPositions[15].x, pointLightPositions[15].y, pointLightPositions[15].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[24].ambient"), Light4.x, Light4.y, Light4.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[24].diffuse"), Light4.x, Light4.y, Light4.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[24].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[24].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[24].linear"), 0.35f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[24].quadratic"), 0.44f);

        // Point light 26
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[25].position"), pointLightPositions[16].x, pointLightPositions[16].y, pointLightPositions[16].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[25].ambient"), Light1.x, Light1.y, Light1.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[25].diffuse"), Light1.x, Light1.y, Light1.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[25].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[25].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[25].linear"), 0.09f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[25].quadratic"), 0.032f);

        // SpotLight
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), spotLightPosition[0].x, spotLightPosition[0].y, spotLightPosition[0].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.0f, 0.0f, 0.0f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"),1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.07f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.017f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(125.0f)));
        glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(150.0f)));

        // Set material properties
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 64.0f); //Indica el brillo de la textura de los objetos

        // Create camera transformations
        glm::mat4 view;
        view = camera.GetViewMatrix();

        // Get the uniform locations
        GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model"); //Se obtiene la matriz modelo desde el shader
        GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view"); //Se obtiene la matriz vista desde el shader
        GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection"); //Se obtiene la matriz proyeccion desde el shader

        // Pass the matrices to the shader
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glm::mat4 model(1);
        //Carga de modelo 
        view = camera.GetViewMatrix();
        //PISO Y TECHO (BAR Y BODEGA)
        model = glm::mat4(1); //Setea la matriz en el origen del mundo
        model = glm::translate(model, glm::vec3(0.2f, -0.2f, 0.4f)); //Traslada el objeto
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        Floor.Draw(lightingShader); //Dibuja el objeto especificado
        model = glm::mat4(1); //Setea la matriz en el origen del mundo
        model = glm::translate(model, glm::vec3(-0.1f, -0.2f, 0.0f)); //Traslada el objeto
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Rota el objeto
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        Floor_W.Draw(lightingShader); //Dibuja el objeto especificado
        model = glm::mat4(1); //Setea la matriz en el origen del mundo
        model = glm::translate(model, glm::vec3(-0.16f, -0.2f, 0.0f)); //Traslada el objeto
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Rota el objeto
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        Roof1.Draw(lightingShader);  //Dibuja el objeto especificado
        model = glm::mat4(1); //Setea la matriz en el origen del mundo
        model = glm::translate(model, glm::vec3(-0.2f, -0.2f, 0.0f)); //Traslada el objeto
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Rota el objeto
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        Roof2.Draw(lightingShader);  //Dibuja el objeto especificado
        model = glm::mat4(1); //Setea la matriz en el origen del mundo
        model = glm::translate(model, glm::vec3(-0.16f, -0.2f, 0.0f)); //Traslada el objeto
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Rota el objeto
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        Roof_W.Draw(lightingShader);  //Dibuja el objeto especificado
        //PAREDES (BAR Y BODEGA)
        model = glm::mat4(1); //Setea la matriz en el origen del mundo
        model = glm::translate(model, glm::vec3(0.0f, -0.2f, 0.0f)); //Traslada el objeto
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Rota el objeto
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        Wall1.Draw(lightingShader);  //Dibuja el objeto especificado
        model = glm::mat4(1); //Setea la matriz en el origen del mundo
        model = glm::translate(model, glm::vec3(0.0f, -0.2f, 0.0f)); //Traslada el objeto
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Rota el objeto
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        Wall2.Draw(lightingShader);  //Dibuja el objeto especificado
        model = glm::mat4(1); //Setea la matriz modelo en el origen del mundo
        model = glm::translate(model, glm::vec3(-0.2f, -0.2f, 0.0f)); //Traslada el objeto
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Rota el objeto
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        Wall3.Draw(lightingShader);  //Dibuja el objeto especificado
        model = glm::mat4(1); //Setea la matriz en el origen del mundo
        model = glm::translate(model, glm::vec3(0.0f, -0.2f, 0.0f)); //Traslada el objeto
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Rota el objeto
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        Wall4.Draw(lightingShader);  //Dibuja el objeto especificado
        model = glm::mat4(1); //Setea la matriz en el origen del mundo
        model = glm::translate(model, glm::vec3(0.0f, -0.2f, 0.0f)); //Traslada el objeto
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Rota el objeto
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        Title1.Draw(lightingShader);  //Dibuja el objeto especificado
        model = glm::mat4(1); //Setea la matriz en el origen del mundo
        model = glm::translate(model, glm::vec3(0.0f, -0.2f, 0.0f)); //Traslada el objeto
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Rota el objeto
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        Title2.Draw(lightingShader);  //Dibuja el objeto especificado
        model = glm::mat4(1); //Setea la matriz en el origen del mundo
        model = glm::translate(model, glm::vec3(0.0f, -0.2f, 0.0f)); //Traslada el objeto
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Rota el objeto
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        Wall1_W.Draw(lightingShader);  //Dibuja el objeto especificado
        model = glm::mat4(1); //Setea la matriz en el origen del mundo
        model = glm::translate(model, glm::vec3(0.0f, -0.2f, 0.0f)); //Traslada el objeto
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Rota el objeto
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        Wall2_W.Draw(lightingShader);  //Dibuja el objeto especificado
        model = glm::mat4(1); //Setea la matriz modelo en el origen del mundo
        model = glm::translate(model, glm::vec3(-0.2f, -0.2f, 0.0f)); //Traslada el objeto
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Rota el objeto
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        Wall3_W.Draw(lightingShader);  //Dibuja el objeto especificado
        model = glm::mat4(1); //Setea la matriz en el origen del mundo
        model = glm::translate(model, glm::vec3(0.0f, -0.25f, 0.0f)); //Traslada el objeto
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Rota el objeto
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        Sign.Draw(lightingShader); //Dibuja el objeto especificado
        model = glm::mat4(1); //Setea la matriz en el origen del mundo
        model = glm::translate(model, glm::vec3(0.0f, -0.25f, 0.0f)); //Traslada el objeto
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Rota el objeto
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        Light_S.Draw(lightingShader); //Dibuja el objeto especificado
        //COLUMNAS DE MESAS Y SILLAS
        for (j = -8.0f;j <= 4.0f;j += 4.0f) { //Se usa un ciclo for para dibujar varias copias del mismo objeto pero posiciones distintas
            for (i = -2.8f;i >= -10.0f;i -= 1.2f) { //Se usa un ciclo for para dibujar varias copias del mismo objeto pero posiciones distintas
                //MESAS
                model = glm::mat4(1); //Setea la matriz en el origen del mundo
                model = glm::translate(model, glm::vec3(i, 0.0f, j)); //Traslada el objeto
                model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f)); //Escala el objeto
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
                Table.Draw(lightingShader); //Dibuja el objeto especificado
                //SILLAS_ENFRENTE
                model = glm::mat4(1); //Setea la matriz en el origen del mundo
                model = glm::translate(model, glm::vec3(i, 0.0f, j+1)); //Traslada el objeto
                model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Rota el objeto
                model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f)); //Escala el objeto
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
                Chair.Draw(lightingShader); //Dibuja el objeto especificado
                //SILLAS_ATRAS
                model = glm::mat4(1); //Setea la matriz en el origen del mundo
                model = glm::translate(model, glm::vec3(i, 0.0f, j-1)); //Traslada el objeto
                model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Rota el objeto
                model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f)); //Escala el objeto
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
                Chair.Draw(lightingShader); //Dibuja el objeto especificado
            }
        }
        //BANCOS
        for (i = -10.4f;i <= 6.0f;i += 1.2f) { //Se usa un ciclo for para dibujar varias copias del mismo objeto pero posiciones distintas
            model = glm::mat4(1); //Setea la matriz en el origen del mundo
            model = glm::translate(model, glm::vec3(4.0f, 0.0f, i)); //Traslada el objeto
            model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Rota el objeto
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f)); //Escala el objeto
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
            Bench.Draw(lightingShader); //Dibuja el objeto especificado
        }
        //BARRA
        model = glm::mat4(1); //Setea la matriz en el origen del mundo
        model = glm::translate(model, glm::vec3(4.0f, 0.0f, -2.65f)); //Traslada el objeto
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Rota el objeto
        model = glm::scale(model, glm::vec3(0.715f, 0.5f, 0.5f)); //Escala el objeto
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        Counter1.Draw(lightingShader); //Dibuja el objeto especificado
        model = glm::mat4(1); //Setea la matriz en el origen del mundo
        model = glm::translate(model, glm::vec3(4.0f, 0.0f, 0.0f)); //Traslada el objeto
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Rota el objeto
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f)); //Escala el objeto
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        Counter2.Draw(lightingShader); //Dibuja el objeto especificado
        model = glm::mat4(1); //Setea la matriz en el origen del mundo
        model = glm::translate(model, glm::vec3(9.5f, 1.95f, 5.5f)); //Traslada el objeto
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Rota el objeto sobre el eje Y
        model = glm::rotate(model, glm::radians(rot2), glm::vec3(1.0f, 0.0f, 0.0f)); //Rota el objeto sobre el eje X
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.69f)); //Escala el objeto
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        Counter3.Draw(lightingShader); //Dibuja el objeto especificado
        model = glm::mat4(1); //Setea la matriz en el origen del mundo
        model = glm::translate(model, glm::vec3(4.2f, 1.55f, -3.7f)); //Traslada el objeto
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Rota el objeto
        model = glm::scale(model, glm::vec3(0.68f, 0.5f, 0.5f)); //Escala el objeto
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        Counter4.Draw(lightingShader); //Dibuja el objeto especificado
        //ESTANTES
        model = glm::mat4(1); //Setea la matriz en el origen del mundo
        model = glm::translate(model, glm::vec3(4.4f, 1.55f, -3.0f)); //Traslada el objeto
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Rota el objeto
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f)); //Escala el objeto
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        Rack.Draw(lightingShader); //Dibuja el objeto especificado (ESTANTE BAR)
        model = glm::mat4(1); //Setea la matriz en el origen del mundo
        model = glm::translate(model, glm::vec3(0.0f, -0.2f, -12.5f)); //Traslada el objeto
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Rota el objeto
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f)); //Escala el objeto
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        Rack_W.Draw(lightingShader); //Dibuja el objeto especificado (ESTANTE BODEGA)
        //REFRIGERADORES
        model = glm::mat4(1); //Setea la matriz en el origen del mundo
        model = glm::translate(model, glm::vec3(-5.5f, -0.15f, -10.5f)); //Traslada el objeto
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Rota el objeto
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f)); //Escala el objeto
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        Fridge.Draw(lightingShader); //Dibuja el objeto especificado
        model = glm::mat4(1); //Setea la matriz en el origen del mundo
        model = glm::translate(model, glm::vec3(-8.9f, 1.525f, -20.2f)); //Traslada el objeto
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Rota el objeto
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f)); //Escala el objeto
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        Frame_F.Draw(lightingShader); //Dibuja el objeto especificado
        model = glm::mat4(1); //Setea la matriz en el origen del mundo
        model = glm::translate(model, glm::vec3(-5.5f, -0.15f, -6.5f)); //Traslada el objeto
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Rota el objeto
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f)); //Escala el objeto
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        Fridge.Draw(lightingShader); //Dibuja el objeto especificado
        model = glm::mat4(1); //Setea la matriz en el origen del mundo
        model = glm::translate(model, glm::vec3(-8.9f, 1.525f, -16.2f)); //Traslada el objeto
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Rota el objeto
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f)); //Escala el objeto
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        Frame_F.Draw(lightingShader); //Dibuja el objeto especificado
        //CONGELADOR
        model = glm::mat4(1); //Setea la matriz en el origen del mundo
        model = glm::translate(model, glm::vec3(0.0f, -0.2f, 0.0f)); //Traslada el objeto
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Rota el objeto
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        Freezer.Draw(lightingShader); //Dibuja el objeto especificado
        //LAMPARA BODEGA
        model = glm::mat4(1); //Setea la matriz en el origen del mundo
        model = glm::translate(model, glm::vec3(-0.2f, -0.2f, 0.0f)); //Traslada el objeto
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Rota el objeto
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        Lamp3.Draw(lightingShader); //Dibuja el objeto especificado
        //BARRILES
        k = 2.0f;
        l = 12.0f;
        for (j = -0.2f;j <= 9.0f;j += 2.3f) { //Se usa un ciclo for para dibujar varias copias del mismo objeto pero posiciones distintas
            for (i = k;i <= l;i += 2.5f) { //Se usa un ciclo for para dibujar varias copias del mismo objeto pero posiciones distintas
                model = glm::mat4(1); //Setea la matriz en el origen del mundo
                model = glm::translate(model, glm::vec3(i, j, 0.0f)); //Traslada el objeto
                model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Rota el objeto
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
                Barrel.Draw(lightingShader); //Dibuja el objeto especificado
            }
            k += 1.25f;
            l -= 1.25f;
        }
        //BOTELLAS (ESTANTE BAR)
        for (j = 10.8f;j <= 11.2f;j += 0.4f) { //Se usa un ciclo for para dibujar varias copias del mismo objeto pero posiciones distintas
            for (i = -4.9f;i <= -1.7f;i += 0.4f) { //Se usa un ciclo for para dibujar varias copias del mismo objeto pero posiciones distintas
                //CERVEZA
                model = glm::mat4(1); //Setea la matriz en el origen del mundo
                model = glm::translate(model, glm::vec3(j, 2.2f, i)); //Traslada el objeto
                model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Rota el objeto
                model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f)); //Escala el objeto
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
                Bottle1.Draw(lightingShader); //Dibuja el objeto especificado
                //WHISKY
                model = glm::mat4(1); //Setea la matriz en el origen del mundo
                model = glm::translate(model, glm::vec3(j, 3.65f, i)); //Traslada el objeto
                model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Rota el objeto
                model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f)); //Escala el objeto
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
                Bottle2.Draw(lightingShader); //Dibuja el objeto especificado
                //VINO
                model = glm::mat4(1); //Setea la matriz en el origen del mundo
                model = glm::translate(model, glm::vec3(j, 5.25f, i) + glm::vec3(movKitX, movKitY, 0)); //Traslada el objeto
                model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Rota el objeto sobre el eje Y
                model = glm::rotate(model, glm::radians(rotKit), glm::vec3(1.0f, 0.0f, 0.0)); //Rota el objeto sobre el eje X
                model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f)); //Escala el objeto
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
                Bottle3.Draw(lightingShader); //Dibuja el objeto especificado
            }
        }
        //BOTELLAS (ESTANTE BODEGA)
        for (j = 1.9f;j <= 5.1f;j += 3.2f) { //Se usa un ciclo for para dibujar varias copias del mismo objeto pero posiciones distintas
            for (i = -3.5;i <= -2.3;i += 0.4f) { //Se usa un ciclo for para dibujar varias copias del mismo objeto pero posiciones distintas
                //WHISKY
                model = glm::mat4(1); //Setea la matriz en el origen del mundo
                model = glm::translate(model, glm::vec3(i, j, -24.25f)); //Traslada el objeto
                model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f)); //Escala el objeto
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
                Bottle2.Draw(lightingShader); //Dibuja el objeto especificado
                //VINO
                model = glm::mat4(1); //Setea la matriz en el origen del mundo
                model = glm::translate(model, glm::vec3(i, j + 1.6, -24.25f)); //Traslada el objeto
                model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f)); //Escala el objeto
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
                Bottle3.Draw(lightingShader); //Dibuja el objeto especificado
            }
        }
        for (j = 1.9f;j <= 5.1f;j += 3.2f) { //Se usa un ciclo for para dibujar varias copias del mismo objeto pero posiciones distintas
            for (i = -1.3;i <= -0.1;i += 0.4f) { //Se usa un ciclo for para dibujar varias copias del mismo objeto pero posiciones distintas
                //WHISKY
                model = glm::mat4(1); //Setea la matriz en el origen del mundo
                model = glm::translate(model, glm::vec3(i, j, -24.25f)); //Traslada el objeto
                model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f)); //Escala el objeto
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
                Bottle2.Draw(lightingShader); //Dibuja el objeto especificado
                //VINO
                model = glm::mat4(1); //Setea la matriz en el origen del mundo
                model = glm::translate(model, glm::vec3(i, j + 1.6, -24.25f)); //Traslada el objeto
                model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f)); //Escala el objeto
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
                Bottle3.Draw(lightingShader); //Dibuja el objeto especificado
            }
        }
        for (j = 1.9f;j <= 5.1f;j += 3.2f) { //Se usa un ciclo for para dibujar varias copias del mismo objeto pero posiciones distintas
            for (i = 0.9;i <= 1.7;i += 0.4f) { //Se usa un ciclo for para dibujar varias copias del mismo objeto pero posiciones distintas
                //WHISKY
                model = glm::mat4(1); //Setea la matriz en el origen del mundo
                model = glm::translate(model, glm::vec3(i, j, -24.25f)); //Traslada el objeto
                model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f)); //Escala el objeto
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
                Bottle2.Draw(lightingShader); //Dibuja el objeto especificado
                //VINO
                model = glm::mat4(1); //Setea la matriz en el origen del mundo
                model = glm::translate(model, glm::vec3(i, j + 1.6, -24.25f)); //Traslada el objeto
                model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f)); //Escala el objeto
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
                Bottle3.Draw(lightingShader); //Dibuja el objeto especificado
            }
        }
        for (j = 1.9f;j <= 5.1f;j += 3.2f) { //Se usa un ciclo for para dibujar varias copias del mismo objeto pero posiciones distintas
            for (i = 3.1;i <= 4.3;i += 0.4f) { //Se usa un ciclo for para dibujar varias copias del mismo objeto pero posiciones distintas
                //WHISKY
                model = glm::mat4(1); //Setea la matriz en el origen del mundo
                model = glm::translate(model, glm::vec3(i, j, -24.25f)); //Traslada el objeto
                model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f)); //Escala el objeto
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
                Bottle2.Draw(lightingShader); //Dibuja el objeto especificado
                //VINO
                model = glm::mat4(1); //Setea la matriz en el origen del mundo
                model = glm::translate(model, glm::vec3(i, j + 1.6, -24.25f)); //Traslada el objeto
                model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f)); //Escala el objeto
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
                Bottle3.Draw(lightingShader); //Dibuja el objeto especificado
            }
        }
        //CERVEZAS (REFRIGERADOR 1)
        for (k = 2.5f;k <= 3.55f;k += 1.05f) {
            for (j = -15.4f;j >= -17.0f;j -= 0.4f) { //Se usa un ciclo for para dibujar varias copias del mismo objeto pero posiciones distintas
                for (i = -9.6f;i >= -10.3f;i -= 0.3f) { //Se usa un ciclo for para dibujar varias copias del mismo objeto pero posiciones distinta
                    model = glm::mat4(1); //Setea la matriz en el origen del mundo
                    model = glm::translate(model, glm::vec3(i, k, j)); //Traslada el objeto
                    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Rota el objeto
                    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f)); //Escala el objeto
                    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                    glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
                    Bottle1.Draw(lightingShader); //Dibuja el objeto especificado
                }
            }
        }
        //CERVEZAS (REFRIGERADOR 2)
        for (k = 2.5f;k <= 3.55f;k += 1.05f) {
            for (j = -19.4f;j >= -21.0f;j -= 0.4f) { //Se usa un ciclo for para dibujar varias copias del mismo objeto pero posiciones distintas
                for (i = -9.6f;i >= -10.3f;i -= 0.3f) { //Se usa un ciclo for para dibujar varias copias del mismo objeto pero posiciones distinta
                    model = glm::mat4(1); //Setea la matriz en el origen del mundo
                    model = glm::translate(model, glm::vec3(i, k, j)); //Traslada el objeto
                    model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Rota el objeto
                    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f)); //Escala el objeto
                    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                    glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
                    Bottle1.Draw(lightingShader); //Dibuja el objeto especificado
                }
            }
        }

        glEnable(GL_BLEND);//Activa la funcionalidad para trabajar el canal alfa
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //VENTANAS Y PUERTAS
        model = glm::mat4(1); //Setea la matriz en el origen del mundo
        model = glm::translate(model, glm::vec3(0.0f, -0.2f, 0.0f)); //Traslada el objeto
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Rota el objeto
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 0.65); //Transparenta el objeto con un color blanco
        Window1.Draw(lightingShader); //Dibuja el objeto especificado
        model = glm::mat4(1); //Setea la matriz en el origen del mundo
        model = glm::translate(model, glm::vec3(0.0f, -0.2f, 0.0f)); //Traslada el objeto
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Rota el objeto
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 0.65); //Transparenta el objeto con un color blanco
        Window2.Draw(lightingShader); //Dibuja el objeto especificado
        model = glm::mat4(1); //Setea la matriz en el origen del mundo
        model = glm::translate(model, glm::vec3(-1.05f, -0.25f, 11.75f)); //Traslada el objeto
        model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f)); //Rota el objeto
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 0.65); //Transparenta el objeto con un color blanco
        Door.Draw(lightingShader); //Dibuja el objeto especificado (PUERTA ENTRADA)
        model = glm::mat4(1); //Setea la matriz en el origen del mundo
        model = glm::translate(model, glm::vec3(11.67f, -0.25f, -11.75f)); //Traslada el objeto
        model = glm::rotate(model, glm::radians(rot3), glm::vec3(0.0f, 1.0f, 0.0f)); //Rota el objeto
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 0.65); //Transparenta el objeto con un color blanco
        Door_W.Draw(lightingShader); //Dibuja el objeto especificado (PUERTA BODEGA)
        //PUERTAS REFRIGERADORES
        model = glm::mat4(1); //Setea la matriz en el origen del mundo
        model = glm::translate(model, glm::vec3(-8.9f, 1.525f, -20.2f)); //Traslada el objeto
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Rota el objeto
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f)); //Escala el objeto
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 0.65); //Transparenta el objeto con un color blanco
        Door_F.Draw(lightingShader); //Dibuja el objeto especificado
        model = glm::mat4(1); //Setea la matriz en el origen del mundo
        model = glm::translate(model, glm::vec3(-8.9f, 1.525f, -16.2f)); //Traslada el objeto
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Rota el objeto
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f)); //Escala el objeto
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 0.65); //Transparenta el objeto con un color blanco
        Door_F.Draw(lightingShader); //Dibuja el objeto especificado
        //PUERTAS CONGELADOR
        model = glm::mat4(1); //Setea la matriz en el origen del mundo
        model = glm::translate(model, glm::vec3(0.0f, -0.2f, 0.0f)); //Traslada el objeto
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Rota el objeto
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 0.65); //Transparenta el objeto con un color blanco
        Door_Freezer.Draw(lightingShader); //Dibuja el objeto especificado
        //ADORNOS
        model = glm::mat4(1); //Setea la matriz en el origen del mundo
        model = glm::translate(model, glm::vec3(0.0f, -0.2f, 0.0f)); //Traslada el objeto
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Rota el objeto
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 0.0, 1.0, 0.0, 0.85); //Transparenta el objeto con un color verde
        Bush1.Draw(lightingShader); //Dibuja el objeto especificado
        model = glm::mat4(1); //Setea la matriz en el origen del mundo
        model = glm::translate(model, glm::vec3(-0.2f, -0.2f, 0.0f)); //Traslada el objeto
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Rota el objeto
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 0.0, 1.0, 0.0, 0.85); //Transparenta el objeto con un color verde
        Bush2.Draw(lightingShader); //Dibuja el objeto especificado

        glDisable(GL_BLEND); //Desactiva el canal alfa
        glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0);
        glBindVertexArray(0);

        Anim.Use(); //Animacion de movimiento de las lamparas
        tiempo = glfwGetTime(); //Obtiene el tiempo con el que va a variar el cambio de color de los poinlights
        modelLoc = glGetUniformLocation(Anim.Program, "model"); //Se obtiene la matriz modelo desde el shader
        viewLoc = glGetUniformLocation(Anim.Program, "view"); //Se obtiene la matriz vista desde el shader
        projLoc = glGetUniformLocation(Anim.Program, "projection"); //Se obtiene la matriz proyeccion desde el shader
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1f(glGetUniformLocation(Anim.Program, "time"), tiempo);
        //LAMPARAS
        model = glm::mat4(1); //Setea la matriz en el origen del mundo
        model = glm::translate(model, glm::vec3(-0.2f, -0.2f, 0.0f)); //Traslada el objeto
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Rota el objeto
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Lamp1.Draw(Anim); //Dibuja el objeto especificado
        model = glm::mat4(1); //Setea la matriz en el origen del mundo
        model = glm::translate(model, glm::vec3(-0.2f, -0.2f, 0.0f)); //Traslada el objeto
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); //Rota el objeto
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        Lamp2.Draw(Anim); //Dibuja el objeto especificado

        // Also draw the lamp object, again binding the appropriate shader
        lampShader.Use();
        // Get location objects for the matrices on the lamp shader (these could be different on a different shader)
        modelLoc = glGetUniformLocation(lampShader.Program, "model"); //Se obtiene la matriz modelo desde el shader
        viewLoc = glGetUniformLocation(lampShader.Program, "view"); //Se obtiene la matriz  vista desde el shader
        projLoc = glGetUniformLocation(lampShader.Program, "projection"); //Se obtiene la matriz proyeccion desde el shader
        // Set matrices
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        model = glm::mat4(1); //Setea la matriz en el origen del mundo
        model = glm::translate(model, lightPos); //Traslada el objeto
        model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        // Draw the light object (using light's vertex attributes)
        for (GLuint i = 0; i < 17; i++)
        {
            model = glm::mat4(1); //Setea la matriz en el origen del mundo
            model = glm::translate(model, pointLightPositions[i]); //Traslada el objeto
            model = glm::scale(model, glm::vec3(0.06f)); // Make it a smaller cube
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36); //Dibuja los pointlights con triangulos
        }
        //model = glm::mat4(1);
        //model = glm::translate(model, spotLightPosition[0]);
        //model = glm::scale(model, glm::vec3(0.1f)); // Make it a smaller cube
        //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        //glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);

        // Draw skybox as last
        glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content
        SkyBoxshader.Use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix()));    // Remove any translation component of the view matrix
        glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view)); //Se obtiene la matriz vista desde el shader
        glUniformMatrix4fv(glGetUniformLocation(SkyBoxshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection)); //Se obtiene la matriz proyeccion desde el shader

        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36); //Dibuja el SkyBox con triangulos
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // Set depth function back to default

        // Swap the screen buffers
        glfwSwapBuffers(window);
    }


    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();



    return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{
    // Camera controls
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP]) //Desplazarse hacia adelante
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }

    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN]) //Desplazarse hacia atras
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }

    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT]) //Desplazarse hacia la izquierda
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }

    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT]) //Desplazarse hacia la derecha
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }

    if (anim) //Animacion de la puerta principal (abrir)
    {
        if (rot > -180.0f) //Modifica la rotacion de la puerta para abirla
        {
            rot -= 2.0f;
            flag = false;
        }
        else
            flag = true;
    }

    if (not anim) //Animacion de la puerta principal (cerrar)
    {
        if (rot < -90.0f) //Modifica la rotacion de la puerta para cerrarla
        {
            rot += 2.0f;
            flag = true;
        }
        else
            flag = false;
    }

    if (anim2) //Animacion de la puerta de la barra (abrir)
    {
        if (rot2 < 90.0f) //Modifica la rotacion de la puerta del bar para levantarla
        {
            rot2 += 2.0f;
            flag2 = false;
        }
        else
            flag2 = true;
    }

    if (not anim2) //Animacion de la puerta de la barra (cerrar)
    {
        if (rot2 > 0.0f) //Modifica la rotacion de la puerta del bar para bajarla
        {
            rot2 -= 2.0f;
            flag2 = true;
        }
        else
            flag2 = false;
    }

    if (anim3) //Animacion de la puerta de la bodega (abrir)
    {
        if (rot3 > -180.0f) //Modifica la rotacion de la puerta para abirla
        {
            rot3 -= 2.0f;
            flag3 = false;
        }
        else
            flag3 = true;
    }

    if (not anim3) //Animacion de la puerta de la bodega (cerrar)
    {
        if (rot3 < -90.0f) //Modifica la rotacion de la puerta para cerrarla
        {
            rot3 += 2.0f;
            flag3 = true;
        }
        else
            flag3 = false;
    }
}

//ANIMACION 1
void animacion() //Funcion que permite la animacion de caida de las botellas con las teclas definidas
{
    //Movimiento botellas
    if (caida) //Animacion de caida de botellas de vino
    {
        if (recorrido1)
        {
            movKitX -= 0.1f; //Desplaza todas las botellas una unidad menos sobre el eje X
            if (movKitX < -0.9f)
            {
                recorrido1 = false;
                recorrido2 = true;
            }
        }
        if (recorrido2)
        {
            rotKit = 90; //Rota los objetos sobre el eje X para ahora realizar un desplazamiento sobre el eje Y
            movKitY -= 0.1f; //Desplaza todas las botellas cinco unidades menos sobre el eje Y
            if (movKitY < -4.9)
            {
                recorrido2 = false;
                recorrido3 = true;
            }
        }
    }

    if (levantar && (not recorrido2)) //Animacion para devolver las botellas a su sitio
    {
        if (recorrido3)
        {
            movKitY += 5.0f; //Desplaza todas las botellas cinco unidades mas sobre el eje Y
            if (movKitY > -0.1f)
            {
                recorrido3 = false;
                recorrido4 = true;
            }
        }
        if (recorrido4)
        {
            rotKit = 0; //Rota los objetos sobre el eje X para ahora realizar un desplazamiento sobre el eje X
            movKitX += 1.0f; //Desplaza todas las botellas una unidad mas sobre el eje X
            if (movKitX > -0.1f)
            {
                recorrido1 = true;
                recorrido4 = false;
            }
        }
    }
}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
    if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            keys[key] = true;
        }
        else if (action == GLFW_RELEASE)
        {
            keys[key] = false;
        }
    }

    if (keys[GLFW_KEY_SPACE]) //Animacion de luces blancas del bar
    {
        active = !active;
        if (active) //Si la tecla espacio fue presionada
        {
            if (active2) //Si la tecla I fue presionada
            {
                Light1 = glm::vec3(1.0f, 1.0f, 1.0f); //Enciende las luces del bar con un color blanco
                Light2 = glm::vec3(0);//Cuando es solo un valor en los 3 vectores pueden dejar solo una componente
            }
            else
            {
                Light1 = glm::vec3(1.0f, 1.0f, 1.0f); //Enciende las luces del bar con un color blanco
                Light2 = glm::vec3(1.0f, 1.0f, 1.0f); //Enciende las luces del titulo del bar con un color blanco
            }
        }
        else
        {
            Light1 = glm::vec3(0); //Apaga las luces del bar
            Light2 = glm::vec3(0); //Apaga las luces del titulo del bar
        }
    }

    if (keys[GLFW_KEY_I]) //Animacion de cambio de luces del titulo del bar
    {
        active2 = !active2;
        if (active2) //Si la tecla I fue presionada
        {
            Light2 = glm::vec3(0); //Apaga las luces blancas del titulo del bar
            Light3 = glm::vec3(1.0f, 0.0f, 1.0f); //Enciende las luces del titulo del bar con un color morado que despues cambia a rojo
        }
        else
        {
            if (active) //Si la tecla espacio fue presionada
            {
                Light2 = glm::vec3(1.0f, 1.0f, 1.0f); //Enciende las luces del titulo del bar con un color blanco
                Light3 = glm::vec3(0); //Apaga las luces de color del titulo del bar
            }
            else
            {
                Light2 = glm::vec3(0); //Apaga las luces blancas del titulo del bar
                Light3 = glm::vec3(0); //Apaga las luces de color del titulo del bar
            }
        }
    }

    if (keys[GLFW_KEY_O]) //Activa la animacion de apretura y cierre de la puerta principal
    {
        if (not flag) //Si la tecla O fue presionada
            anim = true; //Activa la animacion
        else
            anim = false;
    }

    if (keys[GLFW_KEY_P]) //Activa la animacion de apretura y cierre de la puerta del bar
    {
        if (not flag2) //Si la tecla P fue presionada
            anim2 = true; //Activa la animacion
        else
            anim2 = false;
    }

    if (keys[GLFW_KEY_L]) //Activa la animacion de apretura y cierre de la puerta de la bodega
    {
        if (not flag3) //Si la tecla O fue presionada
            anim3 = true; //Activa la animacion
        else
            anim3 = false;
    }

    if (keys[GLFW_KEY_4]) //Activa la animacion de caida de botellas de vino
    {
        caida = true;
        levantar = false;
    }

    if (keys[GLFW_KEY_5]) //Activa la animacion para devolver las botellas a su sitio
    {
        levantar = true;
        caida = false;
    }
}

void MouseCallback(GLFWwindow *window, double xPos, double yPos)
{
    if (firstMouse)
    {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    GLfloat xOffset = xPos - lastX;
    GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

    lastX = xPos;
    lastY = yPos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}