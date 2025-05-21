#define STB_IMAGE_IMPLEMENTATION

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Shaders/shader_s.h>
#include <iostream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <filesystem>
#include <stb_image.h>


void processInput(GLFWwindow *window);
float mixFloat = 0.5f;


int main()
{
    // Initialize GLFW
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Configure GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a window
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Initialize GLAD (must be done after creating an OpenGL context)
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Set viewport
    glViewport(0, 0, 800, 600);

    Shader ourShader("C:/Users/Utilisateur/Documents/GitHub/OpenGLProjectAdam/src/shaders/shader.vs", "C:/Users/Utilisateur/Documents/GitHub/OpenGLProjectAdam/src/shaders/shader.fs");
     
    
    // Ici on créer le Vertex Shader
    

    float vertices[] = {
      // positions        // colors          // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };

    unsigned int indices[] = { // note that we start from 0!
        0, 1, 3, // first triangle
        1, 2, 3 // second triangle
    };
        
    unsigned int VBOs[1],VAOs[1], EBOs[1]; // On créer un int VBO, enft c juste un ID jcrois? Vertex Buffer Object
    glGenVertexArrays(1, VAOs);
    glGenBuffers(1, VBOs);
    glGenBuffers(1,EBOs);

    glBindVertexArray(VAOs[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]); // La on bind vbo à arraybuffer comme ça quand on envoi des valeurs dans arrayBuffer bah il les envoies à l'adressede VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),(void*)0); //le 0 c'est la localisation, le 3 c'est le nombre de valeurs, truc float c pour dire le type, le false c pour si on normalize ou pas, ensuite l'autre c pour definir la taille de chaque tranche d'info, genere là c'est 3*4bytes, et après 0 c'est le truc de départ.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float),(void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    

    unsigned int textures[2];
    glGenTextures(2, textures);
    glBindTexture(GL_TEXTURE_2D,textures[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    unsigned char *data = stbi_load("C:/Users/Utilisateur/Documents/GitHub/OpenGLProjectAdam/resources/textures/container.jpg",&width,&height,&nrChannels,0);
    if(data)
    {   
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout <<  "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


    glBindTexture(GL_TEXTURE_2D,textures[1]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    data = stbi_load("C:/Users/Utilisateur/Documents/GitHub/OpenGLProjectAdam/resources/textures/pokeball.png",&width,&height,&nrChannels,0);
    if(data)
    {   
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout <<  "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    ourShader.use(); // don’t forget to activate the shader first!
    ourShader.setInt("texture1", 0); 
    ourShader.setInt("texture2", 1); 
    // ourShader.setFloat("mixFloat",0.5f);


    //glm shit

   
    
    // Main render loop
    while (!glfwWindowShouldClose(window))
    {
        // Swap buffers and poll events

       
        processInput(window); //Truc qui vérif si on press espace

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // Couleur du clear
        glClear(GL_COLOR_BUFFER_BIT);   // Lancement du clear -> remplace tt les couleur par celle du dessus
        
        // float timeValue = glfwGetTime();
        // float greenValue = (sin(timeValue) / 2.0f + 0.5f);
        // int vertexColorLocation = glGetUniformLocation(shaderProgram,"ourColor");
        // glUniform4f(vertexColorLocation,0.0f,greenValue,0.0f,1.0f);
        
        // Bind textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textures[1]);


        // First draw
        glm::mat4 trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
        trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

        glBindVertexArray(VAOs[0]);
        ourShader.setMat4("transform", trans);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Second draw
        trans = glm::mat4(1.0f);
        trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
        float scaleValue = sin(glfwGetTime()/2 +0.5f);
        trans = glm::scale(trans, glm::vec3(scaleValue, scaleValue, 1.0f));
        ourShader.setMat4("transform", trans);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
            
        glfwSwapBuffers(window); // Alors ça jcrois ça permet de swap les buffers à chaque frame, genre 1-2-1
        glfwPollEvents(); // jsp ça apparement ça enregistre les events du genre t'appuies sur un truc?? 

    }

    glDeleteVertexArrays(1, &VAOs[1]);
    glDeleteBuffers(1, &VBOs[1]);
    glDeleteBuffers(1, &EBOs[1]);
    // Clean up resources
    glfwTerminate(); // Jsp ça clean la mémoire?
    
    return 0;
}

void processInput(GLFWwindow *window) // Ca c'est plutot explicite
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if(glfwGetKey(window,  GLFW_KEY_DOWN) == GLFW_PRESS)
        mixFloat -= 0.001f;
        if(mixFloat<=0.0f)
            mixFloat = 0.0f;
    if(glfwGetKey(window,  GLFW_KEY_UP) == GLFW_PRESS)
        mixFloat += 0.001f;
        if(mixFloat>=1.0f)
            mixFloat = 1.0f;
}

