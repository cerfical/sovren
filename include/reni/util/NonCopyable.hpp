#ifndef RENI_UTIL_NON_COPYABLE_HEADER
#define RENI_UTIL_NON_COPYABLE_HEADER

namespace reni {

	/**
	 * @brief Mixin type to prevent copying objects of another type.
	*/
	struct NonCopyable {

		NonCopyable() = default;
		~NonCopyable() = default;

		NonCopyable(const NonCopyable&) = delete;
		NonCopyable& operator=(const NonCopyable&) = delete;

		NonCopyable(NonCopyable&&) = default;
		NonCopyable& operator=(NonCopyable&&) = default;

	};

}

#endif