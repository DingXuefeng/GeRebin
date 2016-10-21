#!/bin/bash
echo "Bkg:"
./GeRebin bgfs_nov_2012_gempi4.spe bg_1keV.dat
echo "V50:"
./GeRebin v_pur_final_160201_gempi4.spe v50_1keV.dat
