#pragma once

namespace reni::rg {
	
	class Line2D;
	class Rect2D;
	class Transform2D;

	class Triangle3D;
	class Transform3D;
	class Camera3D;


	/**
	 * @brief Implements the *Visitor* pattern.
	*/
	class NodeVisitor {
	public:

		/**
		 * @brief Called for all Line2D nodes.
		*/
		virtual void visit(const Line2D& n) = 0;


		/**
		 * @brief Called for all Rect2D nodes.
		*/
		virtual void visit(const Rect2D& n) = 0;


		/**
		 * @brief Called for all Transform2D nodes.
		*/
		virtual void visit(const Transform2D& n) = 0;
		

		/**
		 * @brief Called for all Triangle3D nodes.
		*/
		virtual void visit(const Triangle3D& n) = 0;


		/**
		 * @brief Called for all Transform3D nodes.
		*/
		virtual void visit(const Transform3D& n) = 0;


		/**
		 * @brief Called for all CameraNode nodes.
		*/
		virtual void visit(const Camera3D& n) = 0;


	protected:
		~NodeVisitor() = default;
	};

}