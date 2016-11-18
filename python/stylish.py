# vim: ts=4:sw=4:et:sta
import ROOT as r

ratio_plot_max = 2.3
small_number = 1e-5
y_divide = 0.3


def draw_channel_info(config, plot_ratio, is2d=False, proc=None):
    """Draws channel information based on cuts and standard CMS labelling and
    lumi information.  The parameter `plot_ratio` determines the text size."""
    if plot_ratio:
        scale = 1
    else:
        scale = .5 / .7

    if config['analysis']['channel'] == 'll':
        channel_label = "Dilepton + "
    elif config['analysis']['channel'] == 'tl':
        channel_label = "Lep + #tau_{h} + "
    elif config['analysis']['channel'] == 'ttl':
        channel_label = "Lep + #tau_{h}#tau_{h} + "
    elif config['analysis']['channel'] == 'tll':
        channel_label = "Dilepton + #tau_{h} + "

    tex = r.TLatex()
    tex.SetNDC()
    tex.SetTextFont(42)
    tex.SetTextSize(0.05 * scale)
    tex.SetTextAlign(31)
    tex.DrawLatex(.99 - r.gPad.GetRightMargin(), .84,
                  channel_label + "jets")
    tex.SetTextSize(0.055 * scale)
    tex.SetTextAlign(11)
    if is2d and proc:
        tex.DrawLatex(0.14, 0.84, proc)
    tex.DrawLatex(0.14, 0.91, "CMS Preliminary")
    # tex.DrawLatex(0.14, 0.91, "CMS ttH")
    # tex.DrawLatex(0.30, 0.92, "#tau_{h}#tau_{h}")
    tex.SetTextAlign(31)
    tex.DrawLatex(1 - r.gPad.GetRightMargin(), 0.91, "#sqrt{s} = 8 TeV, L = 19.5 fb^{-1}")


def setup_upper_axis(histo, scale=True, split=True, is2d=False):
    if scale:
        histo.Scale(0)
    histo.SetTitle("")

    histo.GetXaxis().SetLabelFont(62)
    histo.GetXaxis().SetTitleFont(62)

    histo.GetYaxis().SetTitleFont(62)
    histo.GetYaxis().SetTitleSize(.05)
    histo.GetYaxis().SetTitleOffset(1.1)
    histo.GetYaxis().SetLabelFont(62)
    histo.GetYaxis().SetLabelSize(.04)

    if not split:
        histo.GetXaxis().SetTitleSize(0.05)
        histo.GetXaxis().SetTitleOffset(1.1)
        histo.GetXaxis().SetLabelSize(0.045)
        # histo.GetXaxis().SetLabelOffset(0.02)

    if is2d:
        histo.GetYaxis().SetTitleOffset(1.5)
        histo.GetYaxis().SetTitleSize(0.04)
        histo.GetYaxis().SetLabelFont(62)
        histo.GetYaxis().SetLabelSize(.04)

        histo.GetZaxis().SetTitleFont(62)
        histo.GetYaxis().SetTitleOffset(1.6)
        histo.GetZaxis().SetTitleSize(0.04)
        histo.GetZaxis().SetLabelFont(62)
        histo.GetZaxis().SetLabelSize(0.03)


def setup_lower_axis(histo):
    histo.GetXaxis().SetTitleFont(62)
    histo.GetXaxis().SetTitleSize(0.14)
    histo.GetXaxis().SetTitleOffset(0.9)
    histo.GetXaxis().SetLabelFont(62)
    histo.GetXaxis().SetLabelSize(0.12)
    histo.GetXaxis().SetLabelOffset(0.02)

    histo.GetYaxis().SetTitleFont(62)
    histo.GetYaxis().SetTitle("Data/MC")
    histo.GetYaxis().CenterTitle()
    histo.GetYaxis().SetTitleSize(0.1)
    histo.GetYaxis().SetTitleOffset(0.45)
    histo.GetYaxis().SetLabelFont(62)
    histo.GetYaxis().SetLabelSize(0.1)
    histo.GetYaxis().SetRangeUser(0, ratio_plot_max)
    histo.GetYaxis().SetNdivisions(505)


def setup_pad(pad):
    pad.SetPad(small_number, small_number, 1 - small_number, 1 - small_number)
    pad.SetTopMargin(0.1)
    pad.SetBottomMargin(0.14)
    pad.SetLeftMargin(0.14)
    pad.SetRightMargin(0.05)
    pad.SetTicks(0, 0)


def setup_upper_pad(pad):
    pad.SetPad(small_number, y_divide + small_number, 1 - small_number, 1 - small_number)
    pad.SetTopMargin(0.1)
    pad.SetBottomMargin(small_number)
    pad.SetLeftMargin(0.11)
    pad.SetRightMargin(0.05)
    pad.SetTicks(0, 0)


def setup_lower_pad(pad):
    pad.SetPad(small_number, small_number, 1 - small_number, y_divide - small_number)
    pad.SetBottomMargin(0.3)
    pad.SetLeftMargin(0.11)
    pad.SetRightMargin(0.05)
    pad.SetTopMargin(small_number)
    pad.SetTicks(1, 0)


def setup():
    r.gStyle.Reset()
    r.gStyle.SetCanvasBorderMode(0)
    r.gStyle.SetCanvasColor(r.kWhite)

    r.gStyle.SetPadBorderMode(0)
    r.gStyle.SetPadColor(r.kWhite)

    r.gStyle.SetFrameBorderMode(0)
    r.gStyle.SetFrameBorderSize(1)
    r.gStyle.SetFrameFillColor(0)
    r.gStyle.SetFrameFillStyle(0)
    r.gStyle.SetFrameLineColor(1)
    r.gStyle.SetFrameLineStyle(1)
    r.gStyle.SetFrameLineWidth(1)

    r.gStyle.SetPalette(1)

    r.gStyle.SetOptStat(0)
