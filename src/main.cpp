#include <RED4ext/RED4ext.hpp>

RED4EXT_C_EXPORT bool RED4EXT_CALL Main(RED4ext::v1::PluginHandle aHandle, RED4ext::v1::EMainReason aReason, const RED4ext::v1::Sdk* aSdk)
{
    RED4EXT_UNUSED_PARAMETER(aHandle);
    

    switch (aReason)
    {
    case RED4ext::v1::EMainReason::Load:
    {
        aSdk->logger->Trace(aHandle, "Olá Mundo");
        RED4ext::CRTTISystem::Get()->AddRegisterCallback(RegisterTypes);
        RED4ext::CRTTISystem::Get()->AddPostRegisterCallback(PostRegisterTypes);
        break;
    }
    case RED4ext::v1::EMainReason::Unload:
    {
        break;
    }
    }

    return true;
}

