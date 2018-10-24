#pragma once
#include "../core/gamebasis/App.h"
using namespace engine;
namespace game {
	class Flopio : public App
	{
	public:
		void VOnUpdate(double delta) { std::cout << "inside\n"; }

	private:

	};
}