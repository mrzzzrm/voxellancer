#include "gameplay.h"

#include "gamestate/game.h"
#include "gamestate/gameplay/running/gameplayrunning.h"
#include "gamestate/gameplay/paused/gameplaypaused.h"

#include "utils/fsm/trigger.h"
#include "utils/fsm/triggeredtransition.h"

#include "world/world.h"


GamePlay::GamePlay(Game* game):
    GameState("In Game", game),
    m_game(game),
    m_runningState(new GamePlayRunning(this)),
    m_pausedState(new GamePlayPaused(this)),
    m_player(this),
    m_scene(this, &m_player),
    m_soundManager(new SoundManager()),
    m_scenario(this)
{
    setInitialSubstate(m_runningState);

    m_runningState->setPauseTrigger(Trigger<GameState>(new TriggeredTransition<GameState>(m_runningState, m_pausedState)));
    m_pausedState->setContinueTrigger(Trigger<GameState>(new TriggeredTransition<GameState>(m_pausedState, m_runningState)));
}

Game* GamePlay::game() {
    return m_game;
}

GamePlayScene& GamePlay::scene() {
    return m_scene;
}

GamePlayRunning& GamePlay::running() {
    return *m_runningState;
}

GamePlayPaused& GamePlay::paused() {
    return *m_pausedState;
}

const Scene& GamePlay::scene() const {
    return m_scene;
}

const CameraHead& GamePlay::cameraHead() const {
    return m_player.cameraHead();
}

Player& GamePlay::player() {
    return m_player;
}

SoundManager& GamePlay::soundManager() {
    return *m_soundManager;
}

void GamePlay::update(float deltaSec) {
    GameState::update(deltaSec);
    m_scene.update(deltaSec);
}

void GamePlay::onEntered() {
    GameState::onEntered();

    m_scenario.load();
}

void GamePlay::onLeft() {
    GameState::onLeft();
}

