#pragma once

#include "error_util.hpp"
#include "string_util.hpp"

#include <Windows.h>

#include <cstddef>
#include <memory>
#include <string>
#include <utility>

namespace sovren::win32 {

    class WndClass {
    public:

        WndClass() = default;

        WndClass(const WndClass&) = delete;
        auto operator=(const WndClass&) -> WndClass& = delete;

        WndClass(WndClass&&) = default;
        auto operator=(WndClass&&) -> WndClass& = default;

        ~WndClass() = default;


        WndClass(std::string name, WNDPROC wndProc)
            : clsName_(std::move(name)) {

            const auto tcName = mbToTc(clsName_);
            WNDCLASSEX wndClass = {
                .cbSize = sizeof(wndClass),
                .style = 0,
                .lpfnWndProc = wndProc,
                .cbClsExtra = 0,
                .cbWndExtra = 0,
                .hInstance = nullptr,
                .hIcon = nullptr,
                .hCursor = nullptr,
                .hbrBackground = nullptr,
                .lpszMenuName = nullptr,
                .lpszClassName = tcName.c_str(),
                .hIconSm = nullptr
            };
            clsAtom_.reset(win32Check(RegisterClassEx(&wndClass)));
        }


        [[nodiscard]]
        auto name() const noexcept -> const std::string& {
            return clsName_;
        }


        [[nodiscard]]
        auto atom() const noexcept -> ATOM {
            return clsAtom_.get().value;
        }


    private:
        struct Atom {

            Atom() = default;

            Atom(std::nullptr_t)
                : Atom(static_cast<ATOM>(0)) {}

            Atom(ATOM value)
                : value(value) {}

            explicit operator bool() const {
                return value != 0;
            }

            ATOM value = {};
        };

        class AtomDeleter {
        public:
            using pointer = Atom;

            void operator()(pointer atom) const {
                win32Check(UnregisterClass(MAKEINTATOM(atom.value), nullptr));
            }
        };

        std::unique_ptr<Atom, AtomDeleter> clsAtom_;
        std::string clsName_;
    };

}
