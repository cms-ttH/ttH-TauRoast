#!/usr/bin/env python

from root_pandas import read_root

tree = read_root("test/genfaketau/out/tmva.root", "TestTree")
signal = tree[tree.classID < .5]
background = tree[tree.classID > .5]

for i in [.05 * x for x in range(20)]:
    cut = background.BDTG.quantile(i)
    sfraction = signal[signal.BDTG > cut].size / float(signal.size)
    bfraction = background[background.BDTG < cut].size / float(background.size)
    print "{:4.2f} {:6.4f} {:6.4f} {:6.4f}".format(i, cut, sfraction, bfraction)
