// vim: ts=4:sw=4:et:sta
#ifndef __acessor_h
#define __acessor_h

#include "Branches.h"

namespace roast {
    typedef float (*GetValue_t)(Branches*, const int&);

    GetValue_t get_accessor(const std::string&);
    void setup_accessors();
}

#endif
