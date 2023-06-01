/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////// Author: Brennen Romo                                                                                                                                                                            ////////////////  
//////////////// Date: May 12th, 2023                                                                                                                                                                            ////////////////
//////////////// Project: Mesh class                                                                                                                                                                             ////////////////
//////////////// Description: This project displays a 3D cube loaded into a Model class. You can move the camera with w,a,s,d,q,and e and the cursor                                                              ////////////////
////////////////              The Lightsource can be moved with i,j,k,l,u,o. The Cube can be rotated with the left and right arrow key                                                                           ////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#define STB_IMAGE_IMPLEMENTATION
#define STBI_MSC_SECURE_CRT
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "Shader.h"
#include "Camera.h"
#include "Mesh.h"
#include "Vertex.h"
#include "Model.h"

void windowSizeCallback(GLFWwindow* window, int width, int height);
void windowCloseCallback(GLFWwindow* window);
void keyboardInput(GLFWwindow* window);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
unsigned int loadTexture(char const* path);

/////////////////////// Global Settings //////////////////////////////////////////
const int screenHeight = 1200;
const int screenWidth = 1600;
float angle = 0.0f;
const unsigned int MapWidth = 1600;
const unsigned int MapHeight = 1600;


////////////////////// Lighting   ////////////////////////////////////////

glm::vec3 lightPos(1.2f, 200.0f, 2.0f);

////////////////////// Camera ////////////////////////////////////////////

Camera camera(glm::vec3(0.0f, 0.5f, 5.0f));
float lastX = screenWidth / 2.0f;
float lastY = screenHeight / 2.0f;
bool firstMouse = true;
float xNorm = 0;
float yNorm = 0;

////////////////////// Time ////////////////////////////////////////////

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

/////////////////////// Global Data ////////////////////////////////////////////
// Vertex Data for our Cube
std::vector<float> planeVertices = {
    0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 50.0f, 50.0f,
    0.5f, 0.0f, -0.5f,0.0f, 1.0f, 0.0f, 50.0f, 0.0f,
    -0.5f, 0.0f, -0.5f,0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, 0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 50.0f
};
unsigned int planeIndices[] = {
    0, 1, 3,
    1, 2, 3
};
//Vertex Vectors that will store our Mesh data
std::vector<Vertex> boxVerts, planeVerts;
std::vector<float> boxVertices = {
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
};

float vertices[] = {
         1.0f,  1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f
};
// Index Data for our Rectangle
unsigned int indices[] = {
    0, 1, 3,  // 1st Primitive
    1, 2, 3   // 2nd Primitive
};


std::vector<Vertex> mapVerts;

std::vector<float> mapVertices = {};

unsigned int mapVertCount = 0;

std::vector<unsigned int> mapIndices;

unsigned int mapIndexCount = 0;

// Position data for our cube
glm::vec3 cubePosition(0, 0.5, 0);
/////////////////////////// Main Program ///////////////////

