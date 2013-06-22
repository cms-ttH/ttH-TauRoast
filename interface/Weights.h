#ifndef __Weights_h
#define __Weights_h

namespace roast {
    struct Weights {
        double topPtCorrection;
        double leptonCorrection;
        double puCorrection;
        double tau1Trigger;
        double tau2Trigger;
        double tauIdSys;
        double q2Sys;
        double jetCSV;
        double total;

        Weights() :
            topPtCorrection(0.),
            leptonCorrection(0.),
            puCorrection(0.),
            tau1Trigger(0.),
            tau2Trigger(0.),
            tauIdSys(0.),
            q2Sys(0.),
            jetCSV(0.),
            total(0.) {};

        ClassDef(Weights, 1);
    };
}

ClassImp(roast::Weights)

#endif
