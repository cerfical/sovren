#pragma once

#include <string>
#include <utility>

namespace sovren {

    /**
     * @brief Used as a distinguishing factor for different log messages.
     */
    class LogTag {
    public:

        /**
         * @brief Construct an empty tag.
         */
        LogTag() noexcept = default;


        /**
         * @brief Construct a new tag from a string.
         */
        explicit LogTag(std::string value) noexcept
            : value_(std::move(value)) {}


        /**
         * @brief Check if the tag is valid.
         */
        operator bool() const noexcept {
            return !isEmpty();
        }


        /**
         * @brief Check if the tag doesn't contain a useful value.
         */
        bool isEmpty() const noexcept {
            return value().empty();
        }


        /**
         * @brief String representation of the tag.
         */
        const std::string& value() const noexcept {
            return value_;
        }


    private:
        std::string value_;
    };

}
