#include <iostream>
#include <thread>

#include "webSocketServer.h"

#include <leechcore.h>

#include <boost/timer.hpp>

#include <vmmdll.h>

#include "Entities.h"
#include "Utils.h"
#include "Interfaces.h"


typedef struct _CGlobalVarsBase
{
    float realtime;
    int framecount;
    float absoluteframetime;
    float unknown;
    float curtime;
    float frametime;
    int maxclients;
    int tickcount;
    float interval_per_tick;
    float interpolation_amount;
} CGlobalVarsBase;

// Function Pre-Definitions
int Shutdown();

int main()
{

    // Open vmm.dll with FPGA device
    LPSTR args[] = { (LPSTR)"", (LPSTR)"-device", (LPSTR)"FPGA" };
    if (!VMMDLL_Initialize(3, args))
    {
        std::cout << "[-] Failed to initialize vmm.dll" << std::endl;
        return -1;
    }
    std::cout << "[+] vmm.dll initialized!" << std::endl;

    
    if (!Interfaces::Init())
        return Shutdown();
        
    CGlobalVarsBase globalVars;
    VMMDLL_MemRead(g_pid, g_dwGlobalVars, (PBYTE)&globalVars, sizeof(CGlobalVarsBase));

    int TickRate = 1/ globalVars.interval_per_tick;

    printf("[*] Interval Per Tick %f\n", globalVars.interval_per_tick);
    printf("[*] Tick Rate: %d\n", TickRate);
    printf("\n");

    Entities::update();

    std::thread webSocket(webSocketServer::Start);
    webSocket.join();

    return Shutdown();
}

int Shutdown()
{
    // Close vmm.dll
    if (!VMMDLL_Close())
    {
        std::cout << "[-] Failed to close vmm.dll" << std::endl;
        return 0;
    }
    std::cout << "[+] vmm.dll closed!" << std::endl;

    std::cout << "[*] Exiting..." << std::endl;
    return 0;
}