int main()
{
    //initialize GLFW
    if (!glfwInit())
    {
        //close program if we've failed to run GLFW
        std::cout << "failed to initialize GLFW";
        return -1;
    }
    //Definitions for when we are using Apple device
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    //Set GLFW Window Parameters here (Using version 4.6)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    //using Core profile of OpenGL, (Compatibility profile is for legacy)
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //////////////////////////create Fullscreen Window here////////////////////////
    // GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Render Window", glfwGetPrimaryMonitor(), NULL);

    //////////////////////////create Windowed fullscreen here////////////////////////
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "Render Window", NULL, NULL);
    ////////////////////////create window here////////////////////////////////
   // GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "Render Window", NULL, NULL);
    if (!window)
    {
        std::cout << "Error Creating our GLFW Window!" << std::endl;
        //close down GLFW
        glfwTerminate();
        return -1;
    }
    // Make our Window the current Context
    glfwMakeContextCurrent(window);

    //camera.setDimensions(mode->width, mode->height);
    camera.setDimensions(MapWidth, MapHeight);
    
    /////////////////////////////////////////////////////////////////////////////////
    // /////////////////////////// Set Callback Functions Here ///////////////////////////
    // /////////////////////////////////////////////////////////////////////////////////
    //Sets the Callback function for when the frameBuffer is resized to our custom Callback function.
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

    //Set callback Function for when Window is resized
    glfwSetWindowSizeCallback(window, windowSizeCallback);

    //set callback function for when we close glfw Window 
    glfwSetWindowCloseCallback(window, windowCloseCallback);


    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);


    // Start glad and load all openGL function pointers (for whichever specific system and archritecture our program is running on)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // Configure global penGl state
    glEnable(GL_DEPTH_TEST);

    ////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////// Shaders //////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////


    Shader ourShader("shader.vs", "shader.fs");
    Shader mapShader("sNoise.vs", "sNoise.fs");
    Shader lightShader("Light.vs", "Light.fs");
    mapShader.use();

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////  Map Generation stuff ////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    GLuint fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    GLuint mapTexture;
    glGenTextures(1, &mapTexture);
    glBindTexture(GL_TEXTURE_2D, mapTexture);


    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, MapWidth, MapHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Attach the texture to the framebuffer
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mapTexture, 0);

    // Check framebuffer completeness
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        // Handle framebuffer error
        // ...

        // Clean up and exit
        glDeleteFramebuffers(1, &fbo);
        glDeleteTextures(1, &mapTexture);
        glfwTerminate();
        return -1;
    }

    unsigned int VBO, VAO, EBO;
    //Generate VAO, VBO,EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // First, Bind our Vertex Array Object
    glBindVertexArray(VAO);
    // Bind our VBO now that we have a VAO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //Static Draw for data reused many times without changing

    // Bind our EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    //Specify how our Vertex Data is laid out. (x,y,z position coordinates) 
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);


    //Unbind VBO
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    mapShader.use();


    float randy = 0;
    std::srand(time(NULL));
    randy = rand();
    mapShader.setFloat("u_time", randy);
    mapShader.setVec2("u_resolution", MapWidth, MapHeight);


    //Set lightShader Params
