#pragma once

#include <sovren/Line.hpp>
#include <sovren/Rect.hpp>

#include <variant>

namespace sovren {

    class Shape {
    public:
        [[nodiscard]]
        static auto fromLine(const Line& l) noexcept -> Shape {
            return { l };
        }

        [[nodiscard]]
        static auto fromRect(const Rect& r) noexcept -> Shape {
            return { r };
        }

        Shape() = default;

        Shape(const Shape&) = default;
        auto operator=(const Shape&) -> Shape& = default;

        Shape(Shape&&) = default;
        auto operator=(Shape&&) -> Shape& = default;

        ~Shape() = default;

        template <typename T>
        [[nodiscard]]
        auto as() const noexcept -> const T* {
            return std::get_if<T>(&data_);
        }

        template <typename T>
        [[nodiscard]]
        auto is() const noexcept -> bool {
            return as<T>() != nullptr;
        }

        [[nodiscard]]
        auto isEmpty() const noexcept -> bool {
            return !is<std::monostate>();
        }

        [[nodiscard]]
        operator bool() const noexcept {
            return !isEmpty();
        }

    private:
        using Data = std::variant<std::monostate, Rect, Line>;

        Shape(Data shape) noexcept
            : data_(shape) {}

        Data data_;
    };

}
