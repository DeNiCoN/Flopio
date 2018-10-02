#pragma once
#include <list>
#include <iostream>
#include "Process.h"

namespace engine {
	class ProcessManager {
	public:
		unsigned int updateProcesses(unsigned long delta);
		void atachProcess(std::shared_ptr<Process> process);

		unsigned int getProcessCount() { return processList.size(); };
	private:
		std::list<std::shared_ptr<Process>> processList;
	};
}