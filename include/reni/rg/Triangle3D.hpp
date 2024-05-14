#pragma once

#include "../util/types.hpp"
#include "../math/types.hpp"

#include "RenderNode.hpp"

#include <array>
#include <span>

namespace reni::rg {

	/**
	 * @brief Describes a plain triangle in 3D space.
	*/
	class Triangle3D : public RenderNode {
	public:
		
		Triangle3D(Vec3 p1, Vec3 p2, Vec3 p3) noexcept
			: m_points{ p1, p2, p3 } {}


		void accept(NodeVisitor& visitor) const override;


		std::span<const Vec3, 3> points() const noexcept {
			return m_points;
		}


	private:
		std::array<Vec3, 3> m_points;
	};

}