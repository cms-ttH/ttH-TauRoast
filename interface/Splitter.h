// vim: ts=4:sw=4:et:sta
#ifndef Splitter_h
#define Splitter_h

#include "Branches.h"
#include "TLBranches.h"

namespace roast {
    class Splitter {
        public:
            virtual bool Use(Branches*, int) = 0;
            ClassDef(Splitter, 1);
    };

    class FakeSplitter : Splitter {
        public:
            FakeSplitter(bool real) : _real(real) {};
            virtual bool Use(Branches *b, int idx) {
                if (tl::Branches *e = dynamic_cast<tl::Branches*>(b)) {
                    bool genmatch = abs(e->T_GenMatchId->at(idx)) == 15;
                    return genmatch == _real;
                }
                return false;
            };
        private:
            bool _real;
            ClassDef(FakeSplitter, 1);
    };

    class InclusiveSignalSplitter : Splitter {
        public:
            InclusiveSignalSplitter(int mode) : _mode(mode) {};
            virtual bool Use(Branches *b, int idx) { return (b->higgsDecayMode == _mode); };
        private:
            int _mode;
            ClassDef(InclusiveSignalSplitter, 1);
    };

    class TTbarSplitter : Splitter {
        public:
            TTbarSplitter(int mode) : _mode(mode) {};
            virtual bool Use(Branches *b, int idx) {
                if (b->bQuarkCount >= 2)
                    return 3;
                if (b->bQuarkCount == 1)
                    return 2;
                if (b->cQuarkCount == 2)
                    return 1;
                return 0;
            };
        private:
            int _mode;
            ClassDef(TTbarSplitter, 1);
    };
}

#endif
