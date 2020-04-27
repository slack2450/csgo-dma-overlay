#pragma once

#define MAX_ENTITIES 64
#include <string>
#include <vmmdll.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

typedef struct _EntityInfo
{
    bool m_bDormant;
    bool m_bIsLocalPlayer;

    int m_iHealth;

    glm::vec3 m_vecOrigin;
    glm::vec3 m_vecViewOffset;
    glm::vec3 viewAngles;

    glm::vec2 m_vecFeet;
    glm::vec2 m_vecHead;
} EntityInfo;

extern EntityInfo g_entities[MAX_ENTITIES];
extern EntityInfo g_localPlayer;

#include "Interfaces.h"
#include "Utils.h"

namespace Entities
{
    void update();
    std::string serialise();
}