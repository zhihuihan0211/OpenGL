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

#define TEST_PROJECTION_MATRIX 1

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* */
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);          //主版本
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);            //此版本 。合起来就是 v 3.3 
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);      //设置核心配置文件
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);          //设置核心配置文件
   // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);


    /* Create a windowed mode window and its OpenGL context */
    //window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);

    //test 矩阵投影
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

    // 设置tick interval
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "Error" << std::endl;

    std::cout << glGetString(GL_VERSION) << std::endl; 
    {
#if TEST_PROJECTION_MATRIX
    float positions[] = {
       100.0f,  100.0f, 0.0f, 0.0f,      // 0
       200.0f,  100.0f, 1.0f, 0.0f,      // 1
       200.0f,  200.0f, 1.0f, 1.0f,      // 2
       100.0f,  200.0f, 0.0f, 1.0f      // 3
    };
#else
    float positions[] = {
            -0.5,   -0.5f, 0.0f, 0.0f,      // 0
             0.5f,  -0.5f, 1.0f, 0.0f,      // 1
             0.5f,   0.5f, 1.0f, 1.0f,      // 2
            -0.5f,   0.5f, 0.0f, 1.0f      // 3
     };
#endif // TEST_PROJECTION_MATRIX
        unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        // 启用渲染透明混合
        GLCall(glEnable(GL_BLEND));
        GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

        VertexArray va;
        VertexBuffer vb(positions, 4 * 4 * sizeof(float));

        VertexBufferLayout layout;
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);
        IndexBuffer iB(indices, 6);
#if TEST_PROJECTION_MATRIX
        glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
        //debug 
        glm::vec4 vp(100.0f, 100.0f, 0.0f, 1.0f);
        
#if simulateCalcByCPU 0
    glm::vec4 Result = proj * vp;
#endif //simulateCalcByCPU
#else
        glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);
#endif // TEST_PROJECTION_MATRIX

        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100.0f, 0.0f, 0.0f));
        //glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200.0f, 200.0f, 0.0f));

        //glm::mat4 mvp = proj * view * model; // this is MVP model (model view projection)

        Shader shader{ "res/Shaders/Basic.shader" };

        const unsigned int slot = 0;
        Texture texure("res/texture/ChernoLogo.png");
        texure.Bind(slot);

        shader.Bind();
        shader.SetUniform1i("u_Texture", slot);

        /*清除绑定关系*/
        va.UnBind();
        vb.UnBind();
        iB.UnBind();
        shader.UnBind();
 
        //crate imgui
        ImGui::CreateContext();
        ImGui_ImplGlfwGL3_Init(window, true);
        // Setup style
        ImGui::StyleColorsDark();


        Renderer renderer;
        float Color_R = 0.8f;
        float Increase = 0.05f;

        ///******************************************/
        //bool show_demo_window = true;
        //bool show_another_window = false;
        //ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
        ///*********************************************/

        glm::vec3 translation(200.0f, 200.0f, 0.0f);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            ImGui_ImplGlfwGL3_NewFrame();

            /*dynamic change mpv model !*/
            glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
            glm::mat4 mvp = proj * view * model; // this is MVP model (model view projection)

            shader.Bind();
            shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0);
            shader.SetUniformMat4f("u_MVP", mvp);

            renderer.Draw(va, iB, shader);


            if (Color_R > 1.0f)
            {
                Increase = -0.05f;
            }
            else if (Color_R < 0.0f)
            {
                Increase = 0.05f;
            }

            Color_R += Increase;

            {
                // Display some text (you can use a format string too)
                ImGui::SliderFloat3("translation", &translation.x, 0.0f, 960.0f); 
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            }

            ImGui::Render();
            ImGui_ImplGlfwGL3_RenderDrawData(ImGui::GetDrawData());

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    // Cleanup
    ImGui_ImplGlfwGL3_Shutdown();
    ImGui::DestroyContext();
    glfwTerminate();
    return 0;
}