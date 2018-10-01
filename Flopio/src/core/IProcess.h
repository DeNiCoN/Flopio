#pragma once

namespace engine {

	class Process
	{
	public:
		Process* next;

		virtual void VOnInit() = 0;
		virtual void VProcess(int delta) = 0;
		virtual void VOnEND() = 0;
		virtual void VOnFAIL() = 0;
		virtual void VOnABORT() = 0;

		ProcessState getState() { return state; }
	private:
		ProcessState state;
	};

	enum ProcessState
	{
		UNINITIALIZED,
		RUNNING, 
		PAUSED, 
		ENDED,
		FAILED,
		ABORTED
	};

}