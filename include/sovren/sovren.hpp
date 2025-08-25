#pragma once

#include <sovren/events/Event.hpp>
#include <sovren/events/EventQueue.hpp>
#include <sovren/events/KeyEvent.hpp>
#include <sovren/events/KeyEventType.hpp>
#include <sovren/events/MouseEvent.hpp>
#include <sovren/events/MouseEventType.hpp>
#include <sovren/events/WindowEvent.hpp>
#include <sovren/events/WindowEventType.hpp>

#include <sovren/log/ConsoleLogger.hpp>
#include <sovren/log/Level.hpp>
#include <sovren/log/Log.hpp>
#include <sovren/log/Logger.hpp>
#include <sovren/log/Message.hpp>
#include <sovren/log/Tags.hpp>

#include <sovren/math/Mat.hpp>
#include <sovren/math/Mat2x2.hpp>
#include <sovren/math/Mat3x3.hpp>
#include <sovren/math/Mat4x4.hpp>
#include <sovren/math/Vec.hpp>
#include <sovren/math/Vec2.hpp>
#include <sovren/math/Vec3.hpp>
#include <sovren/math/Vec4.hpp>
#include <sovren/math/mat_ops.hpp>
#include <sovren/math/util.hpp>
#include <sovren/math/vec_ops.hpp>

#include <sovren/pal/EventPump.hpp>
#include <sovren/pal/Key.hpp>
#include <sovren/pal/MouseButton.hpp>
#include <sovren/pal/Window.hpp>
#include <sovren/pal/WindowHandler.hpp>

#include <sovren/rhi/AccessModes.hpp>
#include <sovren/rhi/BindModes.hpp>
#include <sovren/rhi/Buffer.hpp>
#include <sovren/rhi/BufferDesc.hpp>
#include <sovren/rhi/Context2D.hpp>
#include <sovren/rhi/Context3D.hpp>
#include <sovren/rhi/Device.hpp>
#include <sovren/rhi/Format.hpp>
#include <sovren/rhi/Pipeline.hpp>
#include <sovren/rhi/PipelineDesc.hpp>
#include <sovren/rhi/ShaderCode.hpp>
#include <sovren/rhi/ShaderLang.hpp>
#include <sovren/rhi/ShaderStages.hpp>
#include <sovren/rhi/SwapChain.hpp>
#include <sovren/rhi/Texture.hpp>
#include <sovren/rhi/VertexAttr.hpp>
#include <sovren/rhi/Viewport.hpp>

#include <sovren/sg/CameraNode3D.hpp>
#include <sovren/sg/MeshNode3D.hpp>
#include <sovren/sg/RootNode.hpp>
#include <sovren/sg/SceneNode.hpp>
#include <sovren/sg/SceneNode2D.hpp>
#include <sovren/sg/SceneNode3D.hpp>
#include <sovren/sg/WindowNode.hpp>

#include <sovren/util/AggregateN.hpp>
#include <sovren/util/Color.hpp>
#include <sovren/util/ConstructibleFromN.hpp>
#include <sovren/util/Handle.hpp>
#include <sovren/util/Point2.hpp>
#include <sovren/util/Size2.hpp>

#include <sovren/Camera.hpp>
#include <sovren/Input.hpp>
#include <sovren/Line.hpp>
#include <sovren/Mesh.hpp>
#include <sovren/OrthoCamera.hpp>
#include <sovren/PerspCamera.hpp>
#include <sovren/Rect.hpp>
#include <sovren/RenderItem2D.hpp>
#include <sovren/RenderItem3D.hpp>
#include <sovren/RenderList.hpp>
#include <sovren/RenderServer.hpp>
#include <sovren/Shape.hpp>
#include <sovren/Transform2D.hpp>
#include <sovren/Transform3D.hpp>
#include <sovren/Window.hpp>
