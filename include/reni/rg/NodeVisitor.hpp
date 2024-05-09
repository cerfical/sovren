#pragma once

namespace reni::rg {
	
	class Line2D;
	class Rect2D;
	class Triangle3D;


	class NodeVisitor {
	public:

		virtual void visit(const Line2D& n) = 0;
		virtual void visit(const Rect2D& n) = 0;
		virtual void visit(const Triangle3D& n) = 0;

	protected:
		~NodeVisitor() = default;
	};

}