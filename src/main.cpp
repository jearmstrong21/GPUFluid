#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <gen/shader_simple_fast_init_frag.h>
#include <gen/shader_simple_fast_update_frag.h>
#include <gen/shader_simple_fast_display_frag.h>
#include <gen/shader_copy_frag.h>
#include "Computation.h"

void myErrorCallback(int error_code, const char* description) {
    std::cout << "GLFW error code " << error_code << ": " << description << std::endl;
}

int main() {
    glfwSetErrorCallback(myErrorCallback);
    glfwInit();
    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, true);
    glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, false);

    int W = 800;
    int H = 800;

    GLFWwindow *window = glfwCreateWindow(W, H, "GPU Fluid", nullptr, nullptr);
    // W *= 2;
    // H *= 2;

    glfwMakeContextCurrent(window);
    gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
    glfwSwapInterval(1);
    glfwShowWindow(window);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");

    float vorticityConfinement;
    float viscosity;

    const int MAX_SOURCE_COUNT = 10;

    int sourceCount;
    float sourceLoc[MAX_SOURCE_COUNT * 2];
    float sourceDir[MAX_SOURCE_COUNT * 2];
    float sourceSize[MAX_SOURCE_COUNT];
    float sourceStrength[MAX_SOURCE_COUNT];

    auto templateOne = [&]() {
        vorticityConfinement = 0.11;
        viscosity = 0.55;

        sourceCount = 1;

        sourceLoc[0] = .1, sourceLoc[1] = .5;
        sourceDir[0] = 1; sourceDir[1] = 0;
        sourceSize[0] = 2.0;
        sourceStrength[0] = 4.0;

        sourceLoc[2] = .9; sourceLoc[3] = .5;
        sourceDir[2] = 1; sourceDir[3] = 3.1415;
        sourceSize[1] = 2.0;
        sourceStrength[1] = 4.0;
    };

    auto templateTwo = [&]() {
        vorticityConfinement = 0.067;
        viscosity = 0.550;

        sourceCount = 2;

        sourceLoc[0] = 0.760; sourceLoc[1] = 0.500;
        sourceDir[0] = 4.000; sourceDir[1] = 6.283;
        sourceSize[0] = 1.521;
        sourceStrength[0] = 8.000;

        sourceLoc[2] = 0.083; sourceLoc[3] = 0.828;
        sourceDir[2] = 1.000; sourceDir[3] = 6.283;
        sourceSize[1] = 2.000;
        sourceStrength[1] = 8.000;
    };

    templateOne();

    Computation simple_fast_init(W, H, shader_simple_fast_init_frag, shader_simple_fast_init_frag_len);
    Computation simple_fast_update(W, H, shader_simple_fast_update_frag, shader_simple_fast_update_frag_len);
    Computation simple_fast_display(W, H, shader_simple_fast_display_frag, shader_simple_fast_display_frag_len);
    Computation copy(W, H, shader_copy_frag, shader_copy_frag_len);

    ImageData data(W, H, false);
    ImageData temp(W, H, false);

    simple_fast_init.run(&data);

    float number = 4;

    while (!glfwWindowShouldClose(window)) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Simulation settings", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove);
        ImGui::SetWindowPos({ 5, 5 });
        ImGui::SliderFloat("Vorticity confinement", &vorticityConfinement, 0.01, 0.2);
        ImGui::SliderFloat("Viscosity", &viscosity, 0.2, 1.0);

        if (ImGui::CollapsingHeader("Templates")) {
            if (ImGui::Button("1")) {
                templateOne();
            }
            if (ImGui::Button("2")) {
                templateTwo();
            }
        }

        if (ImGui::CollapsingHeader("Souces")) {
            for(int i = 0; i < sourceCount; i++) {
                // ImGui::BeginChild(("#" + std::to_string(i + 1)).c_str(), { 0, 0 }, ImGuiChildFlags_AlwaysAutoResize | ImGuiChildFlags_AutoResizeX | ImGuiChildFlags_AutoResizeY);
                ImGui::PushID(("#" + std::to_string(i + 1)).c_str());
                if (ImGui::CollapsingHeader(("#" + std::to_string(i + 1)).c_str())) {
                    ImGui::SliderFloat("x", &sourceLoc[i*2], 0, 1);
                    ImGui::SliderFloat("y", &sourceLoc[i*2+1], 0, 1);
                    ImGui::SliderFloat("|V|", &sourceDir[i*2], 0, 4);
                    ImGui::SliderFloat("arg(V)", &sourceDir[i*2+1], 0, 6.28318);
                    ImGui::SliderFloat("Size", &sourceSize[i], 0, 4);
                    ImGui::SliderFloat("Strength", &sourceStrength[i], 0, 8);
                    if (ImGui::Button("-")) {
                        sourceCount--;
                        for(int j = i; j < sourceCount; j++) {
                            sourceLoc[j*2] = sourceLoc[j*2+2];
                            sourceLoc[j*2+1] = sourceLoc[j*2+3];
                            sourceDir[j*2] = sourceDir[j*2+2];
                            sourceDir[j*2+1] = sourceDir[j*2+3];
                            sourceSize[j*2] = sourceSize[j*2+1];
                            sourceStrength[j*2] = sourceSize[j*2+1];
                        }
                        ImGui::PopID();
                        break;
                    }
                }
                ImGui::PopID();
            }
            if (sourceCount < MAX_SOURCE_COUNT) {
                if (ImGui::Button("+")) {
                    sourceLoc[sourceCount*2]=0.5;
                    sourceLoc[sourceCount*2+1]=0.5;
                    sourceDir[sourceCount*2]=1;
                    sourceDir[sourceCount*2+1]=(rand()%100)/100.0*6.28318;
                    sourceSize[sourceCount] = 2.0;
                    sourceStrength[sourceCount] = 4.0;
                    sourceCount++;
                }
            }
        }
        ImGui::End();

        ImGui::Render();

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClearColor(0.5, 0.4, 0.3, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        for (int i = 0; i < 50; i++) {
            simple_fast_update.uniform("viscosity", viscosity);
            simple_fast_update.uniform("vorticityConfinement", vorticityConfinement);
            simple_fast_update.uniform("sourceCount", sourceCount);
            simple_fast_update.uniform("sourceLoc", sourceLoc, sourceCount, 2);
            simple_fast_update.uniform("sourceDir", sourceDir, sourceCount, 2);
            simple_fast_update.uniform("sourceSize", sourceSize, sourceCount, 1);
            simple_fast_update.uniform("sourceStrength", sourceStrength, sourceCount, 1);
            simple_fast_update.run(&temp);
            copy.uniform("source", temp);
            copy.run(&data);
        }

        simple_fast_display.uniform("tex", data);
        simple_fast_display.run(nullptr);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
