#pragma once
#include <tests/Test.h>
#include "Shader.h"
#include "VertexArray.h"
#include "Texture.h"
#include "VertexBufferLayout.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "imgui//imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


namespace test
{
	class TestDannyDevito : public Test
	{
	private:
		unsigned int indices[6];
		float positions[20];

		Renderer renderer;
		Shader shader;
		Texture texture1, texture2;
		VertexArray va;
		VertexBuffer vb;
		VertexBufferLayout layout;
		IndexBuffer ib;

		float u_TMix;
		glm::vec3 v_Translate,v_Scale, v_RotationPoint;
		glm::mat4 Translation, Scale, Rotation, InvRotation, m_model, m_view, m_proj, m_MVP;
		float v_Rotate;
		bool RotateAroundCenter;

	public:
		TestDannyDevito();
		~TestDannyDevito();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}