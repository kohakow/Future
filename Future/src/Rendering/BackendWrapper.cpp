#include "BackendWrapper.hpp"
#include <../../../thirdParty/glad/include/glad/glad.h>
#include <../../../thirdParty/glfw/include/GLFW/glfw3.h>
#include <../../../thirdParty/glm/glm/glm.hpp>
#include <../../../thirdParty/glm/glm/gtc/matrix_transform.hpp>
#include <../../../thirdParty/glm/glm/gtc/type_ptr.hpp>
#include "Shaders.hpp"
#include "Camera.hpp"
#include "Model.hpp"

namespace Future
{
    enum class Backend : std::uint8_t
    {
        OPENGL = 0x01,
        VULKAN = 0x02
    };

    std::string BackendToString(Backend const backend)
    {
        switch (backend)
        {
            case Backend::OPENGL: return "OPENGL";
            case Backend::VULKAN: return "VULKAN";
            default: return "UNKNOWN";
        }
    }

    BackendWrapper::BackendWrapper(Window* window, Backend backend)
    {
        m_window = window;
        m_backend = backend;
        FE_CORE_INFO("Backend selected: " + BackendToString(backend));
        gladLoadGL();
    }

    BackendWrapper::~BackendWrapper()
    {

    }

    // TODO: https://www.youtube.com/watch?v=crOfyWiWxmc

    bool BackendWrapper::Init()
    {
        glViewport(0, 0, 1920, 1080);
        Shaders shaderProgram("Shaders/default.vert", "Shaders/default.frag"); // Generates Shader object using shaders default.vert and default.frag

        // Take care of all the light related things
        glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
        glm::mat4 lightModel = glm::mat4(1.0f);
        lightModel = glm::translate(lightModel, lightPos);

        shaderProgram.Activate();
        glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
        glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

        glEnable(GL_DEPTH_TEST); // Enable depth buffer
        glDepthFunc(GL_LESS); // Depth func/type
        //glEnable(GL_CULL_FACE); // Enable Face Culling
        //glCullFace(GL_FRONT); // Cull front faces
        //glFrontFace(GL_CCW);  // CCW is common

        Camera camera(1920, 1080, glm::vec3(0.0f, 0.0f, 1.0f));

        Model test("bnuy/scene.gltf");

        while (m_window->IsRunning())
        {
            glClearColor(0.01f, 0.01f, 0.01f, 1.0f); // Clear screen with color (black)
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear buffers

            camera.UpdateMatrix(45.0f, 0.1f, 100.0f); // Updates and exports the camera matrix to the Vertex Shader

            test.Draw(shaderProgram, camera);

            m_window->Tick();
        }
        shaderProgram.Delete();
        return true;
    }
}