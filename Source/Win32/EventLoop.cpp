#include "EventLoop.hpp"
#include "MessageQueue.hpp"

namespace {
	RENI::MessageQueue messageQueue;
}

namespace RENI {
	void EventLoop::Quit(int exitCode) {
		messageQueue.PostQuitMessage(exitCode);
	}


	bool EventLoop::HandleInput() {
		messageQueue.DispatchMessages();
		return !messageQueue.IsQuitMessage();
	}

	void EventLoop::Wait() {
		messageQueue.Wait();
	}


	int EventLoop::GetExitCode() const {
		return messageQueue.GetExitCode();
	}

	int EventLoop::Run() {
		while(HandleInput()) {
			Wait();
		}
		return GetExitCode();
	}
}