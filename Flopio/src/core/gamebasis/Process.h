#pragma once
#include <memory>

namespace engine {

	class Process;
	typedef std::shared_ptr<Process> SharedProcessPtr;
	typedef std::weak_ptr<Process> WeakProcessPtr;

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
		friend class ProcessManager;

	private:
		ProcessState state;
		SharedProcessPtr next;
	public:
		void inline pause() { state = PAUSED; }
		void inline unpause() { state = RUNNING; }
		void inline end() { state = ENDED; }
		void inline fail() { state = FAILED; }
		void inline abort() { state = ABORTED; }

		ProcessState getState() { return state; }
		SharedProcessPtr removeNext()
		{
			SharedProcessPtr r = next;
			next == nullptr;
			return r;
		}
		SharedProcessPtr getNext() { return next; }
		void setNext(SharedProcessPtr next) { this->next = next; }

		bool isDead() { return state == ENDED || state == FAILED || state == ABORTED; }
		bool isRunning() { return state == RUNNING; }
		bool isPaused() { return state == PAUSED; }
		virtual ~Process() {};
	protected:
		virtual void VOnInit() { state = RUNNING; }
		virtual void VOnUpdate(double delta) = 0;
		virtual void VOnEnd() {};
		virtual void VOnFail() {};
		virtual void VOnAbort() {};
	};

}