////Render our lightsource
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glViewport(0, 0, MapWidth, MapHeight);



    glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    //glDrawArrays(GL_TRIANGLES, 0, 6);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    // glBindVertexArray(0); // no need to unbind it every time 

 

    // Set the read buffer to the color attachment of the FBO
    glReadBuffer(GL_COLOR_ATTACHMENT0);

    unsigned char* pixels = new unsigned char[MapWidth * MapHeight * 4]; // 4 channels (RGBA)

    // Read the pixel data from the FBO
    glReadPixels(0, 0, MapWidth, MapHeight, GL_RGBA, GL_UNSIGNED_BYTE, pixels);


    stbi_write_png("map.png", MapWidth, MapHeight, 4, pixels, MapWidth * 4);
    
    // Iterate through each pixel
    for (int y = 0; y < MapHeight; y++) {
        for (int x = 0; x < MapWidth; x++) {
            // Get the pixel index in the pixel data buffer
            int pixelIndex = (y * MapHeight + x) * 4;

            mapVertices.push_back(x);

            mapVertices.push_back(pixels[pixelIndex]);
            mapVertices.push_back(y);
            mapVertCount += 1;
        }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);




    //////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////// Textures & Materials   ////////////////////////////////////////////////////////
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    camera.setDimensions(mode->width, mode->height);
    glViewport(0, 0, mode->width, mode->height);
    ourShader.use();
    loadVertexArray(mapVertices, mapVerts,3);
    loadVertexArray(boxVertices, boxVerts);
    loadVertexArray(planeVertices, planeVerts);

    for (int i = 0; i < mapVerts.size();i++)
    {
        if (i / MapWidth < MapHeight-1)
        {
            if (i % MapWidth < (MapWidth - 1))
            {
                if (i % 2 == 1)
                {
                    mapIndices.push_back(i);
                    mapIndices.push_back(i + 1);
                    mapIndices.push_back(i + MapWidth);
                    mapIndexCount += 3;
                    mapIndices.push_back(i + 1);
                    mapIndices.push_back(i + MapWidth + 1);
                    mapIndices.push_back(i + MapWidth);
                    mapIndexCount += 3;
                }
                else
                {
                    mapIndices.push_back(i);
                    mapIndices.push_back(i + MapWidth + 1);
                    mapIndices.push_back(i + MapWidth);
                    mapIndexCount += 3;
                    mapIndices.push_back(i);
                    mapIndices.push_back(i + 1);
                    mapIndices.push_back(i + MapWidth + 1);
                    mapIndexCount += 3;
                }
            }
        }
    }

    for (int i = 0; i < mapVerts.size(); i++)
    {
        mapVerts[i].texcoord[0] = i % MapWidth;
        mapVerts[i].texcoord[1] = i / MapHeight;
    }

    for (int i = 0; i < mapIndices.size(); i +=3)
    {
        int index1 = mapIndices[i];
        int index2 = mapIndices[i + 1];
        int index3 = mapIndices[i + 2];

        Vertex& v1 = mapVerts[index1];
        Vertex& v2 = mapVerts[index2];
        Vertex& v3 = mapVerts[index3];

        // Calculate the triangle's normal
        glm::vec3 edge1 = v2.position - v1.position;
        glm::vec3 edge2 = v3.position - v1.position;
        glm::vec3 normal = glm::cross(edge1, edge2);

        // Add the triangle's normal to the vertices
        v1.normal -= normal;
        v2.normal -= normal;
        v3.normal -= normal;
    }

    glm::vec3 pos = mapVerts.at(MapWidth*MapHeight/2).position;

    camera.setPoistion(glm::vec3(0.0f, pos.z/10 , 0.0f));
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Texture texture1("Resources/Textures/crate.jpg", GL_IMAGE_2D);
    Texture texture2("Resources/Textures/checkered.png", GL_IMAGE_2D);
    Texture texture3("Resources/Textures/Floor.jpg", GL_IMAGE_2D);


    Material mat("Stone", texture3.getID(), texture3.getID(), texture3.getID());
    Material mat1("Crate", texture1.getID(), texture1.getID(), texture1.getID());
    Material mat2("Stone", texture3.getID(), texture3.getID(), texture3.getID());

    Mesh mapMesh("Map", mapVerts, mapVertCount, mapIndices, mapIndexCount, { 0,0,0 }, { 0,0,0 }, { 0,0,0 }, { 1,1,1 });

    Mesh Mesh1("Crate", boxVerts, 36, NULL, 0, { 0,0,0 }, { 0,0,0 }, { 0,0,0 }, { 1,1,1 }, mat1);
    Mesh Mesh2("Ground", planeVerts, 4, planeIndices, 6, { 0,0,0 }, { 0,0,0 }, { 0,0,0 }, { 1,1,1 }, mat2);
    Mesh Mesh3("Light", boxVerts, 36, NULL, 0, { 0,0,0 }, { 0,0,0 }, { 0,0,0 }, { 1,1,1 }, mat1);
    Mesh Mesh4("SecondBox", boxVerts, 36, NULL, 0, { 0,0,0 }, { 0,0,0 }, { 0,0,0 }, { 1,1,1 }, mat1);

    std::vector<Mesh*> meshes1, meshes2, meshes3, meshes4;
    meshes1.push_back(&Mesh1);
    meshes1.push_back(&Mesh4);
    meshes2.push_back(&Mesh2);
    meshes3.push_back(&Mesh3);
    meshes4.push_back(&mapMesh);
    Mesh1.setPosition(cubePosition);
    Mesh2.setScale(glm::vec3(100.f, 1.f, 100.f));
    Mesh3.setScale(glm::vec3(0.5f, 0.5f, 0.5f));
    Mesh4.setPosition(glm::vec3(0.0f, 1.5f, 0.0f));
    mapMesh.setScale(glm::vec3(10.0f, 1.0f, 10.0f));
    mapMesh.setPosition(glm::vec3(-(int)(MapWidth*10/2), 0.5f, -(int)(MapHeight*10/2)));
    Model mod1(glm::vec3(0.0f, 0.0f, 0.0f), meshes1);
    Model mod2(glm::vec3(0.0f, 0.0f, 0.0f), meshes2);
    Model mod3(glm::vec3(0.0f, 0.0f, 0.0f), meshes3);
    Model mod4(glm::vec3(0.0f, 0.0f, 0.0f), meshes4);

    mat1.sendToShader(ourShader);

    texture1.bind(texture1.getID());
    texture2.bind(texture2.getID());
    texture3.bind(texture3.getID());


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////




    // light properties
    ourShader.setVec3("light.ambient", 0.1f, 0.1f, 0.1f);
    ourShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
    ourShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

    std::cout << mapVerts.size() << std::endl;
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //Set initial Mesh transforms
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // Start Render Loop here
    do
    {
        lightPos = camera.Position + glm::vec3(0.0f, 5.0f, 0.0f);
        //set constantly changing uniforms
        ourShader.use();
        ourShader.setVec3("viewPos", camera.Position);
        ourShader.setVec3("light.position", lightPos);

        //set Mesh transforms


        mod1.setRotation(glm::vec3(0.0f, angle, 0.0f));
        mod3.setPosition(lightPos);


        //calculate delta time

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        //input
        keyboardInput(window);
        camera.ProcessMouseMovement(xNorm, -yNorm);

        //clear the backbuffer to set colour
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        ///////////////////////////////////////////////////////////////////////////////////////////
        //create variables for our cube's transformation
        ///////////////////////////////////////////////////////////////////////////////////////////
        glm::mat4 projection = camera.Projection;
        glm::mat4 view = camera.GetViewMatrix();

        //Set our Shader's variables

        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //Render our Meshes
        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        mod1.render(&ourShader);
        mod2.render(&ourShader);
        mod4.render(&ourShader);



        lightShader.use();
        lightShader.setMat4("projection", projection);
        lightShader.setMat4("view", view);
        lightShader.setVec3("viewPos", camera.Position);
        lightShader.setVec3("light.position", lightPos);
        ////Render our lightsource
        mod3.render(&lightShader);

        //Swap buffers
        glfwSwapBuffers(window);
        // poll windows events, call callback functions for events
        glfwPollEvents();
    } while (!glfwWindowShouldClose(window));




    // close GLFW
    glfwTerminate();
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////// FUNCTION DEFINITIONS           ////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



