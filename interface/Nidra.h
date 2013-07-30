// vim: ts=4:sw=4:et:sta
#ifndef __Nidra_h
#define __Nidra_h


#include "CutFlow.h"
#include "Process.h"

struct _object;
typedef _object PyObject;

namespace roast {
    template<typename T>
    long analyze(roast::Process&, const std::vector<roast::CutFlow::Cut>&, const int&, PyObject*);

    namespace tll {
        long analyze(roast::Process&, const std::vector<roast::CutFlow::Cut>&, const int&, PyObject*);
    }

    namespace ttl {
        long analyze(roast::Process&, const std::vector<roast::CutFlow::Cut>&, const int&, PyObject*);
    }
}

#endif
