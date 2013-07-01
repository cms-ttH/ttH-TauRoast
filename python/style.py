import ROOT as r

def create_tdr_style():
    style = r.TStyle("tdrStyle", "Style for P-TDR")
    # For the canvas:
    style.SetCanvasBorderMode(0)
    style.SetCanvasColor(r.kWhite)
    style.SetCanvasDefH(600)  #Height of canvas
    style.SetCanvasDefW(600)  #Width of canvas
    style.SetCanvasDefX(0)    #POsition on screen
    style.SetCanvasDefY(0)

    # For the Pad:
    style.SetPadBorderMode(0)
    # style.SetPadBorderSize(Width_t size = 1)
    style.SetPadColor(r.kWhite)
    style.SetPadGridX(False)
    style.SetPadGridY(False)
    style.SetGridColor(0)
    style.SetGridStyle(3)
    style.SetGridWidth(1)

    # For the frame:
    style.SetFrameBorderMode(0)
    style.SetFrameBorderSize(1)
    style.SetFrameFillColor(0)
    style.SetFrameFillStyle(0)
    style.SetFrameLineColor(1)
    style.SetFrameLineStyle(1)
    style.SetFrameLineWidth(1)

    # For the histo:
    # style.SetHistFillColor(1)
    # style.SetHistFillStyle(0)
    #style.SetHistLineColor(1)
    #style.SetHistLineStyle(0)
    #style.SetHistLineWidth(1)
    # style.SetLegoInnerR(Float_t rad = 0.5)
    # style.SetNumberContours(Int_t number = 20)

    #style.SetEndErrorSize(2)
    #style.SetErrorMarker(20);  # Seems to give an error
    #style.SetErrorX(0.)

    style.SetMarkerStyle(20)

    #For the fit/function:
    style.SetOptFit(1)
    style.SetFitFormat("5.4g")
    style.SetFuncColor(2)
    style.SetFuncStyle(1)
    style.SetFuncWidth(1)

    #For the date:
    style.SetOptDate(0)
    # style.SetDateX(Float_t x = 0.01)
    # style.SetDateY(Float_t y = 0.01)

    # For the statistics box:
    style.SetOptFile(0)
    style.SetOptStat(0); # To display the mean and RMS:   SetOptStat("mr")
    style.SetStatColor(r.kWhite)
    style.SetStatFont(42)
    style.SetStatFontSize(0.025)
    style.SetStatTextColor(1)
    style.SetStatFormat("6.4g")
    style.SetStatBorderSize(1)
    style.SetStatH(0.1)
    style.SetStatW(0.15)
    # style.SetStatStyle(Style_t style = 1001)
    # style.SetStatX(Float_t x = 0)
    # style.SetStatY(Float_t y = 0)

    # Margins:
    style.SetPadTopMargin(0.05)
    style.SetPadBottomMargin(0.13)
    style.SetPadLeftMargin(0.16)
    style.SetPadRightMargin(0.02)

    # For the Global title:
    style.SetOptTitle(0);    # 0=No Title
    style.SetTitleFont(42)
    style.SetTitleColor(1)
    style.SetTitleTextColor(1)
    style.SetTitleFillColor(10)
    style.SetTitleFontSize(0.05)
    # style.SetTitleH(0); # Set the height of the title box
    # style.SetTitleW(0); # Set the width of the title box
    # style.SetTitleX(0); # Set the position of the title box
    # style.SetTitleY(0.985); # Set the position of the title box
    # style.SetTitleStyle(Style_t style = 1001)
    # style.SetTitleBorderSize(2)

    # For the axis titles:
    style.SetTitleColor(1, "XYZ")
    style.SetTitleFont(42, "XYZ")
    style.SetTitleSize(0.06, "XYZ")
    # style.SetTitleXSize(Float_t size = 0.02); # Another way to set the size?
    # style.SetTitleYSize(Float_t size = 0.02)
    style.SetTitleXOffset(0.9)
    style.SetTitleYOffset(1.25)
    # style.SetTitleOffset(1.1, "Y"); # Another way to set the Offset

    # For the axis labels:
    style.SetLabelColor(1, "XYZ")
    style.SetLabelFont(42, "XYZ")
    style.SetLabelOffset(0.007, "XYZ")
    style.SetLabelSize(0.05, "XY")
    style.SetLabelSize(0.5, "Z")

    # For the axis:
    style.SetAxisColor(1, "XYZ")
    style.SetStripDecimals(r.kTRUE)
    style.SetTickLength(0.03, "XYZ")
    style.SetNdivisions(510, "XYZ")
    style.SetPadTickX(0);  # 0=Text labels (and tics) only on bottom, 1=Text labels on top and bottom
    style.SetPadTickY(1)

    # Change for log plots:
    style.SetOptLogx(0)
    style.SetOptLogy(0)
    style.SetOptLogz(0)

    # Postscript options:
    style.SetPaperSize(20.,20.)
    # style.SetLineScalePS(Float_t scale = 3)
    # style.SetLineStyleString(Int_t i, const char* text)
    # style.SetHeaderPS(const char* header)
    # style.SetTitlePS(const char* pstitle)

    # style.SetBarOffset(Float_t baroff = 0.5)
    # style.SetBarWidth(Float_t barwidth = 0.5)
    # style.SetPaintTextFormat(const char* format = "g")
    # style.SetPalette(Int_t ncolors = 0, Int_t* colors = 0)
    # style.SetTimeOffset(Double_t toffset)
    # style.SetHistMinimumZero(r.kTRUE)

    #gROOT->ForceStyle();  # Try this if stuff doesn't work right

    style.SetPalette(1)

    return style
