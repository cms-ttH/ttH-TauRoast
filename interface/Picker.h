// vim: ts=4:sw=4:et:sta
#ifndef Picker_h
#define Picker_h

#include "Branches.h"

namespace roast {
    class Picker {
        public:
            virtual ~Picker() {};
            virtual int Pick(Branches *, const std::vector<int>&) = 0;
            ClassDef(Picker, 1);
    };

    class PtPicker : Picker {
        public:
            virtual int Pick(Branches *b, const std::vector<int>& idxs) { return idxs[0]; };
            ClassDef(PtPicker, 1);
    };

    namespace tl {
        class IsoPicker : roast::Picker {
            public:
                virtual int Pick(roast::Branches*, const std::vector<int>&);
                ClassDef(IsoPicker, 1);
        };
    }

    namespace tll {
        class IsoPicker : roast::Picker {
            public:
                virtual int Pick(roast::Branches*, const std::vector<int>&);
                ClassDef(IsoPicker, 1);
        };
    }

    namespace ttl {
        class IsoPicker : roast::Picker {
            public:
                virtual int Pick(roast::Branches*, const std::vector<int>&);
                ClassDef(IsoPicker, 1);
        };
    }
}

#endif
