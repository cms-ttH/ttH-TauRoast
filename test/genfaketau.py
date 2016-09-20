#!/usr/bin/env python

import matplotlib.pyplot as plt
# import pandas as pd
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

taus_in = 'pt eta isoMVA03 genjet_pt genjet_eta genjetcharged_pt genjetcharged_eta match genjet_constituents genjetcharged_constituents'
taus_in = ['tau_' + v for v in taus_in.split()]
taus = read_root("test/genfaketau/out/ntuple.root", "ttjets", columns=taus_in, flatten=True)

fakes = taus[(taus.tau_match == 6)]
selection = taus[(taus.tau_match == 6) & (taus.tau_isoMVA03 >= 5) & (taus.tau_pt >= 20.)]

fig = plt.figure()
axs = fakes[[
    'tau_pt', 'tau_genjet_pt', 'tau_genjetcharged_pt'
]].plot.hist(subplots=True, layout=(3, 1), figsize=(11, 15), sharex=False, bins=30)
for ax, (col, vals) in zip(axs.flat, selection[['tau_pt', 'tau_genjet_pt', 'tau_genjetcharged_pt']].iteritems()):
    vals.hist(ax=ax, bins=30)
plt.savefig('jetfakes_variables.png')
