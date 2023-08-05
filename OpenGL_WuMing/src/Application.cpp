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
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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
        float positions[] = {
            -0.5,   -0.5f, 0.0f, 0.0f,      // 0
             0.5f,  -0.5f, 1.0f, 0.0f,      // 1
             0.5f,   0.5f, 1.0f, 1.0f,      // 2
            -0.5f,   0.5f, 0.0f, 1.0f      // 3
        };

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

        glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);

        Shader shader{ "res/Shaders/Basic.shader" };
        shader.Bind();
        shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0);
        shader.SetUniformMat4f("u_MVP", proj);
        

        const unsigned int slot = 0;
        Texture texure("res/texture/ChernoLogo.png");
        texure.Bind(slot);
        shader.SetUniform1i("u_Texture", slot);

        /*清除绑定关系*/
        va.UnBind();
        vb.UnBind();
        iB.UnBind();
        shader.UnBind();
 
        Renderer renderer;
        float Color_R = 0.8f;
        float Increase = 0.05f;

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            renderer.Clear();

            shader.Bind();
            shader.SetUniform4f("u_Color", Color_R, 0.5, 0.5, 1.0);

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

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }

    glfwTerminate();
    return 0;
}