#pragma once

#include <memory>
#include <string>
#include <glow/ref_ptr.h>
#include <glowutils/ScreenAlignedQuad.h>

#include "etc/contextdependant.h"
#include "display/rendering/screenquad.h"
#include "camera/camera.h"


namespace glow {
    class Texture;
    class Program;
    class VertexArrayObject;
    class Buffer;
}

class Camera;
class VoxelRenderer;

/**
 *  Renders a texture from a file on the screen quad
 *  Optionally including a status text
 */
class TextureRenderer : public ContextDependant {
public:
    TextureRenderer(const std::string& file);

    /**
     *  Draw the texture to the screen quad, setting the Z buffer to inf
     */
    void draw();

    /**
     *  Draw the texture, render Voxellancer in the top center and status in the bottom left,
     *  then switch the buffers
     */
    void display(const std::string& status);


protected:
    glow::ref_ptr<glowutils::ScreenAlignedQuad> m_quad;
    std::shared_ptr<VoxelRenderer> m_voxelRenderer;
    std::unique_ptr<Camera> m_camera;
    std::string m_file;

    void initialize();
    virtual void beforeContextDestroy() override;
    virtual void afterContextRebuild() override;
};

