#include "rnbo.maxapi.getset.h"

MIN_EXTERNAL (rnbo_maxapi_getset);

rnbo_maxapi_getset::rnbo_maxapi_getset (const min::atoms &a)
:
inlet {this, ""},
outlet {this, "() Connect me to a rnbo~ object"},
patchlineupdate {this, "patchlineupdate", MIN_FUNCTION { handlePatchlineUpdate(args); return {}; }},
getterPrintTimer (this, MIN_FUNCTION { onGetterPrintTimer (); return {}; }),
setterPrintTimer (this, MIN_FUNCTION { onSetterPrintTimer (); return {}; })
{
    max::common_symbols_init();

    if constexpr (TEST_SETTERS)
        setterPrintTimer.delay (2000);
    else if constexpr (TEST_GETTERS)
        getterPrintTimer.delay (2000);

}

void rnbo_maxapi_getset::onGetterPrintTimer ()
{
    if (currentlyConnectedRnboObject != nullptr) 
    {
        cout << "" << endl;
        const auto paramVal = max::object_attr_getfloat (
            currentlyConnectedRnboObject,
            max::gensym("testParam")
        );

        cout << "testParam: " << paramVal << endl;
      
        if (auto enumSym = max::object_attr_getsym (
                            currentlyConnectedRnboObject, 
                            max::gensym ("testEnumParam"))
           )
        {
            if (auto enumVal = enumSym->s_name)
                cout << "testEnumParam: " << enumVal << endl;
            
        }
    }

    getterPrintTimer.delay (2000);
}

void rnbo_maxapi_getset::onSetterPrintTimer ()
{
    if (currentlyConnectedRnboObject != nullptr) 
    {
        incrementSetterValues ();

        cout << "Setting testParam to " << currentFloatSetterValue << " and testEnumParam to " << currentEnumSetterValue << endl;

        max::object_attr_setfloat (
            currentlyConnectedRnboObject,
            max::gensym ("testParam"),
            currentFloatSetterValue
        );

        max::object_attr_setsym (
            currentlyConnectedRnboObject,
            max::gensym ("testEnumParam"),
            max::gensym (currentEnumSetterValue.c_str ())
        );
    }

    setterPrintTimer.delay (2000);
}

void rnbo_maxapi_getset::incrementSetterValues ()
{
    // rnbo param is int 0-9 so we'll just cycle through those
    currentFloatSetterValue = (static_cast<int> (currentFloatSetterValue) + 1) % 10;

    // rnbo param is enum Opt1, Opt2, Opt3
    if (currentEnumSetterValue == "Opt1")
        currentEnumSetterValue = "Opt2";
    else if (currentEnumSetterValue == "Opt2")
        currentEnumSetterValue = "Opt3";
    else if (currentEnumSetterValue == "Opt3")
        currentEnumSetterValue = "Opt1";
}



void rnbo_maxapi_getset::handlePatchlineUpdate (const atoms &args)
{
    max::t_object* x = args[0];
    max::t_object* patchline = args[1];
    t_atom_long updatetype = args[2];
    max::t_object* src = args[3];
    t_atom_long srcout = args[4];
    max::t_object* dst = args[5];
    t_atom_long dstin = args[6];

    const auto thisIsSource = src == maxobj ();
    const auto thisIsDestination = dst == maxobj ();

    if (thisIsSource)
    {
        const auto destClassName = getClassnameForMaxObject (dst);

        if (destClassName == "rnbo~")
        {
            if (updatetype == 1 /*isConnected*/)
            {
                currentlyConnectedRnboObject = dst;
                cout << "rnbo~ object connected" << endl;
            }
            else
            {
                currentlyConnectedRnboObject = nullptr;
                cout << "rnbo~ object disconnected" << endl;
            }
        }
        
    }
    else if (thisIsDestination)
    {
        ///
    }
    else 
    {
        // you should never get here
        assert (false);
    }
}

std::string rnbo_maxapi_getset::getClassnameForMaxObject (max::t_object *obj)
{
    if (obj == nullptr || max::object_classname (obj) == nullptr)
        return std::string("");
    
    return std::string (max::object_classname (obj)->s_name);
}