// Function for Processing Inputs
void keyboardInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) //if Escape key is pressed, close Window
        glfwSetWindowShouldClose(window, true);
    else if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) //if Escape key is pressed, close Window
    {
        /*int maximized = glfwGetWindowAttrib(window, GLFW_MAXIMIZED);
        if (!maximized)
            glfwMaximizeWindow(window);
        else
            glfwRestoreWindow(window);*/
        if (camera.getSpeed() != 200.0f)
        {
            camera.setSpeed(200.0f);
        }
        else
        {
            camera.setSpeed(5.0f);
        }
    }
    else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) //if Escape key is pressed, close Window
    {
        angle += 0.1;
    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) //if Escape key is pressed, close Window
    {
        angle -= 0.1f;
    }


    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.ProcessKeyboard(ROLLL, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessKeyboard(ROLLR, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        lightPos -= glm::vec3(0, 0, 0.01);
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        lightPos += glm::vec3(0, 0, 0.01);
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        lightPos -= glm::vec3(0.01, 0, 0);
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        lightPos += glm::vec3(0.01, 0, 0);
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
        lightPos += glm::vec3(0, 0.01, 0);
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        lightPos -= glm::vec3(0, 0.01, 0);
}

unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}


void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    xNorm = ((xpos - (camera.getWidth() / 2)) * 2) / camera.getWidth();
    yNorm = ((ypos - (camera.getHeight() / 2)) * 2) / camera.getHeight();

    camera.ProcessMouseMovement(xNorm, yNorm);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

//Function callback for changing our Window's size
void windowSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    camera.setDimensions(width, height);
    std::cout << "Width: " << width << "\n" << "Height: " << height << "\n";
}

//Function callback for closing our Window
void windowCloseCallback(GLFWwindow* window)
{
    std::cout << "Goodbye!";
}



//Function for when screen is resized
void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    //Set viewport size to new viewport size. 
    glViewport(0, 0, width, height);
}