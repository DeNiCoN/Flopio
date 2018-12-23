#pragma once
#include "../core/gamebasis/App.h"
using namespace engine;
namespace game {
	class Flopio : public App
	{
	public:
		virtual void VOnUpdate(const double delta) override;
		virtual void VOnRender(const double ndelay) override;
		virtual void VOnInit() override;
		Flopio() : App(1) {}
	private:
		Actor ship;
	};
}