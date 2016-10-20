#!/bin/bash
echo "Bkg:"
./process bgfs_nov_2012_gempi4.spe bg_1keV.dat 3000
echo "V50:"
./process v_pur_final_160201_gempi4.spe v50_1keV.dat 3000
