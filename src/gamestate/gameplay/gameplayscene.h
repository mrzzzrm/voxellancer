#pragma once

#include <memory>

#include <glm/glm.hpp>

#include "display/scene.h"

#include "property/property.h"



class GamePlay;
class VoxelRenderer;
class HD3000Dummy;
class SoundManager;
class CameraHead;
class FrameBuffer;
class Blitter;
class RenderPipeline;
class Player;
class Starfield;

class GamePlayScene: public Scene {
public:
    GamePlayScene(GamePlay* gamePlay, Player* player);
    ~GamePlayScene();

    void setPlayer(Player* player);

    virtual void draw(const Camera& camera, glow::FrameBufferObject* target, EyeSide side = EyeSide::Left) const override;
    virtual void update(float deltaSec) override;

    void setOutputBuffer(int i);


protected:
    std::unique_ptr<Blitter> m_outputBlitter;
    std::unique_ptr<RenderPipeline> m_renderPipeline;
    std::unique_ptr<FrameBuffer> m_framebuffer;
    std::unique_ptr<HD3000Dummy> m_hd3000dummy;
    std::shared_ptr<VoxelRenderer> m_voxelRenderer;
    std::shared_ptr<Starfield> m_starField;

    GamePlay* m_gamePlay;
    CameraHead* m_head;
    Player* m_player;

    Property<glm::vec3> m_defaultLightDir;
    int m_currentOutputBuffer;


    void drawGame(const Camera& camera) const;
};

