#include "textfieldhudgetvoxels.h"

#include <vector>

#include "ui/voxelfont.h"
#include "ui/letter.h"
#include "textfieldhudget.h"
#include "hud.h"
#include "voxel/voxelrenderer.h"

#include <glm/gtx/intersect.hpp>

#include "utils/geometryhelper.h"

TextFieldHudgetVoxels::TextFieldHudgetVoxels(Hudget* textFieldHudget, glm::vec3 direction, float scale, std::string content, FontSize fontSize) :
m_hudget(textFieldHudget),
m_voxelFont(VoxelFont::instance()),
m_content(content),
m_direction(direction),
m_scale(scale),
m_fontSize(fontSize)
{
    m_width = m_voxelFont->letterWidth(fontSize) * m_scale;
    m_height = m_voxelFont->letterHeight(fontSize) * m_scale;
    m_offset = -1.f * ((m_content.length() - 1) / 2.0f) * m_width;
}

TextFieldHudgetVoxels::~TextFieldHudgetVoxels() = default;

void TextFieldHudgetVoxels::setContent(std::string content) {
    m_content = content;
    m_offset = -1.f * ((m_content.length() - 1) / 2.0f) * m_width;
}

void TextFieldHudgetVoxels::draw() {
    m_voxelFont->drawString(m_content, worldPosition(), worldOrientation(), m_fontSize, m_scale, FontAlign::CENTER);
}

const glm::vec3 TextFieldHudgetVoxels::upperLeft() const {
    return worldPosition() + worldOrientation() * offsetToCenter(true, true);
}

const glm::vec3 TextFieldHudgetVoxels::lowerLeft() const {
    return worldPosition() + worldOrientation() * offsetToCenter(false, true);
}
const glm::vec3 TextFieldHudgetVoxels::upperRight() const {
    return worldPosition() + worldOrientation() * offsetToCenter(true, false);
}
const glm::vec3 TextFieldHudgetVoxels::lowerRight() const {
    return worldPosition() + worldOrientation() * offsetToCenter(false, false);
}

bool TextFieldHudgetVoxels::isAt(const Ray& ray) const {
    return GeometryHelper::intersectRectangle(&ray, upperLeft(), lowerLeft(), lowerRight(), upperRight());
}

float TextFieldHudgetVoxels::width() {
    return m_width;
}

float TextFieldHudgetVoxels::height() {
    return m_height;
}

float TextFieldHudgetVoxels::scale() {
    return m_scale;
}

const glm::vec3 TextFieldHudgetVoxels::offsetToCenter(bool upper, bool left) const {
    float horizontalOffset, verticalOffset;
    if (left) {
        horizontalOffset = 0;
    } else {
        horizontalOffset = (float)m_content.length();
    }
    if (upper) {
        verticalOffset = m_height;
    } else {
        verticalOffset = -m_height;
    }

    return glm::vec3(m_offset + m_width*horizontalOffset - m_width / 2, verticalOffset, 0);
}

glm::vec3 TextFieldHudgetVoxels::worldPosition() const {
    return m_hudget->worldPosition(m_direction);
}

glm::quat TextFieldHudgetVoxels::worldOrientation() const {
    return m_hudget->worldOrientation(m_direction);
}