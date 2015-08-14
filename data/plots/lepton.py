from ttH.TauRoast.botany import Leaf
from ttH.TauRoast.plotting import Plot
from ttH.TauRoast.useful import config

for n in range(config.leptons):
    lbl = "l_{{{0}}} ".format(n + 1)

    Leaf('lep{0}_pt'.format(n + 1), 'f', 'result = leptons[{0}].p4().Pt()'.format(n))
    Leaf('lep{0}_eta'.format(n + 1), 'f', 'result = leptons[{0}].p4().Eta()'.format(n))
    Leaf('lep{0}_phi'.format(n + 1), 'f', 'result = leptons[{0}].p4().Phi()'.format(n))

    # Kinematics
    Plot(
        name="leptons/kinematic/L{0}_Pt".format(n + 1),
        values=["lep{0}_pt".format(n + 1)],
        labels=[lbl + "P_{T}", "Events"],
        binning=[20, 0, 100]
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

    Leaf('lep{0}_genid'.format(n + 1), 'f', 'result = abs(leptons[{0}].pdgId())'.format(n))
    Leaf('lep{0}_genparentid'.format(n + 1), 'f', 'result = abs(leptons[{0}].parentId())'.format(n))

    # Gen matching
    Plot(
            name="leptons/generator/L{0}_Match".format(n + 1),
            values=["lep{0}_genid".format(n + 1)],
            labels=[lbl + " id", "Events"],
            binning=[30, 0, 30]
    )
    Plot(
            name="leptons/generator/L{0}_ParentMatch".format(n + 1),
            values=["lep{0}_genparentid".format(n + 1)],
            labels=[lbl + "parent id", "Events"],
            binning=[30, 0, 30]
    )

    Leaf('lep{0}_ip'.format(n + 1), 'f', 'result = abs(leptons[{0}].impactParameter())'.format(n))
    Leaf('lep{0}_dz'.format(n + 1), 'f', 'result = abs(leptons[{0}].correctedDZ())'.format(n))
    Leaf('lep{0}_reliso'.format(n + 1), 'f', 'result = abs(leptons[{0}].relativeIsolation())'.format(n))

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
