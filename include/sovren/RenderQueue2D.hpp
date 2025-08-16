#pragma once

#include <sovren/math/Mat3x3.hpp>

#include <sovren/rhi/Context2D.hpp>

#include <sovren/util/Color.hpp>
#include <sovren/util/Point2.hpp>
#include <sovren/util/Size2.hpp>

#include <variant>
#include <vector>

namespace sovren {

    class RenderQueue2D {
    public:
        struct Rect {
            Point2f topLeft;
            Size2f size;

            Mat3x3 transform;
            Color color;
        };

        void drawRect(const Rect& r) {
            commands_.emplace_back(r);
        }

        struct Line {
            Point2f start;
            Point2f end;

            Mat3x3 transform;
            Color color;
        };

        void drawLine(const Line& l) {
            commands_.emplace_back(l);
        }

        void flush(rhi::Context2D& ctx) {
            for(const auto& cmd : commands_) {
                const auto visitor = Overload{
                    [this, &ctx](const Rect& r) {
                        setDrawState(ctx, r.transform, r.color);
                        ctx.drawRect(
                            r.topLeft, { r.topLeft.x + r.size.width, r.topLeft.y + r.size.height }
                        );
                    },

                    [this, &ctx](const Line& l) {
                        setDrawState(ctx, l.transform, l.color);
                        ctx.drawLine(l.start, l.end);
                    },
                };
                std::visit(visitor, cmd);
            }
            commands_.clear();
        }

    private:
        using Command = std::variant<Rect, Line>;

        template <typename... Ts>
        struct Overload : public Ts... {
            using Ts::operator()...;
        };

        void setDrawState(rhi::Context2D& ctx, const Mat3x3& m, Color c) {
            ctx.setTransform({
                { m[0][0], m[0][1], m[1][0], m[1][1], m[2][0], m[2][1] },
            });
            ctx.setOutlineColor(c);
        }

        std::vector<Command> commands_;
    };

}
