#include "TestDannyDevito.h"

namespace test
{
	TestDannyDevito::TestDannyDevito()
		:indices{
			0, 1, 2,
			2, 3, 0
		},
		positions{
		//vector positions                texture coordinates
		   0.0f,    0.0f,    0.0f,			0.0f, 0.0f,
		   100.0f,  0.0f,    0.0f,			1.0f, 0.0f,
		   100.0f,  100.0f,  0.0f,			1.0f, 1.0f,
		   0.0f,    100.0f,  0.0f,			0.0f, 1.0f
		},
		shader{ "Resources/shaders/Basic.shader" },
		texture1{ "Resources/Textures/danny.png" },
		texture2{ "Resources/Textures/danny2.png" },
		va(),
		vb{ positions, 5 * 4 * sizeof(float) },
		layout(),
		ib{ (unsigned int*)indices, 6 },
		u_TMix(0.0f), 
		v_Translate(0, 0, 0), v_Scale(1, 1, 1), v_RotationPoint(0, 0, 0),
		Translation(0.0f), Scale(1.0f), Rotation(0.0f), InvRotation(0.0f), m_model(1.0f),
		m_view{ glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0)) },
		m_proj{ glm::ortho(0.0f, 700.0f, 0.0f, 700.0f, -1.0f, 1.0f) },
		m_MVP(1.0f),
		v_Rotate(0.0f), 
		RotateAroundCenter(true)
	{
		layout.Push<float>(3);
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);
		shader.Bind();
		texture2.Bind(1);
		texture1.Bind();
		shader.SetUniform1i("u_Texture1", 0);
		shader.SetUniform1i("u_Texture2", 1);
	}

	TestDannyDevito::~TestDannyDevito()
	{
	}

	void TestDannyDevito::OnUpdate(float deltaTime)
	{
		Translation = glm::translate(glm::mat4(1.0f), v_Translate);
		Scale = glm::scale(glm::mat4(1.0f), v_Scale);
		Rotation = glm::translate(glm::mat4(1.0f), v_RotationPoint - v_Translate);
		InvRotation = glm::inverse(Rotation);
		Rotation = glm::rotate(Rotation, glm::radians(v_Rotate), glm::vec3(0, 0, 1));
		Rotation = Rotation * InvRotation;

		m_model = Translation * Scale * Rotation;
		m_MVP = m_proj * m_view * m_model;

		shader.SetUniformMat4f("u_MVP", m_MVP);
		shader.SetUniform1f("u_TMix", u_TMix);
	}

	void TestDannyDevito::OnRender()
	{
		renderer.Clear();

		renderer.Draw(va, ib, shader);
	}

	void TestDannyDevito::OnImGuiRender()
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
}