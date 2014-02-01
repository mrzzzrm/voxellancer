#include "starfield.h"

#include <glow/Array.h>
#include <glow/Shader.h>
#include <glow/VertexArrayObject.h>
#include <glow/VertexAttributeBinding.h>
#include <glow/Program.h>
#include <glow/Buffer.h>
#include <glowutils/File.h>

#include "player.h"
#include "utils/randfloat.h"
#include "glowutils/global.h"
#include "voxel/voxelcluster.h"
#include "worldobject/ship.h"
#include "camera/camera.h"
#include "display/rendering/framebuffer.h"
#include "display/rendering/buffernames.h"
#include "utils/randvec.h"



static int STAR_COUNT = 1000;
static float FIELD_SIZE = 300.0f;
static float STAR_FADE_IN_SEC = 2.0f;

struct Star {
    glm::vec3 pos;
    float brightness;
    float size;
};

Starfield::Starfield(Player* player) :
    RenderPass("starfield"),
    m_player(player)
{
    createAndSetupShaders();
    createAndSetupGeometry();
}

void Starfield::update(float deltaSec) {

    Star* starbuffer = (Star*) m_starBuffer->map(GL_READ_WRITE);
    glm::vec3 position = m_player->playerShip()->transform().position();

    // only perform once per second if this is a performance problem
    for (int i = 0; i < STAR_COUNT; i++) {
        starbuffer[i].brightness = glm::min(1.0f, starbuffer[i].brightness + deltaSec / STAR_FADE_IN_SEC);
        while (starbuffer[i].pos.x - position.x < -FIELD_SIZE) {
            starbuffer[i].pos.x += 2 * FIELD_SIZE;
            starbuffer[i].brightness = 0;
        }
        while (starbuffer[i].pos.x - position.x > FIELD_SIZE) {
            starbuffer[i].pos.x -= 2 * FIELD_SIZE;
            starbuffer[i].brightness = 0;
        }
        while (starbuffer[i].pos.y - position.y < -FIELD_SIZE) {
            starbuffer[i].pos.y += 2 * FIELD_SIZE;
            starbuffer[i].brightness = 0;
        }
        while (starbuffer[i].pos.y - position.y > FIELD_SIZE) {
            starbuffer[i].pos.y -= 2 * FIELD_SIZE;
            starbuffer[i].brightness = 0;
        }
        while (starbuffer[i].pos.z - position.z < -FIELD_SIZE) {
            starbuffer[i].pos.z += 2 * FIELD_SIZE;
            starbuffer[i].brightness = 0;
        }
        while (starbuffer[i].pos.z - position.z > FIELD_SIZE) {
            starbuffer[i].pos.z -= 2 * FIELD_SIZE;
            starbuffer[i].brightness = 0;
        }
    }

    m_starBuffer->unmap();
}


void Starfield::apply(FrameBuffer& frameBuffer, Camera& camera, EyeSide eyeside) {
    int side = eyeside == EyeSide::Left ? 0 : 1;
    m_matricesQueue[side].push(camera.viewProjection());
    if (m_matricesQueue[side].size() > 2) { // store for the last 2 frames... need to find a framerate independent way
        m_matricesQueue[side].pop();
    }

    glDisable(GL_CULL_FACE);
    glEnable(GL_BLEND); 
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDepthMask(GL_FALSE);

    frameBuffer.setDrawBuffers({ BufferNames::Color, BufferNames::Emissisiveness });

    glm::mat4 m1 = camera.viewProjection();
    glm::mat4 m2 = m_matricesQueue[side].front();

    m_shaderProgram->setUniform("viewProjection", m1);
    m_shaderProgram->setUniform("oldViewProjection", m2);
    m_shaderProgram->use();
    m_vertexArrayObject->drawArrays(GL_POINTS, (GLint)0, (GLsizei)STAR_COUNT);
    
    glDepthMask(GL_TRUE);
    glDisable(GL_BLEND);
    glEnable(GL_CULL_FACE);
}

void Starfield::createAndSetupShaders() {
    glow::Shader * vertexShader = glowutils::createShaderFromFile(GL_VERTEX_SHADER, "data/starfield/starfield.vert");
    glow::Shader * geometryShader = glowutils::createShaderFromFile(GL_GEOMETRY_SHADER, "data/starfield/starfield.geo");
    glow::Shader * fragmentShader = glowutils::createShaderFromFile(GL_FRAGMENT_SHADER, "data/starfield/starfield.frag");

    m_shaderProgram = new glow::Program();
    m_shaderProgram->attach(vertexShader, geometryShader, fragmentShader);
    m_shaderProgram->bindFragDataLocation(0, "fragColor");

}


void Starfield::createAndSetupGeometry() {
    m_vertexArrayObject = new glow::VertexArrayObject();

    m_starBuffer = new glow::Buffer(GL_ARRAY_BUFFER);

    glow::Array<Star> stars;

    for (int i = 0; i < STAR_COUNT; i++) {
        stars.push_back(Star{ RandVec3::rand(-FIELD_SIZE, FIELD_SIZE), 0.0f, RandFloat::rand(0.5f, 1.5f) });
    }
    m_starBuffer->setData(stars);

    glow::VertexAttributeBinding* binding = m_vertexArrayObject->binding(0);
    GLint location = m_shaderProgram->getAttributeLocation("a_vertex");
    binding->setAttribute(location);
    binding->setBuffer(m_starBuffer, 0, sizeof(Star));
    binding->setFormat(3, GL_FLOAT, GL_FALSE, offsetof(Star, pos));
    m_vertexArrayObject->enable(location);

    binding = m_vertexArrayObject->binding(1);
    location = m_shaderProgram->getAttributeLocation("a_brightness");
    binding->setAttribute(location);
    binding->setBuffer(m_starBuffer, 0, sizeof(Star));
    binding->setFormat(1, GL_FLOAT, GL_FALSE, offsetof(Star, brightness));
    m_vertexArrayObject->enable(location);

    binding = m_vertexArrayObject->binding(2);
    location = m_shaderProgram->getAttributeLocation("a_size");
    binding->setAttribute(location);
    binding->setBuffer(m_starBuffer, 0, sizeof(Star));
    binding->setFormat(1, GL_FLOAT, GL_FALSE, offsetof(Star, size));
    m_vertexArrayObject->enable(location);

}

