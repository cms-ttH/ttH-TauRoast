def btag(jet):
    return jet.csv() > 0.679

class Snippet(object):
    def __init__(self, code):
        self.__code = code

    def _execute(self, event, combo, locals=None, globals=None, use_exec=True):
        mylocals = {}
        myglobals = {
                'event': event,
                'btag': btag,
                'taus': combo.taus(),
                'leptons': combo.leptons(),
                'jets': combo.jets()
        }

        if locals:
            mylocals.update(locals)
        if globals:
            myglobals.update(globals)
        if use_exec:
            exec self.__code in mylocals, myglobals
        else:
            return eval(self.__code, myglobals, mylocals)
