// vim: ts=4:sw=4:et:sta
#ifndef Splitter_h
#define Splitter_h

#include "Branches.h"

namespace roast {
    class Splitter {
        public:
            virtual bool Use(Branches*, int) = 0;
            ClassDef(Splitter, 1);
    };

    class InclusiveSignalSplitter : Splitter {
        public:
            InclusiveSignalSplitter(int mode) : _mode(mode) {};
            virtual bool Use(Branches *b, int idx) { return (b->Ev_higgsDecayMode == _mode); };
        private:
            int _mode;
            ClassDef(InclusiveSignalSplitter, 1);
    };

    class TTbarSplitter : Splitter {
        public:
            TTbarSplitter(int mode) : _mode(mode) {};
            virtual bool Use(Branches *b, int idx) {
                if (b->Ev_bQuarkCount >= 2)
                    return 3;
                if (b->Ev_bQuarkCount == 1)
                    return 2;
                if (b->Ev_cQuarkCount == 2)
                    return 1;
                return 0;
            };
        private:
            int _mode;
            ClassDef(TTbarSplitter, 1);
    };
}

#endif
