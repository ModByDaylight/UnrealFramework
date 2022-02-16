#include <Unreal/Property/FInterfaceProperty.hpp>

namespace RC::Unreal
{
    UClass* FInterfaceProperty::GetInterfaceClass()
    {
        return Helper::Casting::offset_deref<UClass*>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::InterfaceProperty_InterfaceClass));
    }

    void FInterfaceProperty::SetInterfaceClass(UClass* NewInterfaceClass)
    {
        UClass** InterfaceClass = Helper::Casting::ptr_cast<UClass**>(this, StaticOffsetFinder::retrieve_static_offset(MemberOffsets::InterfaceProperty_InterfaceClass));
        if (!InterfaceClass) { throw std::runtime_error{"FInterfaceProperty::SetInterfaceClass: The location of 'InterfaceClass' was nullptr"}; }

        *InterfaceClass = NewInterfaceClass;
    }
}
