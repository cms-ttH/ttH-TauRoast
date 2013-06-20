#ifndef __Nidra_h
#define __Nidra_h

#include <iostream>
#include <sstream>

#include "CutFlow.h"
#include "Process.h"

namespace roast {
    template<typename T> void analyze(roast::Process&, const std::vector<std::string>&, const int&);

    namespace ttl {
        void analyze(roast::Process&, const std::vector<std::string>&, const int&);
    }
}


#endif
