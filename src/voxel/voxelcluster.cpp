#include "voxelcluster.h"

#include <iostream>
#include <list>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <glow/DebugMessageOutput.h>
#include <glow/ChangeListener.h>
#include <glow/logging.h>
#include <glowutils/MathMacros.h>

#include "utils/tostring.h"


VoxelCluster::VoxelCluster(glm::vec3 center, float scale): 
    m_voxel(),
    m_voxelRenderData(this),
    m_transform(center, scale)
{
}

VoxelCluster::VoxelCluster(const VoxelCluster& other):
	m_voxel(other.m_voxel),
    m_aabb(other.m_aabb),
	m_voxelRenderData(this),
	m_transform(other.m_transform)
{
	
}

VoxelCluster::~VoxelCluster() {

}

WorldTransform &VoxelCluster::transform() {
    return m_transform;
}

const WorldTransform &VoxelCluster::transform() const {
    return m_transform;
}

void VoxelCluster::addVoxel(const Voxel & voxel) {
    m_voxel[voxel.gridCell()] = voxel;
    m_aabb.extend(voxel.gridCell());
    m_voxelRenderData.invalidate();
}

void VoxelCluster::removeVoxel(const cvec3 & position) {
    m_voxel.erase(position);
    m_voxelRenderData.invalidate();
}

AABB VoxelCluster::aabb() {
    return AABB::containing(sphere());
}

Sphere VoxelCluster::sphere() {
    Sphere sphere;
    sphere.setPosition(m_transform.applyTo(glm::vec3(0)));
    // m_aabb only contains the center of each voxel so add sqrt(2)
    sphere.setRadius((glm::length(glm::vec3(m_aabb.rub() - m_aabb.llf())) + glm::root_two<float>()) / 2.f * m_transform.scale());
    return sphere;
}


const std::unordered_map<cvec3, Voxel, VoxelHash> & VoxelCluster::voxel() const{
    return m_voxel;
}

VoxelRenderData * VoxelCluster::voxelRenderData() {
    return &m_voxelRenderData;
}


void VoxelCluster::update(float delta_sec) {

}


