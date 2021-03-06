#pragma once

#include <memory>

#include "geometry/viewport.h"

class CameraHead;
class Scene;
class StereoRenderInfo;
class View;

class Viewer {
public:
    Viewer(const Viewport& viewport);
    ~Viewer();

    const View& view() const;

    void setViewport(const Viewport& viewport);

    void switchToMonoView();
    void switchToStereoView(const StereoRenderInfo& stereoRenderInfo);

    void update(float deltaSec);

    void draw(const Scene& scene, const CameraHead& cameraHead);


protected:
    std::unique_ptr<View> m_view;
    Viewport m_viewport;
};

