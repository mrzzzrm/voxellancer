#include "renderpipeline.h"

#include <glow/logging.h>

#include "framebuffer.h"
#include "renderpass.h"
#include "defaultrenderpipeline.h"


RenderPipeline::RenderPipeline(const std::string& name):
    RenderPass(name),
    m_initialized(false)
{
}

void RenderPipeline::apply(FrameBuffer& frameBuffer, const RenderMetaData& metadata) {
    if (!m_initialized) {
        glow::debug("Renderpipeline: intializing");
        m_initialized = true;
        setup();
    }

    for (std::shared_ptr<RenderPass>& pass : m_passes) {
        pass->apply(frameBuffer, metadata);
    }
}

RenderPipeline* RenderPipeline::getDefault() {
    return new DefaultRenderPipeline();
}

void RenderPipeline::add(std::shared_ptr<RenderPass> pass, int index) {
    if (index < 0) {
        m_passes.push_back(pass);
    } else {
        m_passes.insert(m_passes.begin() + index, pass);
    }
}

void RenderPipeline::insertAfter(std::shared_ptr<RenderPass> pass, const std::string& after) {
    auto iter = m_passes.begin();
    while (iter != m_passes.end()) {
        if ((*iter)->name() == after) {
            iter++;
            m_passes.insert(iter, pass);
            return;
        }
        iter++;
    }
    assert(false); // pass not found
}

