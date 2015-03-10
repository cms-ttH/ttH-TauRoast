import ROOT as r

class Legend:
    def __init__(self, margin, ncols, topmargin=0):
        self.__current = 0
        self.__box_dx = 0.06
        self.__box_dy = 0.04
        self.__legend_x = margin
        self.__legend_dx = (1. - 2 * margin) / ncols
        self.__legend_y = 1. - 0.5 * margin - topmargin
        self.__legend_dy = 1.4 * self.__box_dy
        self.__pos_x = self.__legend_x
        self.__pos_y = self.__legend_y

        self.__ncols = ncols

        self.__markers = []
        self.__paves = []
        self.__tex = r.TLatex()
        self.__tex.SetNDC()
        self.__tex.SetTextSize(0.035)
        self.__line = r.TLine()

    def _to_ndc(self, x, y):
        new_x = r.gPad.GetLeftMargin() + x * (1 - r.gPad.GetLeftMargin() - r.gPad.GetRightMargin())
        new_y = r.gPad.GetBottomMargin() + y * (1 - r.gPad.GetBottomMargin() - r.gPad.GetTopMargin())
        return (new_x, new_y)

    def advance(self):
        self.__current += 1
        self.__pos_x += self.__legend_dx
        if self.__current % self.__ncols == 0:
            self.__pos_x = self.__legend_x
            self.__pos_y -= self.__legend_dy

    def new_row(self):
        if self.__current % self.__ncols == 0:
            return
        self.__current = 0
        self.__pos_x = self.__legend_x
        self.__pos_y -= self.__legend_dy

    def draw_box(self, pattern, color, label, line=False):
        (x1, y1) = self._to_ndc(self.__pos_x, self.__pos_y)
        (x2, y2) = self._to_ndc(self.__pos_x + self.__box_dx, self.__pos_y - self.__box_dy)
        pave = r.TPave(x1, y1, x2, y2, 0, "NDC")
        pave.SetFillStyle(pattern)
        pave.SetFillColor(color)
        pave.SetBorderSize(0 if not line else 1)
        pave.Draw()
        self.__paves.append(pave)

        (text_x, text_y) = self._to_ndc(
                    self.__pos_x + 1.2 * self.__box_dx,
                    self.__pos_y - 0.8 * self.__box_dy)
        self.__tex.DrawLatex(text_x, text_y, label)
        self.advance()

    def draw_marker(self, style, color, label):
        self.__line.SetLineColor(color)
        self.__line.SetLineWidth(1)
        self.__line.DrawLineNDC(
                *(self._to_ndc(
                    self.__pos_x,
                    self.__pos_y - self.__box_dy / 2)
                + self._to_ndc(
                    self.__pos_x + self.__box_dx,
                    self.__pos_y - self.__box_dy / 2)))
        self.__line.DrawLineNDC(
                *(self._to_ndc(
                    self.__pos_x + self.__box_dx / 2,
                    self.__pos_y)
                + self._to_ndc(
                    self.__pos_x + self.__box_dx / 2,
                    self.__pos_y - self.__box_dy)))
        (marker_x, marker_y) = self._to_ndc(self.__pos_x + self.__box_dx / 2, self.__pos_y - self.__box_dy / 2)
        marker = r.TMarker(marker_x, marker_y, style)
        marker.SetMarkerStyle(style)
        marker.SetMarkerColor(color)
        marker.SetNDC()
        marker.Draw()
        self.__markers.append(marker)
        (text_x, text_y) = self._to_ndc(
                    self.__pos_x + 1.2 * self.__box_dx,
                    self.__pos_y - 0.8 * self.__box_dy)
        self.__tex.DrawLatex(text_x, text_y, label)
        self.advance()

    def draw_line(self, width, color, label):
        self.__line.SetLineColor(color)
        self.__line.SetLineWidth(width)
        self.__line.DrawLineNDC(
                *(self._to_ndc(
                    self.__pos_x,
                    self.__pos_y - self.__box_dy / 2)
                + self._to_ndc(
                    self.__pos_x + self.__box_dx,
                    self.__pos_y - self.__box_dy / 2)))
        (text_x, text_y) = self._to_ndc(
                    self.__pos_x + 1.2 * self.__box_dx,
                    self.__pos_y - 0.8 * self.__box_dy)
        self.__tex.DrawLatex(text_x, text_y, label)
        self.advance()
