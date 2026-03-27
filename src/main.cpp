#include <RED4ext/RED4ext.hpp>
#include <RED4ext/Scripting/Natives/ScriptGameInstance.hpp>
#include <RED4ext/Scripting/IScriptable.hpp>
#include <RED4ext/RTTITypes.hpp>

using namespace RED4ext;

struct Animal : IScriptable
{
    CClass* GetNativeType() override;
    void Talk();
};

TTypedClass<Animal> cls("Animal");

//a

CClass* Animal::GetNativeType()
{
    return &cls;
}

void Animal::Talk()
{
    RED4ext::CString message("animal is talking");
    // LogChannel is a global function: LogChannel(CName channel, CString message)
    ExecuteGlobalFunction("LogChannel", nullptr, CName("Info"), message);
}

// RTTI Wrapper for Animal::Talk
void TalkWrapper(IScriptable* aThis, CStackFrame* aFrame, void* aOut, int64_t a4)
{
    RED4EXT_UNUSED_PARAMETER(aFrame);
    RED4EXT_UNUSED_PARAMETER(aOut);
    RED4EXT_UNUSED_PARAMETER(a4);

    auto animal = static_cast<Animal*>(aThis);
    animal->Talk();
}

void RegisterTypes()
{
    cls.flags = {.isNative = true};
    cls.parent = CRTTISystem::Get()->GetClass("IScriptable");
    CRTTISystem::Get()->RegisterType(&cls);
}

void PostRegisterTypes()
{
    auto rtti = CRTTISystem::Get();
    auto animalCls = rtti->GetClass("Animal");
    
    auto talkFunc = CClassFunction::Create(animalCls, "Talk", "Talk", &TalkWrapper, {.isNative = true});
    animalCls->RegisterFunction(talkFunc);
}

RED4EXT_C_EXPORT bool RED4EXT_CALL Main(v1::PluginHandle aHandle, v1::EMainReason aReason, const v1::Sdk* aSdk)
{
    RED4EXT_UNUSED_PARAMETER(aHandle);
    RED4EXT_UNUSED_PARAMETER(aSdk);

    switch (aReason)
    {
    case v1::EMainReason::Load:
    {
        aSdk->logger->Trace(aHandle, "Olá Mundo");
        CRTTISystem::Get()->AddRegisterCallback(RegisterTypes);
        CRTTISystem::Get()->AddPostRegisterCallback(PostRegisterTypes);
        break;
    }
    case v1::EMainReason::Unload:
    {
        break;
    }
    }

    return true;
}

RED4EXT_C_EXPORT void RED4EXT_CALL Query(v1::PluginInfo* aInfo)
{
    aInfo->name = L"AnimalMod";
    aInfo->author = L"Antigravity";
    aInfo->version = RED4EXT_V1_SEMVER(1, 0, 0);
    aInfo->runtime = RED4EXT_V1_RUNTIME_VERSION_LATEST;
    aInfo->sdk = RED4EXT_V1_SDK_VERSION_CURRENT;
}

RED4EXT_C_EXPORT uint32_t RED4EXT_CALL Supports()
{
    return RED4EXT_API_VERSION_1;
}
