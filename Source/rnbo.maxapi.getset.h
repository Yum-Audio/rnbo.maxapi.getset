#pragma once

#define TEST_GETTERS true
#define TEST_SETTERS false

static_assert (TEST_GETTERS || TEST_SETTERS, "At least one mechanism must be tested.");
static_assert (TEST_GETTERS != TEST_SETTERS, "Only one mechanism can be tested at a time.");

#include "c74_min.h"

typedef c74::min::timer<c74::min::timer_options::deliver_on_scheduler> MaxTimer;

using namespace c74;
using namespace c74::min;

class rnbo_maxapi_getset : public object<rnbo_maxapi_getset>
{
public:
    rnbo_maxapi_getset (const min::atoms &args = {});

    MIN_DESCRIPTION { "RNBO MaxAPI getset demo" };
    MIN_TAGS        { "utilities" };
    MIN_AUTHOR      { "Yum Audio" };
    MIN_RELATED     {""};

    inlet<> inlet;
    outlet<> outlet;

  private:
    message<> patchlineupdate;
    void handlePatchlineUpdate (const atoms& args);
    std::string getClassnameForMaxObject (max::t_object *obj);
    max::t_object *currentlyConnectedRnboObject = nullptr;

    MaxTimer getterPrintTimer;
    void onGetterPrintTimer ();

    MaxTimer setterPrintTimer;
    void onSetterPrintTimer ();
    float currentFloatSetterValue = 0.0f;
    std::string currentEnumSetterValue = "Opt1";
    void incrementSetterValues ();

};

