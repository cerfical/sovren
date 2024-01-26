#ifndef RENI_LOCKED_PTR_HEADER
#define RENI_LOCKED_PTR_HEADER

#include <mutex>

namespace RENI {

	/**
	 * @brief Provides exclusive access to a mutex-protected resource.
	*/
	template <typename T>
	class LockedPtr {
	public:

		LockedPtr(T* ptr, std::mutex& mutex) noexcept
			: m_mutex(&mutex), m_ptr(ptr) {
			m_mutex->lock();
		}

		LockedPtr(const LockedPtr&) = delete;
		LockedPtr& operator=(const LockedPtr&) = delete;

		~LockedPtr() {
			m_mutex->unlock();
		}


		/** @{ */
		T* operator->() const noexcept {
			return m_ptr;
		}
		/** @} */


	private:
		std::mutex* m_mutex = {};
		T* m_ptr = {};
	};

}

#endif