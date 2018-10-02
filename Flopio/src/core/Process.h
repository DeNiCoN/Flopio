#pragma once

namespace engine {

	enum ProcessState
	{
		UNINITIALIZED,
		RUNNING,
		PAUSED,
		ENDED,
		FAILED,
		ABORTED
	};

	class Process
	{
	private:
		ProcessState state;
	public:
		Process* next;
		void inline pause() { state == PAUSED; };
		void inline unpause() { state == RUNNING; };

		ProcessState getState() { return state; }
		bool isDead() { return state == ENDED || state == FAILED || state == ABORTED; };
		bool isRunning() { return state == RUNNING; };
		bool isPaused() { return state == PAUSED; };
		virtual ~Process() {};
	protected:
		virtual void VOnInit() { state = RUNNING; };
		virtual void VOnUpdate(int delta) = 0;
		virtual void VOnEnd() {};
		virtual void VOnFail() {};
		virtual void VOnAbort() {};
	};

}

