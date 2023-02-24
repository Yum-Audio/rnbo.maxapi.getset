#include "rnbo.maxapi.test.h"

MIN_EXTERNAL (rnbo_maxapi_test);

rnbo_maxapi_test::rnbo_maxapi_test (const min::atoms &a)
:
mcInlet {this, "(multichannelsignal) Audio In", "multichannelsignal"},
mcOutlet {this, "() Connect me to a rnbo~ object"},
patchlineupdate {this, "patchlineupdate", MIN_FUNCTION { handlePatchlineUpdate(args); return {}; }},
getterPrintTimer (this, MIN_FUNCTION { onGetterPrintTimer (); return {}; })     
{
    max::common_symbols_init();
    getterPrintTimer.delay (2000);
}

void rnbo_maxapi_test::onGetterPrintTimer ()
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
                max::gensym ("testEnumParam")
            ))
        {
            if (auto enumVal = enumSym->s_name)
            {
                cout << "enumVal->s_name: " << enumVal << endl;
            }
            else
            {
                cout << "enumVal->s_name == nullptr" << endl;
            }
        }
        else
        {
            cout << "enumSym == nullptr" << endl;
        }
    }

    getterPrintTimer.delay (2000);
}

void rnbo_maxapi_test::handlePatchlineUpdate (const atoms &args)
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
        ///
    }
}

std::string rnbo_maxapi_test::getClassnameForMaxObject (max::t_object *obj)
{
    if (obj == nullptr || max::object_classname (obj) == nullptr)
        return std::string("");
    
    return std::string (max::object_classname (obj)->s_name);
}