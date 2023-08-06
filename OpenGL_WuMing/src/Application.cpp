#include<GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "Texture/Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"
#include "tests/TestClearColor.h"


#define TEST_PROJECTION_MATRIX 1

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);          //���汾
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);            //�˰汾 ������������ v 3.3 
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);      //���ú��������ļ�
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);          //���ú��������ļ�
   // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


    /* Create a windowed mode window and its OpenGL context */
    //window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);

    //test ����ͶӰ
#if TEST_PROJECTION_MATRIX
    window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
#else
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
#endif //TEST_PROJECTION_MATRIX


    if (!window)
    {
        glfwTerminate(); 
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // ����tick interval
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "Error" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl; 
    {
        // ������Ⱦ͸�����
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        Renderer renderer;
 
        //crate imgui
        ImGui::CreateContext();
        ImGui_ImplGlfwGL3_Init(window, true);
        ImGui::StyleColorsDark();

        test::TestClearColor test;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            renderer.Clear();

            test.OnUpdate(0.0f);
            test.OnRender();

            ImGui_ImplGlfwGL3_NewFrame();
            test.OnImGuiRender();
            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }
    // Cleanup
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}