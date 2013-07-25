#ifndef __Nidra_h
#define __Nidra_h


#include "CutFlow.h"
#include "Process.h"

struct _object;
typedef _object PyObject;

namespace roast {
    template<typename T>
    long analyze(roast::Process&, const std::vector<std::string>&, const int&, PyObject*);

    namespace ttl {
        long analyze(roast::Process&, const std::vector<std::string>&, const int&, PyObject*);
    }
}

#endif
