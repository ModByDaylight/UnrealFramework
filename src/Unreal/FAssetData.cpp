#include <Unreal/FAssetData.hpp>
#include <Unreal/UClass.hpp>
#include <Unreal/UScriptStruct.hpp>

namespace RC::Unreal {
    UScriptStruct* FAssetData::InternalStaticStruct{nullptr};

    FAssetData::FAssetData() {
        StaticStruct()->InitializeStruct(this);
    }

    FAssetData::FAssetData(const FAssetData& InData) {
        StaticStruct()->CopyScriptStruct(this, &InData);
    }

    FAssetData::~FAssetData() {
        StaticStruct()->DestroyStruct(this);
    }

    UScriptStruct* FAssetData::StaticStruct() {
        if (InternalStaticStruct == nullptr) {
            throw std::runtime_error{"Attempt to retrieve StaticStruct before it has been initialized"};
        }
        if (InternalStaticStruct->GetStructureSize() != sizeof(FAssetData)) {
            throw std::runtime_error{"FAssetData StructureSize mismatch found"};
        }
        return InternalStaticStruct;
    }
}
