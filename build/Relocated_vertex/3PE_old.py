import ROOT
import math
from array import array

ROOT.gStyle.SetOptStat(0)

# ======================================================
# Helpers
# ======================================================
def make_graph(x, y, ey):
    ex = [0.0]*len(x)
    return ROOT.TGraphErrors(
        len(x),
        array('d', x),
        array('d', y),
        array('d', ex),
        array('d', ey)
    )

def get_common_ratio_graph(g1, g2):
    x, y, ex, ey = [], [], [], []

    for i in range(g1.GetN()):
        x1, y1 = ROOT.Double(), ROOT.Double()
        g1.GetPoint(i, x1, y1)
        e1 = g1.GetErrorY(i)

        for j in range(g2.GetN()):
            x2, y2 = ROOT.Double(), ROOT.Double()
            g2.GetPoint(j, x2, y2)

            if abs(x1 - x2) < 1e-6 and y2 != 0:
                e2 = g2.GetErrorY(j)
                r  = y1 / y2
                er = r * math.sqrt((e1/y1)**2 + (e2/y2)**2)

                x.append(float(x1))
                y.append(r)
                ex.append(0.0)
                ey.append(er)

    return ROOT.TGraphErrors(
        len(x),
        array('d', x),
        array('d', y),
        array('d', ex),
        array('d', ey)
    )

# ======================================================
# DATA (same as your macro)
# ======================================================
pressure_slide = [2,3,4,5,6,7,8,9,10,11,12,14,15,16]
slide = [9.70,11.27,15.08,23.04,24.79,28.00,28.43,32.58,
         35.11,41.68,40.76,46.05,44.48,45.95]
slide_err = [0.42,0.86,1.02,1.63,1.62,1.55,1.59,1.44,
             1.74,1.80,1.84,1.78,1.89,1.75]

pressure_excel = [3,4,5,6,7,8,9,10,11,12,14,15,16]
datasetA = [11.95,11.19,13.07,15.84,18.45,16.65,
            20.50,22.65,27.14,28.27,30.73,28.18,30.20]
NA = [1355,1224,2035,1863,840,799,1056,746,748,711,781,685,1129]
A_err = [100*math.sqrt((y/100)*(1-y/100)/n) for y,n in zip(datasetA,NA)]

pressure_sim = [2,3,4,5,6,7,8,9,10,11,12,13,14,15,16]
sim4PE = [5.137,9.3093,11.5942,14.4366,20.5479,26.8707,
          27.5261,31.5789,36.4621,33.9223,
          41.0909,40.5594,41.0169,37.50,46.595]
sim_total = [292,333,276,284,292,294,287,285,277,283,275,281,295,312,279]
sim_err = [100*math.sqrt((y/100)*(1-y/100)/n) for y,n in zip(sim4PE,sim_total)]

pressure_shubhi4 = pressure_slide
shubhi4 = [8.29,9.94,12.63,18.37,19.33,22.89,21.45,26.06,
           28.17,34.75,34.03,38.27,35.32,37.59]
shubhi4_err = [0.39,0.81,0.95,1.50,1.48,1.45,1.45,1.35,
               1.64,1.74,1.77,1.74,1.82,1.70]

pressure_sheet = pressure_slide
N4 = [6620,4064,1224,8141,3728,840,799,1056,746,748,711,781,685,2257]
N5 = [595,470,165,1353,739,267,259,393,304,246,321,399,336,1135]

eff_sheet = []
eff_sheet_err = []
for n5,n4 in zip(N5,N4):
    p = n5/n4
    eff_sheet.append(100*p)
    eff_sheet_err.append(100*math.sqrt(p*(1-p)/n4))
    
pressure_dec2025 = [4.25, 6, 7.25, 8, 9, 10, 11, 12]
dec2025_raw = [0.135, 0.159, 0.178, 0.163, 0.198,
               0.21, 0.234, 0.246]
dec2025_events = [467, 2082, 1971, 2795, 2750,
                  2078, 2061, 2912]

dec2025 = []
dec2025_err = []

