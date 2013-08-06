// vim: ts=4:sw=4:et:sta
#ifndef __Nidra_h
#define __Nidra_h


#include "CutFlow.h"
#include "Picker.h"
#include "Process.h"
#include "Splitter.h"
#include "Weight.h"

struct _object;
typedef _object PyObject;

namespace roast {
    template<typename T>
    long analyze(roast::Process&, const std::vector<roast::CutFlow::Cut>&, const int&, PyObject*);
    template<typename T>
    long fill(roast::Process&, std::vector<roast::Weight>&, PyObject*, roast::Splitter*, roast::Picker*);

    namespace tll {
        long analyze(roast::Process&, const std::vector<roast::CutFlow::Cut>&, const int&, PyObject*);
        long fill(roast::Process&, std::vector<roast::Weight>&, PyObject*, roast::Splitter* s=0, roast::Picker* p=0);
    }

    namespace ttl {
        long analyze(roast::Process&, const std::vector<roast::CutFlow::Cut>&, const int&, PyObject*);
        long fill(roast::Process&, std::vector<roast::Weight>&, PyObject*, roast::Splitter* s=0, roast::Picker* p=0);
    }
}

#endif
