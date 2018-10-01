#pragma once

namespace engine {

	class Process
	{
	public:
		virtual void VOnInit() = 0;
		virtual void VProcess(int delta) = 0;
		virtual void VOnKill() = 0;
		virtual Process* VNext() = 0;
	private:

	};

	enum ProcessState
	{
		RUNNING, INIT, PAUSED, DEAD
	};

}