#ifndef RENI_WINDOW_HEADER
#define RENI_WINDOW_HEADER

#include "Size2D.hpp"
#include "Point2D.hpp"

#include "Keys.hpp"
#include "MouseButtons.hpp"

#include <memory>
#include <string_view>
#include <string>

namespace reni {

	/**
	 * @brief Simple rectangular window on the screen.
	*/
	class Window {
	public:
		
		Window();
		virtual ~Window();

		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;



		/** @{ */
		/**
		 * @brief Set a new client area dimensions.
		*/
		void setSize(Size2D s);
		


		/**
		 * @brief Dimensions of the window's client area.
		*/
		Size2D size() const;

		

		/**
		 * @brief Width of the client area.
		*/
		int width() const {
			return size().width;
		}



		/**
		 * @brief Height of the client area.
		*/
		int height() const {
			return size().height;
		}



		/**
		 * @brief Set a new window title.
		*/
		void setTitle(std::string_view title);



		/**
		 * @brief Text from the window's title bar.
		*/
		std::string title() const;



		/**
		 * @brief Toggle visible/hidden state.
		*/
		void setVisible(bool visible);

		

		/**
		 * @brief Make the window visible on the screen.
		*/
		void show() {
			setVisible(true);
		}



		/**
		 * @brief Hide the window.
		*/
		void hide() {
			setVisible(false);
		}



		/**
		 * @brief Check if the window is currently visible.
		*/
		bool isVisible() const;
		/** @} */

		

		/** @{ */
		
		/**
		 * @brief Check the pressed/released state of a key.
		*/
		bool keyState(Keys key) const;



		/**
		 * @brief Check the pressed/released state of a mouse button.
		*/
		bool buttonState(MouseButtons button) const;



		/**
		 * @brief Enable/disable mouse input capture.
		*/
		void toggleMouseCapture();



		/**
		 * @brief Platform-native handle for the window.
		*/
		void* nativeHandle() const;
		/** @} */



	protected:

		/**
		 * @brief Called when the window has been closed.
		*/
		virtual void onClose();

		

		/**
		 * @brief Called when the window has been resized.
		*/
		virtual void onResize(Size2D newSize, Size2D oldSize)
		{ }



		/**
		 * @brief Called when a key has been pressed.
		*/
		virtual void onKeyPress(Keys key)
		{ }
		


		/**
		 * @brief Called when a key has been released.
		*/
		virtual void onKeyRelease(Keys key)
		{ }
		
		
		
		/**
		 * @brief Called when a mouse button has been pressed.
		*/
		virtual void onMousePress(MouseButtons button)
		{ }

		
		
		/**
		 * @brief Called when a mouse button has been released.
		*/
		virtual void onMouseRelease(MouseButtons button)
		{ }



		/**
		 * @brief Called when the mouse has been moved.
		*/
		virtual void onMouseMove(Point2D newPos, Point2D oldPos)
		{ }



	private:
		struct ImplBase; struct Impl;
		std::unique_ptr<Impl> m_impl;
	};

}

#endif