#pragma once
#include "Actor.h"
#include "..\utils.h"



namespace engine
{
	class Component
	{
	public:
		virtual void update(double delta) {}
		virtual const char* getName() const = 0;
		virtual unsigned int getId() { return hash(getName(), strlen(getName())); }
	};
}