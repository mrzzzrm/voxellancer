#include "game.h"

#include <GL/glew.h>

#include <algorithm>
#include <random>
#include <memory>
#include <iostream>
#include <fstream>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <glow/AutoTimer.h>
#include <glow/logging.h>

#include <fmod.hpp>
#include <fmod_dsp.h>
#include <fmod_errors.h>

#include "property/propertymanager.h"
#include "resource/clustercache.h"
#include "utils/hd3000dummy.h"
#include "utils/linuxvmdummy.h"
#include "ui/inputhandler.h"
#include "ui/hud.h"

#include "world/world.h"
#include "world/god.h"
#include "skybox.h"
#include "voxel/voxelrenderer.h"
#include "world/worldobject.h"
#include "collision/collisiondetector.h"

Game::Game(GLFWwindow *window):
	m_window(window),
	m_camera(),
	m_inputHandler(window, &m_camera)
{
	reloadConfig();
}

Game::~Game(){

}

void Game::reloadConfig() {
	PropertyManager::instance()->load("data/config.ini");
}

void Game::initialize()
{
    glow::AutoTimer t("Initialize Game");

	glow::debug("Game::testFMOD()");
    //testFMOD();

	//Must be created first
	m_linuxvmdummy = std::unique_ptr<LinuxVMDummy>(new LinuxVMDummy);

    glow::debug("create world");
    m_world = World::instance();

    glow::debug("Create Skybox");
	m_skybox = std::unique_ptr<Skybox>(new Skybox);

	glow::debug("Create Voxel");
    m_voxelRenderer = std::unique_ptr<VoxelRenderer>(new VoxelRenderer);


    WorldObject *m_testClusterA = new WorldObject();
    ClusterCache::instance()->fill(m_testClusterA, "data/voxelcluster/basicship.csv");
	//m_testClusterA->transform().setCenter(glm::vec3(3, 0, 3)); // TODO: shouldnt center == centerofmass ?
    m_testClusterA->transform().setPosition(glm::vec3(0, 0, -10));
	//m_testClusterA->removeVoxel(glm::ivec3(3, 2, 3)); // this verifies the objects are different
    m_world->god().scheduleSpawn(m_testClusterA);


    WorldObject *m_testClusterB = new WorldObject();
    ClusterCache::instance()->fill(m_testClusterB, "data/voxelcluster/normandy.csv");
	//m_testClusterB->transform().setCenter(glm::vec3(3, 0, 3));
    m_testClusterB->transform().setPosition(glm::vec3(40, 50, -80));
    m_world->god().scheduleSpawn(m_testClusterB);

    WorldObject *m_testCluster = new WorldObject();
    m_testCluster->transform().move(glm::vec3(0, 0, 0));
    m_testCluster->addVoxel(new Voxel(glm::ivec3(1, 0, 0), cvec3(0, 255, 0)));
    m_testCluster->addVoxel(new Voxel(glm::ivec3(2, 0, 0), cvec3(255, 255, 0)));
    m_testCluster->addVoxel(new Voxel(glm::ivec3(1, 1, 0), cvec3(0, 0, 255)));
    m_testCluster->addVoxel(new Voxel(glm::ivec3(1, 0, 1), cvec3(255, 0, 0)));
    m_testCluster->addVoxel(new Voxel(glm::ivec3(0, 0, 0), cvec3(255, 0, 128)));
    m_testCluster->finishInitialization();
    m_world->god().scheduleSpawn(m_testCluster);

    WorldObject *m_testClusterMoveable = new WorldObject();
    m_testClusterMoveable->transform().move(glm::vec3(-20, 0, 0));
    m_testClusterMoveable->transform().rotate(glm::angleAxis(-90.f, glm::vec3(0, 1, 0)));
    m_testClusterMoveable->addVoxel(new Voxel(glm::ivec3(0, 0, 7), cvec3(0, 255, 0)));
    m_testClusterMoveable->addVoxel(new Voxel(glm::ivec3(0, 0, 6), cvec3(255, 255, 0)));
    m_testClusterMoveable->addVoxel(new Voxel(glm::ivec3(0, 0, 5), cvec3(255, 255, 0)));
    m_testClusterMoveable->addVoxel(new Voxel(glm::ivec3(0, 0, 4), cvec3(255, 255, 0)));
    m_testClusterMoveable->addVoxel(new Voxel(glm::ivec3(0, 0, 3), cvec3(255, 255, 0)));
    m_testClusterMoveable->addVoxel(new Voxel(glm::ivec3(0, 0, 2), cvec3(255, 255, 0)));
    m_testClusterMoveable->addVoxel(new Voxel(glm::ivec3(0, 0, 1), cvec3(255, 255, 0)));
    m_testClusterMoveable->addVoxel(new Voxel(glm::ivec3(1, 1, 7), cvec3(0, 0, 255)));
    m_testClusterMoveable->addVoxel(new Voxel(glm::ivec3(1, 0, 7), cvec3(255, 0, 0)));
    m_testClusterMoveable->addVoxel(new Voxel(glm::ivec3(0, 0, 8), cvec3(255, 0, 128)));
    m_testClusterMoveable->finishInitialization();
    m_world->god().scheduleSpawn(m_testClusterMoveable);

    WorldObject *wall = new WorldObject(1);
    wall->transform().move(glm::vec3(-20, 0, -50));
    wall->transform().rotate(glm::angleAxis(-90.f, glm::vec3(0, 1, 0)));
    for(int x = 0; x < 50; x++) {
        for(int y = 0; y < 30; y++) {
            for(int z = 0; z < 3; z++) {
                wall->addVoxel(new Voxel(glm::ivec3(z, x, y), cvec3(180, 120, 120)));
            }
        }
    }
    wall->finishInitialization();
    m_world->god().scheduleSpawn(wall);

    m_inputHandler.setWorldObject(m_testClusterMoveable);




    glow::debug("Initial spawn");
    m_world->god().spawn();

	glow::debug("Setup Camersa");
	//viewport set in resize
	m_camera.setPosition(glm::vec3(0, 5, 30));
	m_camera.setZNear(1);
	m_camera.setZFar(9999);

	glow::debug("Create HUD");
	m_hud = std::unique_ptr<HUD>(new HUD());
	m_hud->setCamera(&m_camera);

    m_hd3000dummy = std::unique_ptr<HD3000Dummy>(new HD3000Dummy);

    glClearColor(0.2f, 0.3f, 0.4f, 1.f);
	glow::debug("Game::initialize Done");
}


