#!/usr/bin/env python
# vim: set fileencoding=utf-8 :

import math
import matplotlib
import matplotlib.pyplot as plt
# import pandas as pd
import numpy as np
from root_pandas import read_root

matplotlib.rc('font', **{'sans-serif': 'Liberation Sans', 'family': 'sans-serif'})

quants = 'eta pt'.split()
quants += 'closestdr closestpt closestparticledr closestparticlept'.split()

leptons_in = ['genparticle_' + q.lower() for q in quants] + ['fakeable', 'mva', 'pt']
leptons_in = ['lepton_' + v for v in leptons_in]
leptons = read_root("test/genfaketau/out/ntuple.root", "ttjets", columns=leptons_in, flatten=True)

gen_in = [q.lower() for q in quants]
gen_in = ['genparticle_' + v for v in gen_in]
allparticles = read_root("test/genfaketau/out/ntuple.root", "ttjets", columns=gen_in, flatten=True)
looseparticles = allparticles
particles = allparticles

print "before", len(allparticles)
print "preselected", len(looseparticles)
print "after", len(particles)

selection = leptons[leptons.lepton_mva >= 3]

print "leptons before", len(leptons)
print "leptons after", len(selection)

pt_binning = range(0, 150, 10) + [10000]
const_binning = range(0, 15) + [10000]

labels = ['genparticles', u'tight leptons']
pspace = [
    ('pt', 'pt [GeV]', range(0, 201, 5)),
    ('eta', 'eta', np.array(range(-50, 51, 2)) * 0.1),
    ('closestpt', 'pt of closest genjet', range(0, 201, 5)),
    ('closestdr', u'ΔR of closest genjet', np.array(range(41)) * 0.05 * math.pi),
    ('closestparticlept', 'pt of closest particle', range(0, 201, 5)),
    ('closestparticledr', u'ΔR of closest particle', np.array(range(41)) * 0.05 * math.pi)
]

for name, title, nbins in pspace:
    print 'Processing', name
    fig = plt.figure()
    plt.title(title)
    plt.hist([looseparticles['genparticle_' + name]], bins=nbins, normed=1, alpha=0.2, label=['particles'])
    plt.hist([particles['genparticle_' + name]], bins=nbins, normed=1, alpha=0.5, label=['selected particles'])
    plt.hist([selection['lepton_genparticle_' + name]], bins=nbins, normed=1, alpha=0.7, label=[u'matched particles'])
    # n, bins, patches = plt.hist([particles['genjet_' + name], selection['lepton_genjet_' + name]], bins=nbins, normed=1)
    # plt.legend(patches, labels)
    plt.legend()
    plt.savefig('lepton_{}.png'.format(name))
