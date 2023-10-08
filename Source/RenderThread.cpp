#include "RenderThread.hpp"
#include "RenderWindow.hpp"

#include <condition_variable>
#include <thread>
#include <mutex>

struct RENI::RenderThread::Impl {
	RenderWindow* targetWindow = nullptr;
	std::jthread thread;

	std::condition_variable suspend;
	std::mutex mutex;

	Status status = Status::Stopped;
};

namespace RENI {
	bool RenderThread::updateStatus() {
		std::unique_lock lock(m_impl->mutex);
		if(m_impl->status == Status::Running) {
			return true;
		}

		if(m_impl->status == Status::Suspending) {
			m_impl->status = Status::Suspended;
			lock.unlock();

			m_impl->suspend.notify_one();

			lock.lock();
			m_impl->suspend.wait(lock, [this]() {
				return m_impl->status == Status::Resuming || m_impl->status == Status::Stopping;
			});
		}

		if(m_impl->status == Status::Resuming) {
			m_impl->status = Status::Running;
			lock.unlock();
			m_impl->suspend.notify_one();
			return true;
		}

		if(m_impl->status == Status::Stopping) {
			m_impl->status = Status::Stopped;
			lock.unlock();
			m_impl->suspend.notify_one();
			return false;
		}

		return false;
	}

	void RenderThread::render() {
		auto targetWindow = m_impl->targetWindow;
		while(updateStatus()) {
			auto renderDevice = targetWindow->renderDevice();
			renderDevice->startDraw();
			targetWindow->onRender();
			renderDevice->endDraw();
		}
	}


	RenderThread::RenderThread(RenderWindow* targetWindow)
		: m_impl(std::make_unique<Impl>()) {
		m_impl->targetWindow = targetWindow;
		start();
	}

	RenderThread::~RenderThread() {
		stop();
	}


	void RenderThread::start() {
		if(m_impl->status == Status::Stopped) {
			m_impl->status = Status::Running;
			m_impl->thread = std::jthread(
				[this]() noexcept { this->render(); }
			);
		}
	}

	void RenderThread::stop() {
		// already running or suspended thread can be stopped
		if(m_impl->status == Status::Suspended || m_impl->status == Status::Running) {
			{
				std::unique_lock lock(m_impl->mutex);
				m_impl->status = Status::Stopping;
			}
			m_impl->suspend.notify_one();
			{
				std::unique_lock lock(m_impl->mutex);
				m_impl->suspend.wait(lock, [this]() {
					return m_impl->status == Status::Stopped;
				});
			}
		}
	}


	void RenderThread::suspend() {
		// running thread can be suspended
		if(m_impl->status == Status::Running) {
			std::unique_lock lock(m_impl->mutex);
			m_impl->status = Status::Suspending;
			m_impl->suspend.wait(lock, [this]() {
				return m_impl->status == Status::Suspended;
			});
		}
	}

	void RenderThread::resume() {
		// only a previously suspended thread can be resumed
		if(m_impl->status == Status::Suspended) {
			{
				std::unique_lock lock(m_impl->mutex);
				m_impl->status = Status::Resuming;
			}
			m_impl->suspend.notify_one();
			{
				std::unique_lock lock(m_impl->mutex);
				m_impl->suspend.wait(lock, [this]() {
					return m_impl->status == Status::Running;
				});
			}
		}
	}


	RenderThread::Status RenderThread::status() const noexcept {
		return m_impl->status;
	}
}