#!/bin/sh

set -e

max=80000

cmsRun test/validate.py maxEvents=$max outputFile=validate_tth.root inputFiles_load=test/validate_tth.txt
python test/validate_plot_tau.py validate_tth_numEvent${max}.root validate_tth.pdf

cmsRun test/validate.py maxEvents=$max outputFile=validate_vbf.root inputFiles_load=test/validate_vbf.txt
python test/validate_plot_tau.py validate_vbf_numEvent${max}.root validate_vbf.pdf

cp validate*pdf ~/www/ttH
