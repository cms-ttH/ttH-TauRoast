# vim: ts=4:sw=4:et:sta
# coding: utf-8
from pyparsing import *
import ROOT as r

r.gROOT.SetBatch()
r.gSystem.Load("libttHTauRoast")

try:
    from ROOT import roast
except:
    sys.stderr.write("Failed to import 'roast'!\n")
    sys.exit(1)

class ComposedCut(object):
    def __init__(self, cut1, cut2, or_cuts):
        self._negate = False
        self._cut1 = cut1
        self._cut2 = cut2
        self._or = or_cuts

    def __repr__(self):
        rpr = "{0} {1} {2}".format(
                repr(self._cut1),
                '||' if self._or else '&&',
                repr(self._cut1))
        if self._negate:
            return "<!({0})>".format(rpr)
        return "<{0}>".format(rpr)

    def __str__(self):
        rpr = "({0}) {1} ({2})".format(
                self._cut1,
                '||' if self._or else '&&',
                self._cut2)
        return rpr

    def negate(self):
        self._negate = not self._negate

    def roastify(self):
        return roast.CutFlow.ComposedCut(
                str(self),
                self._cut1.roastify(),
                self._cut2.roastify(),
                roast.CutFlow.ComposedCut.kOr if self._or else roast.CutFlow.ComposedCut.kAnd,
                self._negate)

class ValueCut(object):
    epsilon = 1e-5

    def __init__(self, loc, tokens):
        if len(tokens) == 5:
            self._min = float(tokens[0])
            tokens = tokens[2:]
        else:
            self._min = None
        (self._name, self._op, self._max) = tokens

        self._max = float(self._max)
        self._negate = False

        if self._op in ['=', '!=', u'≠']:
            self._min = self._max - ValueCut.epsilon
            self._max += ValueCut.epsilon

        if self._op in [u'≥', '>=', '!=', u'≠']:
            self._min, self._max = self._max, self._min

    def __repr__(self):
        return "<{0}: {3}({1} ≤, ≤ {2})>".format(
                self._name,
                self._min,
                self._max,
                '!' if self._negate else '')

    def __str__(self):
        s = self._name
        if self._min:
            s = str(self._min) + " <= " + s
        if self._max:
            s += " <= " + str(self._max)
        if self._negate:
            s = "!(" + s + ")"
        return s

    def negate(self):
        self._negate = not self._negate

    def roastify(self):
        name = self._name.decode('utf-8').encode('ascii', 'xmlcharrefreplace')
        return roast.CutFlow.ValueCut(
                name,
                self._min if self._min else float("-inf"),
                self._max if self._max else float("inf"),
                self._negate)

class Parser(object):
    def __init__(self):
        chars = alphanums + '_'

        var = Word(chars)
        num = Combine(Optional(Literal("-")) + Word(nums) + Optional(Literal(".") + Word(nums)))

        ge = Literal(u"≥") | Literal(">=")
        le = Literal(u"≤") | Literal("<=")
        eq = Literal("=")
        neq = Literal(u"≠") | Literal("!=")

        inclusive_range = Optional(num + le) + var + le + num
        exclusive_range = Optional(num + ge) + var + ge + num
        eq_value = var + (eq ^ neq) + num

        inclusive_range.setParseAction(ValueCut)
        exclusive_range.setParseAction(ValueCut)
        eq_value.setParseAction(ValueCut)

        term = inclusive_range ^ exclusive_range ^ eq_value

        self._expr = infixNotation(term, [
            (Keyword("not"), 1, opAssoc.RIGHT, self.__neg),
            (Keyword("and"), 2, opAssoc.LEFT, self.__chain),
            (Keyword("or"), 2, opAssoc.LEFT, self.__chain)])

        # tests = [
                # "75 <= bar <= 105",
                # "2.1 <= eta <= 2.1",
                # "foo >= 4.1 and mode = 1",
                # "foo >= 4.1 and not mode = 1",
                # "2.1 ≥ foo >= 4.1 and mode ≠ 1",
                # "not 2.1 <= foo <= 4.1 and mode ≠ 1"
                # ]

        # for t in tests:
            # print t, "→", expr.parseString(t)

    def __chain(self, tokens):
        cut1, op, cut2 = tokens[0]
        del tokens[0]
        return ComposedCut(cut1, cut2, op == 'or')

    def __neg(self, tokens):
        del tokens[0][0]
        tokens[0][0].negate()
        return tokens[0]

    def process(self, s):
        return self._expr.parseString(s)[0].roastify()

if __name__ == '__main__':
    import doctest
    doctest.testmod()
