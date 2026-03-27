#include <string>
#define CPPHTTPLIB_OPENSSL_SUPPORT

#include "RED4ext/Api/ApiVersion.hpp"
#include "RED4ext/Api/v1/PluginInfo.hpp"
#include "RED4ext/Api/v1/Runtime.hpp"
#include "RED4ext/Api/v1/SemVer.hpp"
#include "RED4ext/Api/v1/Version.hpp"
#include "RED4ext/CNamePool.hpp"
#include "RED4ext/Common.hpp"
#include "RED4ext/RTTISystem.hpp"
#include "RED4ext/RTTITypes.hpp"
#include "RED4ext/Scripting/Functions.hpp"
#include "RED4ext/Scripting/IScriptable.hpp"
#include "RED4ext/Scripting/Stack.hpp"
#include <RED4ext/RED4ext.hpp>
#include <cstdint>
#include <httplib.h>

httplib::Client cli("http://localhost:8080");

struct MyCustomClass : RED4ext::IScriptable
{
    RED4ext::CClass* GetNativeType();
};

RED4ext::TTypedClass<MyCustomClass> cls("MyCustomClass");

RED4ext::CClass* MyCustomClass::GetNativeType() 
{
    return &cls;
}

void GetNumber(RED4ext::IScriptable* aContext, RED4ext::CStackFrame* aFrame, float* aOut, int64_t a4)
{
    RED4EXT_UNUSED_PARAMETER(aContext);
    RED4EXT_UNUSED_PARAMETER(a4);

    aFrame->code++;

    if (auto res = cli.Get("/hi")) 
    {
        res->body;
       *aOut = 6.25;
       return;
    } 

    *aOut = 10.0;
}

void GetWarningText(RED4ext::IScriptable* aContext, RED4ext::CStackFrame* aFrame, std::string* aOut, int64_t a4)
{
    RED4EXT_UNUSED_PARAMETER(aContext);
    RED4EXT_UNUSED_PARAMETER(a4);

    aFrame->code++;

    if (auto res = cli.Get("/")) 
    {
    
       *aOut = res->body;
       return;
    } 

    std::string text{"Not Possible"};
    *aOut = text;
}

RED4EXT_C_EXPORT void RED4EXT_CALL RegisterTypes()
{
    RED4ext::CNamePool::Add("MyCustomClass");

    cls.flags = { .isNative = true };
    RED4ext::CRTTISystem::Get()->RegisterType(&cls);
}

RED4EXT_C_EXPORT void RED4EXT_CALL PostRegisterTypes()
{
    auto rtti = RED4ext::CRTTISystem::Get();
    auto scriptable = rtti->GetClass("IScriptable");
    cls.parent = scriptable;

    RED4ext::CBaseFunction::Flags flags =  {.isNative = true};
    auto getNumber = RED4ext::CClassFunction::Create(&cls, "GetNumber", "GetNumber", &GetNumber);
    getNumber->flags = flags;
    getNumber->SetReturnType("Float");
    cls.RegisterFunction(getNumber);
}

RED4EXT_C_EXPORT bool RED4EXT_CALL Main(RED4ext::v1::PluginHandle aHandle, RED4ext::v1::EMainReason aReason, const RED4ext::v1::Sdk* aSdk)
{
    RED4EXT_UNUSED_PARAMETER(aHandle);
    
    switch (aReason)
    {
    case RED4ext::v1::EMainReason::Load:
    {
        auto rtti = RED4ext::CRTTISystem::Get();

        rtti->AddRegisterCallback(RegisterTypes);
        rtti->AddPostRegisterCallback(PostRegisterTypes);
        
        aSdk->logger->Info(aHandle, "Olá Mundo!");    
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