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

        ImGui::Begin("Hello World!");
        ImGui::Text("This is some text.");
        ImGui::SliderFloat("value:", &number, 0, 10);
        ImGui::Text("Squared: %f", number * number);
        ImGui::End();

        ImGui::Render();

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClearColor(0.5, 0.4, 0.3, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        for (int i = 0; i < 50; i++) {
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
