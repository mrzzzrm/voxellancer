#include "gameplayscene.h"

#include "glow/FrameBufferObject.h"

#include "camera/camera.h"
#include "camera/camerahead.h"

#include "display/rendering/framebuffer.h"
#include "display/rendering/renderpipeline.h"
#include "display/rendering/blitter.h"
#include "display/rendering/buffernames.h"
#include "display/rendering/starfield.h"
#include "display/rendering/worldtreerenderer.h"

#include "gamestate/gameplay/gameplay.h"

#include "geometry/viewport.h"

#include "sound/soundmanager.h"

#include "ui/hud/hud.h"
#include "ui/voxelfont.h"

#include "voxel/voxelrenderer.h"
#include "voxeleffect/voxelparticleengine.h"

#include "world/world.h"
#include "worldobject/worldobject.h"

#include "display/rendering/skybox.h"
#include "player.h"


GamePlayScene::GamePlayScene(GamePlay& gamePlay):
    m_gamePlay(gamePlay),
    m_voxelRenderer(VoxelRenderer::instance()),
    m_outputBlitter(new Blitter()),
    m_renderPipeline(RenderPipeline::getDefault()),
    m_starField(std::make_shared<Starfield>()),
    m_worldTreeRendererEnabled(false),
    m_worldTreeRenderer(new WorldTreeRenderer()),
    m_framebuffer(nullptr),
    m_currentOutputBuffer(0),
    m_defaultLightDir("vfx.lightdir")
{
    m_renderPipeline->add(m_starField, 0);
}

GamePlayScene::~GamePlayScene() = default;

void GamePlayScene::draw(const Camera& camera, glow::FrameBufferObject* target, const Viewport& destinationViewport, EyeSide side) const {
    if (m_framebuffer == nullptr) {
        m_framebuffer.reset(new FrameBuffer(m_renderPipeline->bufferCount()));
    }

    m_framebuffer->setResolution(camera.viewport());
    m_framebuffer->setDrawBuffers({ BufferNames::Color, BufferNames::TransparencyAccumulation, BufferNames::NormalZ, BufferNames::Emissisiveness, BufferNames::TransparencyCount });
    m_framebuffer->clear();
    m_framebuffer->get().clearBuffer(GL_COLOR, BufferNames::TransparencyAccumulation, glm::vec4(0.0f)); // clear accumulation buffer with 0
    m_framebuffer->setDrawBuffers({ BufferNames::Color, BufferNames::NormalZ, BufferNames::Emissisiveness });

    //drawGame(camera, false);
    // instead of the discard magic in the shader, clear the buffer here (eliminate sources)
    //m_framebuffer->clear();


    glDisable(GL_CULL_FACE);
    CheckGLError();
    glDepthMask(GL_FALSE);
    CheckGLError();
    drawGame(camera, false);
    glEnable(GL_CULL_FACE);
    CheckGLError();
    glDepthMask(GL_TRUE);
    CheckGLError();
    // performance is as bad with this block as with the two lines above
    /*m_framebuffer->setDrawBuffers({ BufferNames::TransparencyAccumulation, BufferNames::NormalZ, BufferNames::Emissisiveness, BufferNames::TransparencyCount });
    glDisable(GL_CULL_FACE);
    CheckGLError();
    glDepthMask(GL_FALSE);
    CheckGLError();
    glEnable(GL_BLEND);
    CheckGLError();
    glBlendFunc(GL_ONE, GL_ONE);
    CheckGLError();
    drawGame(camera, true);
    //drawGameAlpha(camera);
    glEnable(GL_CULL_FACE);
    CheckGLError();
    glDepthMask(GL_TRUE);
    CheckGLError();
    glDisable(GL_BLEND);
    CheckGLError();*/


    RenderMetaData metadata(camera, side);
    m_renderPipeline->apply(*m_framebuffer, metadata);

    // set viewport to original resolution
    glViewport(destinationViewport.x(), destinationViewport.y(), destinationViewport.width(), destinationViewport.height());

    // transfer rendered image to target framebuffer
    m_outputBlitter->setInputMapping({ { "source", m_currentOutputBuffer } });
    m_outputBlitter->apply(*m_framebuffer, target);
}

void GamePlayScene::update(float deltaSec) {
    m_starField->update(deltaSec, m_gamePlay.cameraHead().position());
}

bool GamePlayScene::worldTreeRendererEnabled() const {
    return m_worldTreeRendererEnabled;
}

void GamePlayScene::setWorldTreeRendererEnabled(bool enabled) {
    m_worldTreeRendererEnabled = enabled;
}

void GamePlayScene::setOutputBuffer(int i) {
    m_currentOutputBuffer = glm::min(i, m_renderPipeline->bufferCount() - 1);
    glow::info("Switched to output-buffer: %;", bufferNames[m_currentOutputBuffer]);
}

void GamePlayScene::drawGame(const Camera& camera, bool transparentPass) const {
    if (!transparentPass) {
        World::instance()->skybox().draw(camera);
    }

    m_voxelRenderer->program()->setUniform("lightdir", m_defaultLightDir.get());

    m_voxelRenderer->prepareDraw(camera, true, transparentPass);

    for (WorldObject* worldObject : World::instance()->worldObjects()) {
        VoxelRenderer::instance()->draw(*worldObject);
    }

    World::instance()->player().hud().draw();

    m_voxelRenderer->afterDraw();

    World::instance()->particleEngine().draw(camera, transparentPass);

    if (m_worldTreeRendererEnabled) {
        m_worldTreeRenderer->draw(camera);
    }
}

void GamePlayScene::drawGameAlpha(const Camera& camera) const {
    m_voxelRenderer->prepareDraw(camera, false);
    m_voxelRenderer->program()->setUniform("lightdir", m_defaultLightDir.get());
    VoxelFont::instance()->drawString("Voxellancer", glm::vec3(0, 0.5f, -1) * 40.f, glm::quat(), FontSize::SIZE5x7, 0.4f, FontAlign::CENTER);
    m_voxelRenderer->afterDraw();
}
