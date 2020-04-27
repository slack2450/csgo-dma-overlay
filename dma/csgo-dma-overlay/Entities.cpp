#include <json.hpp>

#include "Entities.h"
#include "Offsets.h"

#include <boost/timer.hpp>

EntityInfo g_entities[MAX_ENTITIES];
EntityInfo g_localPlayer;

namespace Entities
{
	void update()
	{

        for (int i = 0; i < MAX_ENTITIES; i++)
        {
            EntityInfo* entity = &g_entities[i];

            DWORD entityBase;
            if (VMMDLL_MemRead(g_pid, (ULONG64)(g_dwEntityList)+((ULONG64)(i) * 0x10), (PBYTE)&entityBase, sizeof(entityBase)))
            {
                if (entityBase == 0)
                {
                    entity->m_bDormant = false;
                    continue;
                }

                VMMDLL_MemReadEx(g_pid, (ULONG64)(entityBase)+offsets::m_bDormant, (PBYTE)&entity->m_bDormant, sizeof(entity->m_bDormant), 0, VMMDLL_FLAG_NOCACHE);

                if (entity->m_bDormant)
                    continue;

                if (entityBase == g_dwLocalPlayer)
                {
                    entity->m_bIsLocalPlayer = true;
                    VMMDLL_MemReadEx(g_pid, g_dwClientState + offsets::dwClientState_ViewAngles, (PBYTE)&entity->viewAngles, sizeof(entity->viewAngles), 0, VMMDLL_FLAG_NOCACHE);
                }

                VMMDLL_MemReadEx(g_pid, (ULONG64)(entityBase)+offsets::m_iHealth, (PBYTE)&entity->m_iHealth, sizeof(entity->m_iHealth), 0, VMMDLL_FLAG_NOCACHE);

                VMMDLL_MemReadEx(g_pid, (ULONG64)(entityBase)+offsets::m_vecOrigin, (PBYTE)&entity->m_vecOrigin, sizeof(entity->m_vecOrigin), 0, VMMDLL_FLAG_NOCACHE);
                VMMDLL_MemReadEx(g_pid, (ULONG64)(entityBase)+offsets::m_vecViewOffset, (PBYTE)&entity->m_vecViewOffset, sizeof(entity->m_vecViewOffset), 0, VMMDLL_FLAG_NOCACHE);

                if (entity->m_bIsLocalPlayer)
                    g_localPlayer = *entity;

                //printf("entity HP: %d\n ", entity->m_iHealth);
            }
        }

        //printf("\n");

        glm::mat4 matrix = math::createProjectionViewMatrix(g_localPlayer.m_vecOrigin + g_localPlayer.m_vecViewOffset, g_localPlayer.viewAngles);
        for (int i = 0; i < MAX_ENTITIES; i++)
        {
            EntityInfo* entity = &g_entities[i];

            if (!entity->m_bDormant && !entity->m_bIsLocalPlayer)
            {
                entity->m_vecHead = math::transformWorldPointIntoScreenSpace(entity->m_vecOrigin + glm::vec3(0,0,72), matrix, 1920, 1080);
                entity->m_vecFeet = math::transformWorldPointIntoScreenSpace(entity->m_vecOrigin, matrix, 1920, 1080);
            }
        }
	}

    std::string serialise()
    {
        nlohmann::json json;
        for (int i = 0; i < MAX_ENTITIES; i++)
        {
            EntityInfo* entity = &g_entities[i];

            if (entity->m_bDormant || entity->m_iHealth <= 0 || entity->m_bIsLocalPlayer)
                continue;

            if (glm::any(glm::isnan(entity->m_vecFeet)) || glm::any(glm::isnan(entity->m_vecHead)))
                continue;

            nlohmann::json temp;
            temp["m_iHealth"] = entity->m_iHealth;

            temp["m_vecfeet"] = nlohmann::json::object();
            temp["m_vecFeet"]["x"] = entity->m_vecFeet.x;
            temp["m_vecFeet"]["y"] = entity->m_vecFeet.y;

            temp["m_vecHead"] = nlohmann::json::object();
            temp["m_vecHead"]["x"] = entity->m_vecHead.x;
            temp["m_vecHead"]["y"] = entity->m_vecHead.y;
            json.push_back(temp);
        }
        return json.dump();
    }
}