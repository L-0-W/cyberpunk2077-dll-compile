#include "RED4ext/Api/ApiVersion.hpp"
#include "RED4ext/Api/v1/PluginInfo.hpp"
#include "RED4ext/Api/v1/Runtime.hpp"
#include "RED4ext/Api/v1/SemVer.hpp"
#include "RED4ext/Api/v1/Version.hpp"
#include <RED4ext/RED4ext.hpp>

RED4EXT_C_EXPORT bool RED4EXT_CALL Main(RED4ext::v1::PluginHandle aHandle, RED4ext::v1::EMainReason aReason, const RED4ext::v1::Sdk* aSdk)
{
    switch (aReason)
    {
    case RED4ext::v1::EMainReason::Load:
    {
        aSdk->logger->Trace(aHandle, "Olá Mundo!");
        break;
    }
    case RED4ext::v1::EMainReason::Unload:
    {
        // Free memory, detach hooks.
        // The game's memory is already freed, to not try to do anything with it.
        break;
    }
    }

    return true;
}

RED4EXT_C_EXPORT void RED4EXT_CALL Query(RED4ext::v1::PluginInfo* aInfo)
{
    aInfo->name = L"AnimalMod";
    aInfo->author = L"LOW";
    aInfo->version = RED4EXT_V1_SEMVER(0, 0, 1);
    aInfo->runtime = RED4EXT_V1_RUNTIME_VERSION_LATEST;
    aInfo->sdk = RED4EXT_V1_SDK_VERSION_CURRENT;
}

RED4EXT_C_EXPORT uint32_t RED4EXT_CALL Supports()
{
    return RED4EXT_API_VERSION_1;
}