#ifndef RC_UNREAL_FFRAME_HPP
#define RC_UNREAL_FFRAME_HPP

#include <Unreal/Common.hpp>

namespace RC::Unreal
{
    class UObject;
    class UProperty;
    class UField;
    class FOutParmRec;
    class UFunction;

    using FlowStackType = uint8_t[0x30];

    class RC_API FOutputDevice
    {
    protected:
        uint8_t* vtable;

        /** Whether to output the 'Log: ' type front... */
        bool bSuppressEventTag;
        /** Whether to output a line-terminator after each log call... */
        bool bAutoEmitLineTerminator;
    };

    struct RC_API FFrame : public FOutputDevice
    {
        // Variables.
        UFunction* Node;
        UObject* Object;
        uint8_t Code;
        uint8_t* Locals;

        UProperty* MostRecentProperty;
        uint8_t* MostRecentPropertyAddress;

        /** The execution flow stack for compiled Kismet code */
        FlowStackType FlowStack;

        /** Previous frame on the stack */
        FFrame* PreviousFrame;

        /** contains information on any out parameters */
        FOutParmRec* OutParms;

        /** If a class is compiled in then this is set to the property chain for compiled-in functions. In that case, we follow the links to setup the args instead of executing by code. */
        UField* PropertyChainForCompiledIn;

        /** Currently executed native function */
        UFunction* CurrentNativeFunction;

        bool bArrayContextFailed;
    };
}

#endif //RC_UNREAL_FFRAME_HPP
