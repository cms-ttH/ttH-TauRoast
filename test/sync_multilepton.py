#!/usr/bin/env python

import os

from root_numpy import root2array

stuff = root2array("test/sync_multilepton/out/ntuple.root", "ttH")
print stuff.dtype.names


def cols(names):
    columns = []
    for name in names:
        try:
            columns.append(stuff.dtype.names.index(name))
        except ValueError:
            print "Can't find", name
    return columns


def get(names):
    selection = stuff[stuff[names[1]] > -999.]
    columns = cols(names)
    for row in selection:
        yield [row[i] for i in columns]


stuffs = {
    'mu': [
        'pt', 'eta', 'phi', 'E', 'pdgID', 'charge',
        'jetNDauChargedMVASel', 'miniRelIso', 'miniIsoCharged', 'miniIsoNeutral',
        'jetPtRel', 'jetCSV', 'jetPtRatio',
        'sip3D', 'dxy', 'dz', 'segmentCompatibility', 'leptonMVA'
    ],
    'ele': [
        'pt', 'eta', 'phi', 'E', 'pdgID', 'charge',
        'jetNDauChargedMVASel', 'miniRelIso', 'miniIsoCharged', 'miniIsoNeutral',
        'jetPtRel', 'jetCSV', 'jetPtRatio',
        'sip3D', 'dxy', 'dz', 'eleMVA', 'leptonMVA'
    ],
    'tau': [
        'pt', 'eta', 'phi', 'E', 'charge',
        'dxy', 'dz',
        'decayModeFinding', 'byMediumIsolationMVArun2v1DBdR03oldDMwLT'
    ],
    'jet': [
        'pt', 'eta', 'phi', 'E',
        'CSV',
    ],
}

for k, vs in stuffs.items():
    add = ['PFMET', 'PFMETphi'] if k == 'jet' else []
    with open(os.path.join('test/sync_multilepton/sync/me', k + '.txt'), 'w') as fd:
        for row in get(['nEvent'] + ["0_".join([k, v]) for v in vs] + add):
            fd.write(" ".join(map(str, row)) + "\n")
