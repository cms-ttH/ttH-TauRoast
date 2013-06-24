import ROOT as r

get_signals = lambda ps: filter(lambda p: p.IsSignal(), ps)
get_backgrounds = lambda ps: filter(lambda p: p.IsBackground(), ps)

def get_collisions(ps):
    res = filter(lambda p: p.IsCollisions(), ps)
    if len(res) != 1:
        # FIXME turn into real exception
        raise "Not one collision process found!"
    return res[0]

def vectorize(items, cls):
    """Create a vector of class `cls` and fill it with the object in the
    list `items`."""
    res = r.std.vector(cls)()
    for i in items:
        res.push_back(i)
    return res

