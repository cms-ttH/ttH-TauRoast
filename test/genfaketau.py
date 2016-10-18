#!/usr/bin/env python

import matplotlib.pyplot as plt
# import pandas as pd
import numpy as np
from root_pandas import read_root

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

taus_in = 'pt eta isoMVA03 genjet_pt genjet_eta genjetcharged_pt genjetcharged_eta match genjet_constituents genjetcharged_constituents pfake pjet'
taus_in = ['tau_' + v for v in taus_in.split()]
taus = read_root("test/genfaketau/out/ntuple.root", "ttjets", columns=taus_in, flatten=True)

gen_in = 'chargedconstituents constituents chargedpt pt eta pjet pfake'
gen_in = ['genjet_' + v for v in gen_in.split()]
jets = read_root("test/genfaketau/out/ntuple.root", "ttjets", columns=gen_in, flatten=True)

fakes = taus[(taus.tau_match == 6)]
selection = taus[(taus.tau_match == 6) & (taus.tau_isoMVA03 >= 5) & (taus.tau_pt >= 20.)]

pt_binning = range(0, 150, 10) + [10000]
const_binning = range(0, 15) + [10000]

with open('src/FakeData.frag', 'w') as dumpf:
    def dump(name, bins, edges):
        fmt = "std::vector<double> {}{{{}}};\n"
        dumpf.write(fmt.format(name + "_bins", ", ".join(map(str, bins))))
        dumpf.write(fmt.format(name + "_edges", ", ".join(map(str, edges[:-1]))))

    bins, edges = np.histogram(selection.tau_genjet_pt, bins=pt_binning, density=True)
    dump("fake_pt", bins, edges)
    bins, edges = np.histogram(selection.tau_genjetcharged_pt, bins=pt_binning, density=True)
    dump("fake_charged_pt", bins, edges)
    bins, edges = np.histogram(selection.tau_genjet_constituents, bins=const_binning, density=True)
    dump("fake_constituents", bins, edges)
    bins, edges = np.histogram(selection.tau_genjetcharged_constituents, bins=const_binning, density=True)
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
nbins = range(0, 201, 10)

fig = plt.figure()
n, bins, patches = plt.hist([jets.genjet_pt, selection.tau_genjet_pt], bins=nbins, normed=1)
plt.legend(patches, labels)
plt.savefig('jetfakes_genjet_pt.png')

fig = plt.figure()
n, bins, patches = plt.hist([jets.genjet_chargedpt, selection.tau_genjetcharged_pt], bins=nbins, normed=1)
plt.legend(patches, labels)
plt.savefig('jetfakes_genjet_chargedpt.png')

nbins = range(0, 31, 1)

fig = plt.figure()
n, bins, patches = plt.hist([jets.genjet_constituents, selection.tau_genjet_constituents], bins=nbins, normed=1)
plt.legend(patches, labels)
plt.savefig('jetfakes_genjet_constituents.png')

fig = plt.figure()
n, bins, patches = plt.hist([jets.genjet_chargedconstituents, selection.tau_genjetcharged_constituents], bins=nbins, normed=1)
plt.legend(patches, labels)
plt.savefig('jetfakes_genjet_chargedconstituents.png')

# Probability for fakes/jet

nbins = 10

fig = plt.figure()
n, bins, patches = plt.hist([jets.genjet_pfake, selection.tau_pfake], bins=nbins, normed=1)
plt.legend(patches, labels)
plt.savefig('jetfakes_genjet_pfake.png')

fig = plt.figure()
n, bins, patches = plt.hist([jets.genjet_pjet, selection.tau_pjet], bins=nbins, normed=1)
plt.legend(patches, labels)
plt.savefig('jetfakes_genjet_pjet.png')

print jets.genjet_pfake / jets.genjet_pjet

nbins = range(0, 30, 3)

fig = plt.figure()
n, bins, patches = plt.hist([jets.genjet_pfake / jets.genjet_pjet, selection.tau_pfake / selection.tau_pjet], bins=nbins, normed=1)
plt.legend(patches, labels)
plt.savefig('jetfakes_genjet_pfake_over_pjet.png')

nbins = [range(0, 100, 5), range(0, 20, 1)]

fig = plt.figure()
plt.hist2d(jets.genjet_chargedpt, jets.genjet_chargedconstituents, bins=nbins)
plt.xlabel('genjet charged pt')
plt.ylabel('genjet charged constituents')
plt.savefig('genjet_chargedpt_vs_chargedconstituents.png')

fig = plt.figure()
plt.hist2d(selection.tau_genjetcharged_pt, selection.tau_genjetcharged_constituents, bins=nbins)
plt.xlabel('fake tau charged pt')
plt.ylabel('fake tau charged constituents')
plt.savefig('faketau_chargedpt_vs_chargedconstituents.png')

fig = plt.figure()
plt.hist2d(jets.genjet_pt, jets.genjet_constituents, bins=nbins)
plt.xlabel('genjet pt')
plt.ylabel('genjet constituents')
plt.savefig('genjet_pt_vs_constituents.png')

fig = plt.figure()
plt.hist2d(selection.tau_genjet_pt, selection.tau_genjet_constituents, bins=nbins)
plt.xlabel('fake tau pt')
plt.ylabel('fake tau constituents')
plt.savefig('faketau_pt_vs_constituents.png')
