#include <RED4ext/RED4ext.hpp>
#include <RED4ext/Scripting/Natives/Generated/IScriptable.hpp>
#include <RED4ext/Scripting/Natives/ScriptGameInstance.hpp>
#include <RED4ext/Scripting/IScriptable.hpp>
#include <RED4ext/RTTITypes.hpp>

struct Animal : RED4ext::IScriptable
{
    RED4ext::CClass* GetNativeType();
    void Talk();
};

RED4ext::TTypedClass<Animal> cls("Animal");

RED4ext::CClass* Animal::GetNativeType()
{
    return &cls;
}

void Animal::Talk()
{
    RED4ext::CString message("animal is talking");
    RED4ext::ExecuteFunction("LogChannel", nullptr, RED4ext::CName("Debug"), message);
}

void RegisterTypes()
{
    cls.flags = {.isNative = true};
    cls.parent = RED4ext::CRTTISystem::Get()->GetClass("IScriptable");
    RED4ext::CRTTISystem::Get()->RegisterType(&cls);
}

void PostRegisterTypes()
{
    auto rtti = RED4ext::CRTTISystem::Get();
    auto animalCls = rtti->GetClass("Animal");
    
    auto talkFunc = RED4ext::CClassFunction::Create(&Animal::Talk, "Talk", "Talk", {.isNative = true});
    animalCls->RegisterFunction(talkFunc);
}

RED4EXT_C_EXPORT bool RED4EXT_CALL Main(RED4ext::PluginHandle aHandle, RED4ext::EMainReason aReason, const RED4ext::Sdk* aSdk)
{
    switch (aReason)
    {
    case RED4ext::EMainReason::Load:
    {
        RED4ext::CRTTISystem::Get()->AddRegisterCallback(RegisterTypes);
        RED4ext::CRTTISystem::Get()->AddPostRegisterCallback(PostRegisterTypes);
        break;
    }
    case RED4ext::EMainReason::Unload:
    {
        break;
    }
    }

    return true;
}

RED4EXT_C_EXPORT void RED4EXT_CALL Query(RED4ext::PluginInfo* aInfo)
{
    aInfo->name = L"AnimalMod";
    aInfo->author = L"Antigravity";
    aInfo->version = RED4EXT_SEMVER(1, 0, 0);
    aInfo->runtime = RED4EXT_RUNTIME_LATEST;
    aInfo->sdk = RED4EXT_SDK_LATEST;
}

RED4EXT_C_EXPORT uint32_t RED4EXT_CALL Supports()
{
    return RED4EXT_API_VERSION_LATEST;
}
