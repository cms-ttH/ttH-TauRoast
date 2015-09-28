import math

from collections import namedtuple

Config = namedtuple('Config', ['taus', 'leptons'])
config = None

def channel(s):
    global config
    config = Config(taus=s.lower().count("t"), leptons=s.lower().count("l"))

def dR(one, two):
    diff = one.p4() - two.p4()
    return math.sqrt(diff.Eta()**2 + diff.Phi()**2)

def btag(jet):
    return jet.csv() > 0.814

__mva = None
def mva():
    global __mva
    if __mva is None:
        from variation import principal
        __mva = principal
    return __mva.evaluate()

class Snippet(object):
    def __init__(self, code):
        self.__code = code

    def _execute(self, event, combo, locals=None, globals=None, use_exec=True):
        if not locals:
            locals = {}
        if not globals:
            globals = {}
            Snippet.prepare_globals(globals, event, combo)
        if use_exec:
            exec self.__code in locals, globals
            if 'result' in globals:
                return globals['result']
        else:
            return eval(self.__code, globals, locals)

    @classmethod
    def prepare_globals(cls, globals, event, combo, tagging=False):
        globals.update({
                'event': event,
                'btag': btag,
                'dR': dR,
                'mva': mva,
                'taus': combo.taus(),
                'leptons': combo.leptons(),
                'jets': combo.jets(),
                'tags': filter(btag, combo.jets()) if tagging else [],
                'notags': [j for j in combo.jets() if not btag(j)] if tagging else [],
                'pv': event.pv(),
                'met': event.met()
        })
