#include "ProcessManager.h"

namespace engine {
	WeakProcessPtr ProcessManager::attachProcess(SharedProcessPtr process) {
		processList.push_front(process);
		return WeakProcessPtr(process);
	}

	unsigned int ProcessManager::updateProcesses(unsigned long delta) {
		unsigned int success = 0;
		unsigned int fail = 0;
		std::list<SharedProcessPtr>::iterator i = processList.begin();
		while (i != processList.end()) {
			SharedProcessPtr process = *i;
			std::list<SharedProcessPtr>::iterator thisIt = i;
			i++;
			if (process->state == UNINITIALIZED)
			{
				process->VOnInit();
			}

			if (process->state == RUNNING)
			{
				process->VOnUpdate(delta);
			}

			if (process->isDead())
			{
				switch (process->state)
				{
				case ENDED:
				{
					process->VOnEnd();
					SharedProcessPtr next = process->removeNext();
					if (next)
					{
						attachProcess(next);
					}
					else
					{
						success++;
					}
					break;
				}
				case ABORTED:
				{
					process->VOnAbort();
					fail++;
					break;
				}
				case FAILED:
				{
					process->VOnFail();
					fail++;
					break;
				}
				}
				processList.erase(thisIt);
			}
		}
		return (success << 16) | fail;
	}
}