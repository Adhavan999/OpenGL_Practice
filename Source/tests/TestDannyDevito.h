#pragma once
#include <tests/Test.h>
#include "Shader.h"
#include "VertexArray.h"
#include "Texture.h"

namespace test 
{
	class TestDannyDevito : public Test
	{
	private:
		Shader shader;
		unsigned int *positions;
		Texture texture[2];

	public:
		TestDannyDevito() {}
		~TestDannyDevito() {}

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

		

	};
}