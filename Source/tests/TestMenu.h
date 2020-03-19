#pragma once
#include "Test.h"

namespace test
{
	class TestMenu : public Test
	{
	private:
	public:
		TestMenu();
		~TestMenu() override;

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	};
}