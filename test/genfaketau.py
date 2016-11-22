#!/usr/bin/env python
# vim: set fileencoding=utf-8 :

import math
import matplotlib
import matplotlib.pyplot as plt
# import pandas as pd
import numpy as np
from root_pandas import read_root

matplotlib.rc('font', **{'sans-serif': 'Liberation Sans', 'family': 'sans-serif'})

numbers_in = ['nloosetaus', 'ntighttaus', 'nfaketaus', 'nfaketighttaus', 'nrealtaus', 'nrealtighttaus']
numbers = read_root("test/genfaketau/out/ntuple.root", "ttjets", columns=numbers_in)

fig = plt.figure()
numbers.plot.hist(
    stacked=False,
    subplots=True,
    layout=(3, 2),
    sharey=True,
    ylim=(.5, 1e5),
    logy=True,
    figsize=(8.5, 11),
    grid=True
)
for ax in fig.get_axes():
    ax.set_yscale('log', nonposy='clip')
plt.savefig('jetfakes_numbers.png')

quants = 'eta pt chargedPt constituents chargedConstituents'.split()
quants += 'closestdr closestpt closestparticledr closestparticlept'.split()
quants += 'lpt signalPt signalChargedPt signalConstituents signalChargedConstituents'.split()
quants += 'isoPt isoChargedPt isoConstituents isoChargedConstituents'.split()

taus_in = ['genjet_' + q.lower() for q in quants] + ['isoMVA03', 'antiElectron', 'antiMuon', 'match', 'pt']
taus_in = ['tau_' + v for v in taus_in]
taus = read_root("test/genfaketau/out/ntuple.root", "ttjets", columns=taus_in, flatten=True)

gen_in = [q.lower() for q in quants]
gen_in = ['genjet_' + v for v in gen_in]
alljets = read_root("test/genfaketau/out/ntuple.root", "ttjets", columns=gen_in, flatten=True)
loosejets = alljets[
    (alljets.genjet_pt > 18)
    & (alljets.genjet_eta > -2.5)
    & (alljets.genjet_eta < 2.5)
    & (alljets.genjet_closestparticledr > 0.1)
]
jets = alljets[
    (alljets.genjet_pt > 18)
    & (alljets.genjet_eta > -2.5)
    & (alljets.genjet_eta < 2.5)
    & (alljets.genjet_closestparticledr > 0.1)
    & (alljets.genjet_constituents <= 22)
    & (alljets.genjet_chargedconstituents <= 11)
    & (alljets.genjet_isoconstituents <= 12)
    & (alljets.genjet_isochargedpt < 10)
    & (alljets.genjet_signalpt > 16)
    & (alljets.genjet_signalchargedconstituents <= 5)
]

print "before", len(alljets)
print "preselected", len(loosejets)
print "after", len(jets)

fakes = taus[(taus.tau_match == 6)]
selection = taus[
    (taus.tau_match == 6)
    & (taus.tau_isoMVA03 >= 4)
    # & (taus.tau_antiElectron >= 3)
    # & (taus.tau_antiMuon >= 3)
    & (taus.tau_pt >= 20.)
]

print "fakes before", len(fakes)
print "fakes after", len(selection)

pt_binning = range(0, 150, 10) + [10000]
const_binning = range(0, 15) + [10000]

with open('src/FakeData.frag', 'w') as dumpf:
    def dump(name, bins, edges):
        fmt = "std::vector<double> {}{{{}}};\n"
        dumpf.write(fmt.format(name + "_bins", ", ".join(map(str, bins))))
        dumpf.write(fmt.format(name + "_edges", ", ".join(map(str, edges[:-1]))))

    bins, edges = np.histogram(selection.tau_genjet_pt, bins=pt_binning, density=True)
    dump("fake_pt", bins, edges)
    bins, edges = np.histogram(selection.tau_genjet_chargedpt, bins=pt_binning, density=True)
    dump("fake_charged_pt", bins, edges)
    bins, edges = np.histogram(selection.tau_genjet_constituents, bins=const_binning, density=True)
    dump("fake_constituents", bins, edges)
    bins, edges = np.histogram(selection.tau_genjet_chargedconstituents, bins=const_binning, density=True)
    dump("fake_charged_constituents", bins, edges)

    bins, edges = np.histogram(jets.genjet_pt, bins=pt_binning, density=True)
    dump("jet_pt", bins, edges)
    bins, edges = np.histogram(jets.genjet_chargedpt, bins=pt_binning, density=True)
    dump("jet_charged_pt", bins, edges)
    bins, edges = np.histogram(jets.genjet_constituents, bins=const_binning, density=True)
    dump("jet_constituents", bins, edges)
    bins, edges = np.histogram(jets.genjet_chargedconstituents, bins=const_binning, density=True)
    dump("jet_charged_constituents", bins, edges)

labels = ['genjets', u'genjet fakes']
pspace = [
    ('pt', 'pt [GeV]', range(0, 201, 5)),
    ('eta', 'eta', np.array(range(-50, 51, 2)) * 0.1),
    ('lpt', 'leading track pt [GeV]', range(0, 81, 2)),
    ('chargedpt', 'charged pt component [GeV]', range(0, 201, 5)),
    ('constituents', 'constituent count', range(0, 31, 1)),
    ('chargedconstituents', 'charged constituent count', range(0, 31, 1)),
    # ('neutralconstituents', range(0, 31, 1)),
    ('isoconstituents', u'constintuent count within 0.12 < ΔR < 0.3', range(0, 31, 1)),
    ('signalconstituents', u'constituent count within ΔR < 0.12', range(0, 31, 1)),
    ('isochargedconstituents', u'charged constintuent count within 0.12 < ΔR < 0.3', range(0, 31, 1)),
    ('signalchargedconstituents', u'charged constituent count within ΔR < 0.12', range(0, 31, 1)),
    ('isopt', u'pt component within 0.12 < ΔR < 0.3', range(0, 26, 1)),
    ('signalpt', u'pt component within ΔR < 0.12', range(0, 101, 2)),
    ('isochargedpt', u'charged pt component within 0.12 < ΔR < 0.3', range(0, 26, 1)),
    ('signalchargedpt', u'pt charged component within ΔR < 0.12', range(0, 101, 2)),
    ('closestpt', 'pt of closest genjet', range(0, 201, 5)),
    ('closestdr', u'ΔR of closest genjet', np.array(range(41)) * 0.05 * math.pi),
    ('closestparticlept', 'pt of closest particle', range(0, 201, 5)),
    ('closestparticledr', u'ΔR of closest particle', np.array(range(41)) * 0.05 * math.pi)
]

for name, title, nbins in pspace:
    print 'Processing', name
    fig = plt.figure()
    plt.title(title)
    plt.hist([loosejets['genjet_' + name]], bins=nbins, normed=1, alpha=0.2, label=['genjets'])
    plt.hist([jets['genjet_' + name]], bins=nbins, normed=1, alpha=0.5, label=['selected genjets'])
    plt.hist([selection['tau_genjet_' + name]], bins=nbins, normed=1, alpha=0.7, label=[u'genjets of τ fakes'])
    # n, bins, patches = plt.hist([jets['genjet_' + name], selection['tau_genjet_' + name]], bins=nbins, normed=1)
    # plt.legend(patches, labels)
    plt.legend()
    plt.savefig('jetfakes_genjet_{}.png'.format(name))