for p, n in zip(dec2025_raw, dec2025_events):
    dec2025.append(100.0 * p)
    dec2025_err.append(100.0 * math.sqrt(p*(1-p)/n))
    

# ======================================================
# GRAPHS (ALL of them)
# ======================================================
graphs = [
    ("Shubhi (3.5PE)", make_graph(pressure_slide, slide, slide_err), ROOT.kBlack, 20),
    ("Old data (3.5PE)", make_graph(pressure_excel, datasetA, A_err), ROOT.kRed, 21),
    ("Simulation (4PE)", make_graph(pressure_sim, sim4PE, sim_err), ROOT.kGreen+2, 29),
    ("Shubhi (4PE)", make_graph(pressure_shubhi4, shubhi4, shubhi4_err), ROOT.kViolet+1, 33),
    ("Old data (3PE)", make_graph(pressure_sheet, eff_sheet, eff_sheet_err), ROOT.kBlue+2, 22),
   ("New data(3.5PE)", make_graph(pressure_dec2025, dec2025, dec2025_err), ROOT.kOrange+7, 34)
]

for name,g,c,m in graphs:
    g.SetMarkerStyle(m)
    g.SetMarkerColor(c)
    g.SetLineColor(c)
    g.SetLineWidth(2)

# ======================================================
# CANVAS + PADS
# ======================================================
c = ROOT.TCanvas("c","Efficiency + Ratio",1100,900)

pad1 = ROOT.TPad("pad1","main",0,0.30,1,1)
pad2 = ROOT.TPad("pad2","ratio",0,0.00,1,0.30)

pad1.SetBottomMargin(0.02)
pad2.SetTopMargin(0.02)
pad2.SetBottomMargin(0.30)

pad1.Draw()
pad2.Draw()

# ======================================================
# MAIN PAD (ALL datasets)
# ======================================================
pad1.cd()

graphs[0][1].SetTitle("Efficiency vs Pressure")
graphs[0][1].GetYaxis().SetTitle("Efficiency (%)")
graphs[0][1].GetYaxis().SetRangeUser(0,60)
graphs[0][1].GetXaxis().SetLabelSize(0)

graphs[0][1].Draw("ALP")
for _,g,_,_ in graphs[1:]:
    g.Draw("LP SAME")

leg = ROOT.TLegend(0.12,0.65,0.45,0.88)
leg.SetBorderSize(1)
leg.SetFillStyle(1001)
for name,g,_,_ in graphs:
    leg.AddEntry(g, name, "lp")
leg.Draw()

pad1.SetGrid()

# ======================================================
# RATIO PAD (ONLY ONE RATIO)
# ======================================================
pad2.cd()

g_ratio = get_common_ratio_graph(
    graphs[0][1],   # Shubhi 3.5PE
    graphs[4][1]    # Old data 3PE
)

g_ratio.SetTitle("")
g_ratio.GetYaxis().SetTitle("Ratio")
g_ratio.GetXaxis().SetTitle("Pressure (atm)")
g_ratio.GetYaxis().SetRangeUser(0.6,1.4)

g_ratio.GetYaxis().SetTitleSize(0.10)
g_ratio.GetYaxis().SetLabelSize(0.09)
g_ratio.GetYaxis().SetTitleOffset(0.45)
g_ratio.GetXaxis().SetTitleSize(0.12)
g_ratio.GetXaxis().SetLabelSize(0.10)

g_ratio.SetMarkerStyle(20)
g_ratio.SetMarkerColor(ROOT.kBlue+2)
g_ratio.SetLineColor(ROOT.kBlue+2)
g_ratio.SetLineWidth(2)

g_ratio.Draw("ALP")

xmin = g_ratio.GetXaxis().GetXmin()
xmax = g_ratio.GetXaxis().GetXmax()

unity = ROOT.TLine(xmin,1.0,xmax,1.0)
unity.SetLineStyle(2)
unity.SetLineColor(ROOT.kGray+2)
unity.Draw()

pad2.SetGrid()

c.Update()

