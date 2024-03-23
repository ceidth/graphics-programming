// dear imgui: standalone example application for GLFW + OpenGL 3, using programmable pipeline
// If you are new to dear imgui, see examples/README.txt and documentation at the top of imgui.cpp.
// (GLFW is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan graphics context creation, etc.)

#include "imgui.h"
#include "imgui_impl/imgui_impl_glfw.h"
#include "imgui_impl/imgui_impl_opengl3.h"
#include <stdio.h>

#include "Model.h"
#include "Camera.h"
#include "GraphNode.h"

#define IMGUI_IMPL_OPENGL_LOADER_GLAD

// About OpenGL function loaders: modern OpenGL doesn't have a standard header file and requires individual function pointers to be loaded manually. 
// Helper libraries are often used for this purpose! Here we are supporting a few common ones: gl3w, glew, glad.
// You may use another loader/header of your choice (glext, glLoadGen, etc.), or chose to manually implement your own.
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
#include <GL/gl3w.h>    // Initialize with gl3wInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
#include <GL/glew.h>    // Initialize with glewInit()
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
#include <glad/glad.h>  // Initialize with gladLoadGL()
#else
#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
#endif

#include <GLFW/glfw3.h> // Include glfw3.h after our OpenGL definitions
#include <spdlog/spdlog.h>

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int, char**)
{
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions
#if __APPLE__
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 4.3 + GLSL 430
    const char* glsl_version = "#version 430";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    // Initialize OpenGL loader
#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
    bool err = gl3wInit() != 0;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
    bool err = glewInit() != GLEW_OK;
#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
    bool err = !gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
#endif
    if (err)
    {
        spdlog::error("Failed to initialize OpenGL loader!");
        return 1;
    }
    spdlog::info("Successfully initialized OpenGL loader!");

    // Setup Dear ImGui binding
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Setup style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();

    /*--------RESOURCES SET UP--------*/
    Camera camera(1280, 720, glm::vec3(0.0f, 1.0f, 5.0f));
    Shader shader("./res/shaders/basic.vert", "./res/shaders/basic.frag");
    Transform* instancedTrans = new Transform(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
    instancedTrans->setTranslations(200);

    std::vector<glm::vec3> empty = {};

    /*--------SCENE GRAPH SET UP--------*/
    GraphNode root(nullptr, new Transform(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f)), false); //root
    GraphNode housesRoot(nullptr, new Transform(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f)), false); //houses root
    GraphNode house(new Model(CUBE, instancedTrans->translations, true), instancedTrans, true); //domek :3
    GraphNode roof(new Model(ROOF, instancedTrans->translations, true), instancedTrans, true); //dach

    /*--------ASSIGNING CHILDREN--------*/
    root.addChild(new GraphNode(new Model(PLANE, empty), new Transform(glm::vec3(0.0f), glm::vec3(0.0f), glm::vec3(1.0f)), false)); //ziemia
    root.addChild(&housesRoot);

    housesRoot.addChild(&house);
    house.addChild(&roof);

    /*--------LIGHTS--------*/
    glm::vec3 lightPos = glm::vec3(0.0f, 4.0f, 0.0f);

    shader.use();
    shader.setVec3("camPos", camera.Position);



    /*--------BLA BLA BLA--------*/
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    /*--------GLM VARIABLE--------*/
    bool dirEnabled = true;
    glm::vec3 dirDir = glm::vec3(0.5f, -1.0f, 0.5f);
    bool dirVis = true;

    bool pointRot = true;
    bool pointVis = true;
    bool pointEnabled = true;

    glm::vec3 spotDir[] = {glm::vec3(0.0f, -1.0f, -0.5f), glm::vec3(0.0f, -1.0f, 0.5f)};
    bool spotEnabled[] = {true, true};

    /*--------MAIN LOOP--------*/
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


        glfwPollEvents();
        glClearColor(0.78f, 0.87f , 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        int display_w, display_h;
        glfwMakeContextCurrent(window);
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);

        camera.Inputs(window);

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


        {

            ImGui::Begin("Directional light");
            ImGui::SliderFloat3("Light direction", (float*)&dirDir, -1.0f, 1.0f);
            ImGui::Checkbox("enabled", &dirEnabled);
            ImGui::SameLine();
            ImGui::Checkbox("visualize direction", &dirVis);
            ImGui::NewLine();
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        /*--------SHADER VARIABLES--------*/
        shader.use();
        camera.Matrix(45.0f, 0.1f, 1000.0f, shader, "camMatrix");
        shader.setBool("dirEnabled", dirEnabled);
        shader.setVec3("dirDir", dirDir);
        shader.setBool("pointEnabled", pointEnabled);

        /*shader.setVec3Array("spotPos", 2, *spotPos);
        shader.setVec3Array("spotDir", 2, *spotDir);*/

        int spotInt[] = {(int)spotEnabled[0], (int)spotEnabled[1]};
        shader.setIntArray("spotEnabled", 2, spotInt);

        /*--------LIGHTS--------*/
        

        /*--------RENDER--------*/
        root.draw(shader, deltaTime);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwMakeContextCurrent(window);
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}