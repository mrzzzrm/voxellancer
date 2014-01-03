#include <bandit/bandit.h>

#include <iostream>

#include "geometry/aabb.h"
#include "../bandit_extension/vec3helper.h"
#include "worldobject/worldobject.h"
#include "voxel/voxel.h"


using namespace bandit;


go_bandit([](){
    describe("AABB", [](){
        PropertyManager::instance()->reset();
        PropertyManager::instance()->load("data/config.ini");


        before_each([&](){
        });

        it("works in worldobject", [&]() {
            WorldObject a;

            a.addVoxel(new Voxel(glm::ivec3(0, 0, 0), 0xFFFFFF));
            a.addVoxel(new Voxel(glm::ivec3(1, 0, 0), 0xFFFFFF));
            a.addVoxel(new Voxel(glm::ivec3(2, 0, 0), 0xFFFFFF));
            a.addVoxel(new Voxel(glm::ivec3(3, 0, 0), 0xFFFFFF));
            a.addVoxel(new Voxel(glm::ivec3(4, 0, 0), 0xFFFFFF));
            AssertThat(a.aabb().contains(glm::vec3(4.49, 0, 0)), Equals(true));
            AssertThat(a.transform().position(), EqualsWithDelta(glm::vec3(2, 0, 0), glm::vec3(0.01, 0.01, 0.01)));

            a.setPosition(glm::vec3(7, 0, 0));
            AssertThat(a.aabb().contains(glm::vec3(9.49, 0, 0)), Equals(true));
        });

//        it("can adjust its center", [&]() {
//            WorldObject *d = new WorldObject(1.0, glm::vec3(1,1,1));
//
//            AssertThat(d->collisionDetector().voxeltree().boundingSphere().position(), EqualsWithDelta(glm::vec3(-1, -1, -1), glm::vec3(0.01, 0.01, 0.01)));
//        });
//
//        it("can adjust its center and scale", [&]() {
//            WorldObject *d = new WorldObject(3.0f, glm::vec3(1, 1, 1));
//
//            AssertThat(d->collisionDetector().voxeltree().boundingSphere().radius(), EqualsWithDelta(2.5f, 0.1f));
//            AssertThat(d->collisionDetector().voxeltree().boundingSphere().position(), EqualsWithDelta(glm::vec3(-3, -3, -3), glm::vec3(0.01, 0.01, 0.01)));
//        });
//
//        it("works in rotated  worldobject", [&]() {
//            WorldObject a;
//            a.addVoxel(new Voxel(glm::ivec3(0, 0, 0), 0xFFFFFF));
//            a.addVoxel(new Voxel(glm::ivec3(1, 0, 0), 0xFFFFFF));
//            a.addVoxel(new Voxel(glm::ivec3(2, 0, 0), 0xFFFFFF));
//            a.addVoxel(new Voxel(glm::ivec3(3, 0, 0), 0xFFFFFF));
//            a.addVoxel(new Voxel(glm::ivec3(4, 0, 0), 0xFFFFFF));
//
//            a.rotate(glm::angleAxis(90.f, glm::vec3(0, 1, 0)));
//
//            AssertThat(a.aabb().contains(glm::vec3(0, 0, -2.49)), Equals(true));
//        });
//
//        it("incremental mass and center calculation works", [&]() {
//            WorldObject a(0.8f);
//            WorldObject b(0.8f);
//
//            a.addVoxel(new Voxel(glm::ivec3(1, 0, 0), 0xFFFFFF));
//            a.addVoxel(new Voxel(glm::ivec3(2, 0, 0), 0xFFFFFF));
//            a.addVoxel(new Voxel(glm::ivec3(3, 0, 0), 0xFFFFFF));
//            a.addVoxel(new Voxel(glm::ivec3(3, 1, 0), 0xFFFFFF));
//            a.addVoxel(new Voxel(glm::ivec3(3, 2, 0), 0xFFFFFF));
//            a.removeVoxel(a.voxel(glm::ivec3(3, 2, 0)));
//
//            b.addVoxel(new Voxel(glm::ivec3(1, 0, 0), 0xFFFFFF));
//            b.addVoxel(new Voxel(glm::ivec3(2, 0, 0), 0xFFFFFF));
//            b.addVoxel(new Voxel(glm::ivec3(3, 0, 0), 0xFFFFFF));
//            b.addVoxel(new Voxel(glm::ivec3(3, 1, 0), 0xFFFFFF));
//
//            AssertThat(a.physics().mass(), Equals(b.physics().mass()));
//            AssertThat(a.transform().center(), EqualsWithDelta(b.transform().center(), glm::vec3(0.01)));
//        });
    });
});

