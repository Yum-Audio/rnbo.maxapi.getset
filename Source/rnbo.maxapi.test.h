#pragma once

#include "c74_min.h"


typedef c74::min::timer<c74::min::timer_options::deliver_on_scheduler> MaxTimer;

using namespace c74;
using namespace c74::min;

class rnbo_maxapi_test : public object<rnbo_maxapi_test>,
                         public mc_operator<>
{
public:
  rnbo_maxapi_test(const min::atoms &args = {});

  	MIN_DESCRIPTION	{ "RNBO MaxAPI test" };
    MIN_TAGS		    { "utilities" };
    MIN_AUTHOR	    { "Yum Audio" };
    MIN_RELATED     {""};

    inlet<> mcInlet;
    outlet<> mcOutlet;

    void operator()(audio_bundle input, audio_bundle output){}

  private:
    
  	message<> patchlineupdate;
	  void handlePatchlineUpdate (const atoms& args);
    std::string getClassnameForMaxObject(max::t_object *obj);
    max::t_object *currentlyConnectedRnboObject = nullptr;

    message<> notify;
    void handleNotify(const atoms &args);

    MaxTimer getterPrintTimer;
    void onGetterPrintTimer ();

};

