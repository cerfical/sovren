#pragma once

namespace sovren {

    class Line2D;
    class Rect2D;

    class MeshNode3D;
    class Camera3D;


    /**
     * @brief Implements the *Visitor* pattern.
     */
    class NodeVisitor {
    public:

        NodeVisitor(const NodeVisitor&) = default;
        auto operator=(const NodeVisitor&) -> NodeVisitor& = default;

        NodeVisitor(NodeVisitor&&) = default;
        auto operator=(NodeVisitor&&) -> NodeVisitor& = default;


        /**
         * @brief Called for all Line2D nodes.
         */
        virtual void visit(const Line2D& n) = 0;


        /**
         * @brief Called for all Rect2D nodes.
         */
        virtual void visit(const Rect2D& n) = 0;


        /**
         * @brief Called for all MeshNode3D nodes.
         */
        virtual void visit(const MeshNode3D& n) = 0;


        /**
         * @brief Called for all CameraNode nodes.
         */
        virtual void visit(const Camera3D& n) = 0;


    protected:
        NodeVisitor() = default;
        ~NodeVisitor() = default;
    };

}
