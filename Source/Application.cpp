#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "VertexBufferLayout.h"
#include "Texture.h"
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(700, 700, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1); //updates at monitors refresh rate

	/* Initializing GLEW*/
	if (glewInit() != GLEW_OK)
	{
		std::cout << "ERROR";
	}

	std::cout << glGetString(GL_VERSION) << std::endl;

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	// Setup Platform/Renderer bindings
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init();
	// Setup Dear ImGui style
	ImGui::StyleColorsDark();


	{
		float positions[] = {
		  //vector positions                texture coordinates
			 0.0f,    0.0f,    0.0f,			0.0f, 0.0f,
			 100.0f,  0.0f,    0.0f,			1.0f, 0.0f,
			 100.0f,  100.0f,  0.0f,			1.0f, 1.0f,
			 0.0f,    100.0f,  0.0f,			0.0f, 1.0f
		};

		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		VertexArray va;
		VertexBuffer vb(positions, 5 * 4 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(3);
		layout.Push<float>(2);

		va.AddBuffer(vb, layout);

		IndexBuffer ib(indices, 6);

		glm::mat4 m_view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		glm::mat4 m_proj = glm::ortho(0.0f, 700.0f, 0.0f, 700.0f, -1.0f, 1.0f);
				
		Shader shader("Resources/shaders/Basic.shader");
		shader.Bind();
			
		//Sets uniform values in shader
		//shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);

		Texture texture("Resources/Textures/danny.png");
		Texture texture2("Resources/Textures/danny2.png");
		texture2.Bind(1);
		texture.Bind();

		shader.SetUniform1i("u_Texture1", 0);
		shader.SetUniform1i("u_Texture2", 1);

		va.Unbind();
		vb.Unbind();
		ib.Unbind();;
		shader.Unbind();

		Renderer renderer;

		float u_TMix = 0.0f;
		glm::vec3 v_Translate(0, 0, 0);
		glm::vec3 v_Scale(1, 1, 1);
		glm::vec3 v_RotationPoint(0, 0, 0);
		glm::mat4 Translation,Scale,Rotation,InvRotation,m_model(1.0f);
		float v_Rotate(0.0f);
		bool RotateAroundCenter = true;

		/*---------------------------WINDOW LOOP-----------------------------*/
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			renderer.Clear();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			renderer.Draw(va, ib, shader);

			Translation = glm::translate(glm::mat4(1.0f),v_Translate);
			Scale = glm::scale(glm::mat4(1.0f), v_Scale);
			Rotation = glm::translate(glm::mat4(1.0f), v_RotationPoint - v_Translate);
			InvRotation = glm::inverse(Rotation);
			Rotation = glm::rotate(Rotation, glm::radians(v_Rotate), glm::vec3(0, 0, 1));
			Rotation = Rotation * InvRotation;
			
			m_model = Translation * Scale * Rotation;

			glm::mat4 m_MVP = m_proj * m_view * m_model;
			shader.SetUniformMat4f("u_MVP", m_MVP);

			shader.SetUniform1f("u_TMix", u_TMix);

			{
				ImGui::Begin("My Transformation  Window"); 

				ImGui::Text("Shift the slider to interpolate between the 2 textures");
				ImGui::SliderFloat("Texture Interpolation", &u_TMix, 0.0f, 1.0f);

				ImGui::Text("Set the scale values for X and Y - (%.2f,%.2f)", v_Scale.x, v_Scale.y);
				ImGui::InputFloat2("Scale", &v_Scale.x);

				ImGui::Text("Set the translation values for x and y - (%.2f,%.2f)", v_Translate.x, v_Translate.y);
				ImGui::InputFloat2("Translate", &v_Translate.x);

				ImGui::Checkbox("Rotate Around Center of Texture ", &RotateAroundCenter);

				if (!RotateAroundCenter)
				{
					ImGui::Text("Set the point to rotate around for x and y - (%.2f,%.2f)", v_RotationPoint.x, v_RotationPoint.y);
					ImGui::InputFloat2("Rotate Around", &v_RotationPoint.x);
				}
				else
				{
					v_RotationPoint = { v_Translate.x + 50, v_Translate.y + 50,0 };
				}

				ImGui::Text("Set the rotation value - (%.2f)", v_Rotate);
				ImGui::SliderFloat("Rotate", &v_Rotate, 0, 360);

				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				ImGui::End();
			}



			/*Render GUI layout*/
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
		/* ---------------------------***********----------------------------- */
	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}