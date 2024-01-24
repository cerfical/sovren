#ifndef RENI_UI_EVENT_LOOP_HEADER
#define RENI_UI_EVENT_LOOP_HEADER

#include <memory>

namespace RENI {

	/**
	 * @brief Implements a system event loop for polling UI events.
	*/
	class UiEventLoop {
	public:

		UiEventLoop();
		~UiEventLoop();

		UiEventLoop(const UiEventLoop&) = delete;
		UiEventLoop& operator=(const UiEventLoop&) = delete;
		
		UiEventLoop(UiEventLoop&&) = default;
		UiEventLoop& operator=(UiEventLoop&&) = default;



		/** @{ */
		/**
		 * @brief Start the loop execution.
		*/
		int run();
		/** @} */



	private:
		struct Impl;
		std::unique_ptr<Impl> m_impl;
	};

}

#endif