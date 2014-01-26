#pragma once

#include <memory>

#include "geometry/viewport.h"

#include "view.h"


class CameraHead;
class Scene;
class StereoRenderInfo;

class Viewer {
public:
    Viewer(const Viewport& viewport);

    void setScene(Scene* scene);
    void setCameraHead(CameraHead* cameraHead);
    void setViewport(const Viewport& viewport);

    void switchToMonoView();
    void switchToStereoView(const StereoRenderInfo& stereoRenderInfo);

    void draw();


protected:
    Scene* m_scene;
    std::unique_ptr<View> m_view;
    CameraHead* m_cameraHead;
    Viewport m_viewport;
};
