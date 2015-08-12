import sys

def read(fn):
    with open(fn) as f:
        for l in f.readlines():
            try:
                yield int(l.split(',', 1)[0])
            except ValueError:
                pass

one, two = map(set, map(list, map(read, sys.argv[1:])))

only = one - two
twly = two - one

print len(only), only
print len(twly), twly

with open(sys.argv[1]) as f:
    for l in f.readlines():
        vals = l.split(',')
        event, pdg = map(int, vals[0:2])
        pt, eta, phi, dxy, dz, reliso, sip3d, pmva = map(float, vals[2:])
        if event in only:
            print '---'
            print l.strip()
            if abs(pdg) == 11:
                print pt > 7 and abs(eta) < 2.4 and abs(dxy) < 0.05 and abs(dz) < 0.2 and reliso < 0.5
            if abs(pdg) == 13:
                print pt > 5 and abs(eta) < 2.4 and abs(dxy) < 0.05 and abs(dz) < 0.2 and reliso < 0.5
