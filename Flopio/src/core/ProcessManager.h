#pragma once
#include <list>
#include <iostream>
#include "Process.h"

namespace engine {
	class ProcessManager {
	public:
		~ProcessManager() { processList.clear(); }
		unsigned int updateProcesses(unsigned long delta);
		WeakProcessPtr attachProcess(SharedProcessPtr process);

		unsigned int getProcessCount() { return processList.size(); }
	private:
		std::list<SharedProcessPtr> processList;
	};
}