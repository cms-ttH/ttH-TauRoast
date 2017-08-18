from ttH.TauRoast.botany import Leaf
from ttH.TauRoast.plotting import Plot
from ttH.TauRoast.useful import config

Leaf('lep_pt', '[f]', 'result.resize(leptons.size()); std::transform(leptons.begin(), leptons.end(), result.begin(), [](const superslim::Lepton& t) { return t.p4().pt(); })')
Plot(
    name="leptons/kinematic/L_Pt",
    values=["lep_pt"],
    labels=["#ell P_{T}", "Events"],
    binning=[10, 10, 110],
    essential=True
)

Leaf('lep_eta', '[f]', 'result.resize(leptons.size()); std::transform(leptons.begin(), leptons.end(), result.begin(), [](const superslim::Lepton& t) { return t.p4().eta(); })')
Plot(
    name="leptons/kinematic/L_Eta",
    values=["lep_eta"],
    labels=["#ell #eta", "Events"],
    binning=[10, -2.5, 2.5],
    essential=True
)

for n in range(config.leptons):
    lbl = "l_{{{0}}} ".format(n + 1)

    Leaf('lep{0}_pt'.format(n + 1), 'f', 'result = leptons[{0}].p4().Pt()'.format(n))
    Leaf('lep{0}_eta'.format(n + 1), 'f', 'result = leptons[{0}].p4().Eta()'.format(n))
    Leaf('lep{0}_abseta'.format(n + 1), 'f', 'result = std::abs(leptons[{0}].p4().Eta())'.format(n))
    Leaf('lep{0}_phi'.format(n + 1), 'f', 'result = leptons[{0}].p4().Phi()'.format(n))

    Leaf('lep{0}_genpt'.format(n + 1), 'f', 'result = leptons[{0}].genP4().Pt()'.format(n))
    Leaf('lep{0}_geneta'.format(n + 1), 'f', 'result = leptons[{0}].genP4().Eta()'.format(n))

    for t in range(config.taus):
        Leaf('lep{0}tau{1}_deltaR'.format(n + 1, t + 1), 'f',
             'result = dR(leptons.at({0}), taus.at({1}))'.format(n, t))
        Leaf('lep{0}tau{1}_cosDeltaPhi'.format(n + 1, t + 1), 'f',
             'result = std::cos(leptons.at({0}).p4().phi() - taus.at({1}).p4().phi())'.format(n, t))

        Plot(
            name="leptons/L{}T{}_DeltaR".format(n + 1, t + 1),
            values=["lep{}tau{}_deltaR".format(n + 1, t + 1)],
            labels=["#DeltaR {}, #tau_{{{}}}".format(lbl, t + 1), "Events"],
            binning=[15, 0, 6.28]
        )
        Plot(
            name="leptons/L{}T{}_cosDeltaPhi".format(n + 1, t + 1),
            values=["lep{}tau{}_cosDeltaPhi".format(n + 1, t + 1)],
            labels=["#DeltaR {}, #tau_{{{}}}".format(lbl, t + 1), "Events"],
            binning=[15, -1, 1]
        )

    if (config.taus) == 2:
        Leaf('lep{0}tauOS_deltaR'.format(n + 1), 'f',
             'result = dR(leptons.at({0}), taus.at(0).charge() * leptons.at({0}).charge() < 0 ? taus.at(0) : taus.at(1))'.format(n))
        Leaf('lep{0}tauSS_deltaR'.format(n + 1), 'f',
             'result = dR(leptons.at({0}), taus.at(0).charge() * leptons.at({0}).charge() > 0 ? taus.at(0) : taus.at(1))'.format(n))
        Plot(
            name="leptons/L{}TOS_DeltaR".format(n + 1),
            values=["lep{}tauOS_deltaR".format(n + 1)],
            labels=["#DeltaR {}, #tau_{{OS}}".format(lbl), "Events"],
            binning=[15, 0, 6.28]
        )
        Plot(
            name="leptons/L{}TSS_DeltaR".format(n + 1),
            values=["lep{}tauSS_deltaR".format(n + 1)],
            labels=["#DeltaR {}, #tau_{{SS}}".format(lbl), "Events"],
            binning=[15, 0, 6.28]
        )

        Leaf('lep{0}tauOS_cosDeltaPhi'.format(n + 1), 'f',
             'result = std::cos(leptons.at({0}).p4().Phi() - (taus.at(0).charge() * leptons.at({0}).charge() < 0 ? taus.at(0) : taus.at(1)).p4().Phi())'.format(n))
        Leaf('lep{0}tauSS_cosDeltaPhi'.format(n + 1), 'f',
             'result = std::cos(leptons.at({0}).p4().Phi() - (taus.at(0).charge() * leptons.at({0}).charge() > 0 ? taus.at(0) : taus.at(1)).p4().Phi())'.format(n))
        Plot(
            name="leptons/L{}TOS_CosDeltaPhi".format(n + 1),
            values=["lep{}tauOS_cosDeltaPhi".format(n + 1)],
            labels=["cos(#Delta #phi) {}, #tau_{{OS}}".format(lbl), "Events"],
            binning=[15, -1, 1]
        )
        Plot(
            name="leptons/L{}TSS_CosDeltaPhi".format(n + 1),
            values=["lep{}tauSS_cosDeltaPhi".format(n + 1)],
            labels=["cos(#Delta #phi {}, #tau_{{SS}}".format(lbl), "Events"],
            binning=[15, -1, 1]
        )

    Leaf('lep{}jet_deltaRmin'.format(n + 1), 'f',
         '''std::vector<float> drs(jets.size());
            std::transform(jets.begin(), jets.end(), drs.begin(),
                           [&](const superslim::Jet& j) {{ return dR(leptons.at({}), j); }});
            result = *std::min_element(drs.begin(), drs.end())'''.format(n))
    Plot(
        name="leptons/L{}J_MinDeltaR".format(n + 1),
        values=["lep{}jet_deltaRmin".format(n + 1)],
        labels=["Min #DeltaR {}, jet".format(lbl), "Events"],
        binning=[15, 0, 6.28]
    )

    Leaf('lep{0}_mt'.format(n + 1), 'f',
         '''result = std::sqrt(2 * leptons[{0}].p4().Pt() * event.met().Pt()
                               * (1 - std::cos(leptons[{0}].p4().Phi() - event.met().Phi())))'''.format(n))
    Plot(
        name="leptons/L{0}_Mt".format(n + 1),
        values=["lep{0}_mt".format(n + 1)],
        labels=[lbl + "M_{T}", "Events"],
        binning=[40, 0, 120]
    )

    # Kinematics
    Plot(
        name="leptons/kinematic/L{0}_Pt".format(n + 1),
        values=["lep{0}_pt".format(n + 1)],
        labels=[lbl + "P_{T}", "Events"],
        binning=[20, 0, 150],
        essential=True
    )
    Plot(
        name="leptons/kinematic/L{0}_Eta".format(n + 1),
        values=["lep{0}_eta".format(n + 1)],
        labels=[lbl + "#eta", "Events"],
        binning=[20, -3, 3]
    )
    Plot(
        name="leptons/kinematic/L{0}_Phi".format(n + 1),
        values=["lep{0}_phi".format(n + 1)],
        labels=[lbl + "#Phi", "Events"],
        binning=[20, -3.2, 3.2]
    )

    Plot(
        name="leptons/generator/L{0}_GenPt".format(n + 1),
        values=["lep{0}_genpt".format(n + 1)],
        labels=[lbl + "gen P_{T}", "Events"],
        binning=[20, 0, 150]
    )
    Plot(
        name="leptons/generator/L{0}_GenPt_low".format(n + 1),
        values=["lep{0}_genpt".format(n + 1)],
        labels=[lbl + "gen P_{T}", "Events"],
        binning=[25, 0, 50]
    )
    Plot(
        name="leptons/generator/L{0}_GenEta".format(n + 1),
        values=["lep{0}_geneta".format(n + 1)],
        labels=[lbl + "gen #eta", "Events"],
        binning=[20, -3, 3]
    )

    Leaf('lep{0}_match'.format(n + 1), 'i', 'result = leptons[{0}].match()'.format(n))
    Leaf('lep{0}_id'.format(n + 1), 'i', 'result = leptons[{}].pdgId()'.format(n))
    Leaf('lep{0}_genid'.format(n + 1), 'f', 'result = abs(leptons[{0}].genPdgId())'.format(n))
    Leaf('lep{0}_genparentid'.format(n + 1), 'f', 'result = abs(leptons[{0}].parentId())'.format(n))

    # Gen matching
    Plot(
        name="leptons/generator/L{0}_Match".format(n + 1),
        values=["lep{0}_match".format(n + 1)],
        labels=[lbl + " id", "Events"],
        binning=[6, 1, 7],
        binlabels=["e", "#mu", "#tau #rightarrow e", "#tau #rightarrow #mu", "#tau #rightarrow #tau_{h}", "fake"]
    )
    Plot(
        name="leptons/generator/L{0}_GenId".format(n + 1),
        values=["lep{0}_genid".format(n + 1)],
        labels=[lbl + " id", "Events"],
        binning=[30, 0, 30]
    )
    Plot(
        name="leptons/generator/L{0}_ParentId".format(n + 1),
        values=["lep{0}_genparentid".format(n + 1)],
        labels=[lbl + "parent id", "Events"],
        binning=[30, 0, 30]
    )

    Leaf('lep{0}_ip'.format(n + 1), 'f', 'result = abs(leptons[{0}].impactParameter())'.format(n))
    Leaf('lep{0}_dz'.format(n + 1), 'f', 'result = abs(leptons[{0}].dz())'.format(n))
    Leaf('lep{0}_reliso'.format(n + 1), 'f', 'result = abs(leptons[{0}].relativeIsolation())'.format(n))
    Leaf('lep{0}_chargediso'.format(n + 1), 'f', 'result = abs(leptons[{0}].chargedIsolation())'.format(n))
    Leaf('lep{0}_neutraliso'.format(n + 1), 'f', 'result = abs(leptons[{0}].neutralIsolation())'.format(n))
    Leaf('lep{0}_mva'.format(n + 1), 'f', 'result = abs(leptons[{0}].mvaRaw())'.format(n))
    Leaf('lep{0}_nontrig'.format(n + 1), 'f', 'result = abs(leptons[{0}].nonTriggeringId())'.format(n))

    # ID
    Plot(
        name="leptons/id/L{0}_ImpactParameter".format(n + 1),
        values=["lep{0}_ip".format(n + 1)],
        labels=[lbl + "Impact parameter", "Events"],
        binning=[20, -0.04, 0.04]
    )
    Plot(
        name="leptons/id/L{0}_CorrectedDZ".format(n + 1),
        values=["lep{0}_dz".format(n + 1)],
        labels=[lbl + "corrected #Delta z", "Events"],
        binning=[20, -0.04, 0.04]
    )
    Plot(
        name="leptons/id/L{0}_RelIso".format(n + 1),
        values=["lep{0}_reliso".format(n + 1)],
        labels=[lbl + "RelIso", "Events"],
        binning=[20, 0, 0.25]
    )
    Plot(
        name="leptons/id/L{0}_ChargedIso".format(n + 1),
        values=["lep{0}_chargediso".format(n + 1)],
        labels=[lbl + "Charged Iso", "Events"],
        binning=[20, 0, 0.25]
    )
    Plot(
        name="leptons/id/L{0}_NeutralIso".format(n + 1),
        values=["lep{0}_neutraliso".format(n + 1)],
        labels=[lbl + "Neutral Iso", "Events"],
        binning=[20, 0, 0.25]
    )
    Plot(
        name="leptons/id/L{0}_MVA".format(n + 1),
        values=["lep{0}_mva".format(n + 1)],
        labels=[lbl + "MVA", "Events"],
        binning=[20, .5, 1]
    )
    Plot(
        name="leptons/id/L{0}_NonTrigID".format(n + 1),
        values=["lep{0}_nontrig".format(n + 1)],
        labels=[lbl + "Non Triggering ID", "Events"],
        binning=[20, 0, 0.25]
    )
