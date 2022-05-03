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

// Function prototypes
void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow *window, double xPos, double yPos);
void DoMovement();

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
bool active;
bool flag = false;
bool flag2 = false;
bool anim = false;
bool anim2 = false;
float rot = -90.0f;
float rot2 = 0.0f;
float i;
float j;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(-0.2f, 7.7f, 0.0f), //Lampara 1
    glm::vec3(-0.2f, 7.7f, -8.0f), //Lampara2
    glm::vec3(-6.35f, 9.518f, 12.35f), //Letra A
    glm::vec3(-6.45f, 10.6f, 12.35f), //Letra I
    glm::vec3(-6.45f, 11.725f, 12.35f), //Letra H
    glm::vec3(0.0f, 9.518f, 12.35f),
    glm::vec3(0.0f, 10.6f, 12.35f),
    glm::vec3(0.0f, 11.725f, 12.35f),
    glm::vec3(6.35f, 9.518f, 12.35f),
    glm::vec3(6.45f, 10.6f, 12.35f),
    glm::vec3(6.45f, 11.725f, 12.35f)
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


glm::vec3 Light1 = glm::vec3(0);
glm::vec3 Light2 = glm::vec3(0);
glm::vec3 Light3 = glm::vec3(0);
glm::vec3 Light4 = glm::vec3(0);

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
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Iluminacion 2", nullptr, nullptr);

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



    Shader lightingShader("Shaders/lighting.vs", "Shaders/lighting.frag");
    Shader lampShader("Shaders/lamp.vs", "Shaders/lamp.frag");
    
    Model Floor((char*)"Models/Bar/Piso.obj");
    Model Roof1((char*)"Models/Bar/Techo.obj");
    Model Lamp1((char*)"Models/Lampara/Lampara1.obj");
    Model Lamp2((char*)"Models/Lampara/Lampara2.obj");
    Model Roof2((char*)"Models/Bar/Techo2.obj");
    Model Wall1((char*)"Models/Bar/Pared_izq.obj");
    Model Wall2((char*)"Models/Bar/Pared_trasera.obj");
    Model Wall3((char*)"Models/Bar/Pared_der.obj");
    Model Wall4((char*)"Models/Bar/Pared_frontal.obj");
    Model Bush1((char*)"Models/Bar/Arbusto.obj");
    Model Bush2((char*)"Models/Bar/Arbusto2.obj");
    Model Window1((char*)"Models/Bar/Ventana_izq.obj");
    Model Window2((char*)"Models/Bar/Ventana_der.obj");
    Model Door((char*)"Models/Bar/Puerta.obj");
    Model Title1((char*)"Models/Bar/Titulo.obj");
    Model Title2((char*)"Models/Bar/Titulo2.obj");
    Model Table((char*)"Models/Mesa/table1.obj");
    Model Chair((char*)"Models/Silla/chair2.obj");
    Model Bench((char*)"Models/Banco/barstool3.obj");
    Model Counter1((char*)"Models/Barra/barra1.obj");
    Model Counter2((char*)"Models/Barra/barra2.obj");
    Model Counter3((char*)"Models/Barra/puerta_barra.obj");
    Model Counter4((char*)"Models/Barra/techo_barra.obj");
    Model Bottle1((char*)"Models/Bottles/Beer_bottle/beer2.obj");
    Model Bottle2((char*)"Models/Bottles/Whiskey_bottle/whisky.obj");
    Model Bottle3((char*)"Models/Bottles/Wine_bottle/wine.obj");
    Model Rack((char*)"Models/Estante/estante.obj");

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
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"),0.1f,0.1f,0.1f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.1f, 0.1f, 0.1f);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"),1.0f, 1.0f, 1.0f);


        // Point light 1
        glm::vec3 lightColor;
        lightColor.x= abs(sin(glfwGetTime() *Light1.x));
        lightColor.y= abs(sin(glfwGetTime() *Light1.y));
        lightColor.z= sin(glfwGetTime() *Light1.z);
        
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), Light1.x, Light1.y, Light1.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), Light1.x, Light1.y, Light1.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.09f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"),0.032f);

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
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].ambient"), Light1.x, Light1.y, Light1.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].diffuse"), Light1.x, Light1.y, Light1.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[2].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].linear"), 0.22f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[2].quadratic"), 0.20f);

        // Point light 4
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].position"), pointLightPositions[3].x, pointLightPositions[3].y, pointLightPositions[3].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].ambient"), Light1.x, Light1.y, Light1.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].diffuse"), Light1.x, Light1.y, Light1.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[3].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].linear"), 0.22f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[3].quadratic"), 0.20f);

        // Point light 5
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].position"), pointLightPositions[4].x, pointLightPositions[4].y, pointLightPositions[4].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].ambient"), Light1.x, Light1.y, Light1.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].diffuse"), Light1.x, Light1.y, Light1.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[4].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[4].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[4].linear"), 0.22f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[4].quadratic"), 0.20f);

        // Point light 6
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[5].position"), pointLightPositions[5].x, pointLightPositions[5].y, pointLightPositions[5].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[5].ambient"), Light1.x, Light1.y, Light1.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[5].diffuse"), Light1.x, Light1.y, Light1.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[5].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[5].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[5].linear"), 0.22f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[5].quadratic"), 0.20f);

        // Point light 7
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[6].position"), pointLightPositions[6].x, pointLightPositions[6].y, pointLightPositions[6].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[6].ambient"), Light1.x, Light1.y, Light1.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[6].diffuse"), Light1.x, Light1.y, Light1.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[6].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[6].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[6].linear"), 0.22f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[6].quadratic"), 0.20f);

        // Point light 8
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[7].position"), pointLightPositions[7].x, pointLightPositions[7].y, pointLightPositions[7].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[7].ambient"), Light1.x, Light1.y, Light1.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[7].diffuse"), Light1.x, Light1.y, Light1.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[7].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[7].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[7].linear"), 0.22f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[7].quadratic"), 0.20f);

        // Point light 9
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[8].position"), pointLightPositions[8].x, pointLightPositions[8].y, pointLightPositions[8].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[8].ambient"), Light1.x, Light1.y, Light1.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[8].diffuse"), Light1.x, Light1.y, Light1.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[8].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[8].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[8].linear"), 0.22f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[8].quadratic"), 0.20f);

        // Point light 10
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[9].position"), pointLightPositions[9].x, pointLightPositions[9].y, pointLightPositions[9].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[9].ambient"), Light1.x, Light1.y, Light1.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[9].diffuse"), Light1.x, Light1.y, Light1.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[9].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[9].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[9].linear"), 0.22f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[9].quadratic"), 0.20f);

        // Point light 11
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[10].position"), pointLightPositions[10].x, pointLightPositions[10].y, pointLightPositions[10].z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[10].ambient"), Light1.x, Light1.y, Light1.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[10].diffuse"), Light1.x, Light1.y, Light1.z);
        glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[10].specular"), 1.0f, 1.0f, 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[10].constant"), 1.0f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[10].linear"), 0.22f);
        glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[10].quadratic"), 0.20f);

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
        glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 64.0f);

        // Create camera transformations
        glm::mat4 view;
        view = camera.GetViewMatrix();

        // Get the uniform locations
        GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
        GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
        GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

        // Pass the matrices to the shader
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        glm::mat4 model(1);
        //Carga de modelo 
        view = camera.GetViewMatrix();  
        //PISO Y TECHO
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.2f, -0.2f, 0.4f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        Floor.Draw(lightingShader);
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-0.16f, -0.2f, 0.0f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        Roof1.Draw(lightingShader);
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-0.2f, -0.2f, 0.0f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        Roof2.Draw(lightingShader);
        //LAMPARAS
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-0.2f, -0.2f, 0.0f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        Lamp1.Draw(lightingShader);
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-0.2f, -0.2f, 0.0f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        Lamp2.Draw(lightingShader);
        //PAREDES
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, -0.2f, 0.0f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        Wall1.Draw(lightingShader);
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, -0.2f, 0.0f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        Wall2.Draw(lightingShader);
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-0.2f, -0.2f, 0.0f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        Wall3.Draw(lightingShader);
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, -0.2f, 0.0f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        Wall4.Draw(lightingShader);
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, -0.2f, 0.0f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        Title1.Draw(lightingShader);
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, -0.2f, 0.0f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        Title2.Draw(lightingShader);
        //COLUMNAS DE MESAS Y SILLAS
        for (j = -8.0f;j <= 4.0f;j += 4.0f) {
            for (i = -2.8f;i >= -10.0f;i -= 1.2f) {
                //MESAS
                model = glm::mat4(1);
                model = glm::translate(model, glm::vec3(i, 0.0f, j));
                model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
                Table.Draw(lightingShader);
                //SILLAS_ENFRENTE
                model = glm::mat4(1);
                model = glm::translate(model, glm::vec3(i, 0.0f, j+1));
                model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
                Chair.Draw(lightingShader);
                //SILLAS_ATRAS
                model = glm::mat4(1);
                model = glm::translate(model, glm::vec3(i, 0.0f, j-1));
                model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
                Chair.Draw(lightingShader);
            }
        }
        //BANCOS
        for (i = -10.4f;i <= 6.0f;i += 1.2f) {
            model = glm::mat4(1);
            model = glm::translate(model, glm::vec3(4.0f, 0.0f, i));
            model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
            Bench.Draw(lightingShader);
        }
        //BARRA
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(4.0f, 0.0f, -2.65f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.715f, 0.5f, 0.5f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        Counter1.Draw(lightingShader);
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(4.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        Counter2.Draw(lightingShader);
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(9.5f, 1.95f, 5.5f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::rotate(model, glm::radians(rot2), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.69f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        Counter3.Draw(lightingShader);
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(4.2f, 1.55f, -3.7f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.68f, 0.5f, 0.5f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        Counter4.Draw(lightingShader);
        //ESTANTE
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(4.4f, 1.55f, -3.0f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        Rack.Draw(lightingShader);
        //BOTELLAS
        for (j = 10.8f;j <= 11.2f;j += 0.4f) {
            for (i = -4.9f;i <= -1.7f;i += 0.4f) {
                //CERVEZA
                model = glm::mat4(1);
                model = glm::translate(model, glm::vec3(j, 2.2f, i));
                model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
                Bottle1.Draw(lightingShader);
                //WHISKY
                model = glm::mat4(1);
                model = glm::translate(model, glm::vec3(j, 3.65f, i));
                model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
                Bottle2.Draw(lightingShader);
                //VINO
                model = glm::mat4(1);
                model = glm::translate(model, glm::vec3(j, 5.25f, i));
                model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
                model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
                glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
                Bottle3.Draw(lightingShader);
            }
        }

        glEnable(GL_BLEND);//Activa la funcionalidad para trabajar el canal alfa
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //VENTANAS Y PUERTA
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, -0.2f, 0.0f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 0.65);
        Window1.Draw(lightingShader);
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, -0.2f, 0.0f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 0.65);
        Window2.Draw(lightingShader);
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-1.05f, -0.25f, 11.75f));
        model = glm::rotate(model, glm::radians(rot), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 0.65);
        Door.Draw(lightingShader);
        //ADORNOS
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(0.0f, -0.2f, 0.0f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 0.0, 1.0, 0.0, 0.85);
        Bush1.Draw(lightingShader);
        model = glm::mat4(1);
        model = glm::translate(model, glm::vec3(-0.2f, -0.2f, 0.0f));
        model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform1i(glGetUniformLocation(lightingShader.Program, "activaTransparencia"), 0);
        glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 0.0, 1.0, 0.0, 0.85);
        Bush2.Draw(lightingShader);

        glDisable(GL_BLEND); //Desactiva el canal alfa
        glUniform4f(glGetUniformLocation(lightingShader.Program, "colorAlpha"), 1.0, 1.0, 1.0, 1.0);
        glBindVertexArray(0);
    

        // Also draw the lamp object, again binding the appropriate shader
        lampShader.Use();
        // Get location objects for the matrices on the lamp shader (these could be different on a different shader)
        modelLoc = glGetUniformLocation(lampShader.Program, "model");
        viewLoc = glGetUniformLocation(lampShader.Program, "view");
        projLoc = glGetUniformLocation(lampShader.Program, "projection");

        // Set matrices
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        model = glm::mat4(1);
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        // Draw the light object (using light's vertex attributes)
        for (GLuint i = 0; i < 5; i++)
        {
            model = glm::mat4(1);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.06f)); // Make it a smaller cube
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        //model = glm::mat4(1);
        //model = glm::translate(model, spotLightPosition[0]);
        //model = glm::scale(model, glm::vec3(0.1f)); // Make it a smaller cube
        //glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        //glBindVertexArray(VAO);
        //glDrawArrays(GL_TRIANGLES, 0, 36);

        glBindVertexArray(0);



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
    if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);

    }

    if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);


    }

    if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
    {
        camera.ProcessKeyboard(LEFT, deltaTime);


    }

    if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);


    }

    if (anim)
    {
        if (rot > -180.0f)
        {
            rot -= 1.0f;
            flag = false;
        }
        else
            flag = true;
    }

    if (not anim)
    {
        if (rot < -90.0f)
        {
            rot += 1.0f;
            flag = true;
        }
        else
            flag = false;
    }

    if (anim2)
    {
        if (rot2 < 90.0f)
        {
            rot2 += 1.0f;
            flag2 = false;
        }
        else
            flag2 = true;
    }

    if (not anim2)
    {
        if (rot2 > 0.0f)
        {
            rot2 -= 1.0f;
            flag2 = true;
        }
        else
            flag2 = false;
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

    if (keys[GLFW_KEY_SPACE])
    {
        active = !active;
        if (active)
        {
            Light1 = glm::vec3(1.0f, 1.0f, 1.0f);
        }
        else
        {
            Light1 = glm::vec3(0);//Cuado es solo un valor en los 3 vectores pueden dejar solo una componente
        }
    }

    if (keys[GLFW_KEY_O])
    {
        if (not flag)
            anim = true;
        else
            anim = false;
    }

    if (keys[GLFW_KEY_P])
    {
        if (not flag2)
            anim2 = true;
        else
            anim2 = false;
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