void Game::update(float delta_sec)
{
    // avoid big jumps after debugging ;)
    delta_sec = glm::min(1.f, delta_sec);

    m_inputHandler.update(delta_sec);
    World::instance()->update(delta_sec);
	m_hud->update(delta_sec);
}

void Game::draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glFrontFace(GL_CCW);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	m_skybox->draw(&m_camera);

    m_voxelRenderer->prepareDraw(&m_camera);
    for (WorldObject * worldObject : m_world->worldObjects()) {
        m_voxelRenderer->draw(worldObject);
    }

    // draw all other voxelclusters...
    m_voxelRenderer->afterDraw();

	m_hud->draw();

    m_hd3000dummy->drawIfActive();
}

void ERRCHECK(FMOD_RESULT result)
{
    if (result != FMOD_OK)
    {
        printf("FMOD error! (%d) %s\n", result, FMOD_ErrorString(result));
        exit(-1);
    }
}

void Game::testFMOD()
{
    FMOD::System * system = 0;
    FMOD::Sound  * sound = 0;
    FMOD::Channel *channel = 0;

    FMOD_RESULT result = FMOD::System_Create(&system);
    ERRCHECK(result);

    result = system->init(32, FMOD_INIT_NORMAL, 0);
    ERRCHECK(result);

    result = system->createSound("data/LASER.mp3", FMOD_SOFTWARE, 0, &sound);
    ERRCHECK(result);

    result = system->playSound(FMOD_CHANNEL_FREE, sound, false, &channel);
    ERRCHECK(result);
}

InputHandler * Game::inputHandler()
{
    return &m_inputHandler;
}



