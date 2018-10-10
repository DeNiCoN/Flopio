#pragma once
#include "Process.h"
#include <iostream>
namespace engine
{
	class DelayProcess : public Process
	{
	private:
		double from;
		double to;
	public:
		DelayProcess(double seconds) : to(seconds) {}
		void VOnUpdate(double delta) {
			from += delta;
			if (from > to)
				end();
		}
	};
}