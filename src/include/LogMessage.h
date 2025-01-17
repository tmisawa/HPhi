/* HPhi  -  Quantum Lattice Model Simulator */
/* Copyright (C) 2015 Takahiro Misawa, Kazuyoshi Yoshimi, Mitsuaki Kawamura, Youhei Yamaji, Synge Todo, Naoki Kawashima */

/* This program is free software: you can redistribute it and/or modify */
/* it under the terms of the GNU General Public License as published by */
/* the Free Software Foundation, either version 3 of the License, or */
/* (at your option) any later version. */

/* This program is distributed in the hope that it will be useful, */
/* but WITHOUT ANY WARRANTY; without even the implied warranty of */
/* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the */
/* GNU General Public License for more details. */

/* You should have received a copy of the GNU General Public License */
/* along with this program.  If not, see <http://www.gnu.org/licenses/>. */
#pragma once

//sz.c
const char* cStateLocSpin= "j = %d loc %d \n";
const char* cInitalSz=  "initial sz : %s";
const char* cOMPSzStart=  "omp parallel sz finishes: %s";
const char* cOMPSzMid= "mid omp parallel sz : %s";
const char* cOMPSzFinish=  "omp parallel sz finishes: %s";
const char* cSingleSzStart="single sz starts: %s";
const char* cSingleSzFinish= "single sz finishes: %s";
const char* cReadSzStart ="READ=1: read starts: %s";
const char* cReadSzEnd  ="READ=1: read finishes: %s";

//CG_EigenVector.c
const char* cLogCG_EigenVecStart="Start: Calculate EigenVector by CG method.\n";
const char* cLogCG_EigenVecEnd="End: Calculate EigenVector by CG method.\n";
const char* cCG_EigenVecFinish="CG Eigenvector finishes: %s";

//diagonalcalc.c
const char* cDiagonalCalcFinish="diagonal calculation finishes: %s";


//FirstMultiply.c, Multiply.c
const char* cTPQStep="step %d:TPQ begins: %s";
const char* cTPQStepEnd="step %d:TPQ ends: %s";

//Lanczos_EigenValue.c
const char* cLogLanczos_EigenValueStart="Start: Calculate Lanczos Eigenvalue.\n";
const char* cLogLanczos_EigenValueNotConverged="Lanczos Eigenvalue is not converged in this process.";
const char* cLogLanczos_EigenValueEnd="End  : Calculate Lanczos EigenValue.\n";
const char* cLanczos_EigenValueStart="Lanczos Eigen Value start: %s";
const char* cLanczos_EigenValueStep="%3d th Lanczos step: %s";
const char* cLanczos_EigenValueFinish= "Lanczos Eigenvalue finishes: %s";

//Lanczos_EigenVector.c
const char* cLogLanczos_EigenVectorStart="Start: Calculate Lanczos Eigenvector.\n";
const char* cLogLanczos_EigenVectorEnd="End: Calculate Lanczos Eigenvector.\n";
const char* cLanczos_EigenVectorFinish="Lanczos Eigenvector finishes: %s";

//expec.c
const char* cExpecStart="step %d:Expec begins: %s";
const char* cExpecEnd="step %d:Expec ends: %s";

//expec_cisajs.c
const char* cLogLanczosExpecOneBodyGStart="Start: Calculate one body Green functions by Lanczos method.\n";
const char* cLogCGExpecOneBodyGStart="Start: Calculate one body Green functions by CG method.\n";
const char* cLogLanczosExpecOneBodyGEnd="End: Calculate one body Green functions by Lanczos method.\n";
const char* cLogCGExpecOneBodyGEnd="End: Calculate one body Green functions by CG method.\n";

const char* cLanczosExpecOneBodyGFinish ="Lanczos expec_cisajs finishes: %s";
const char* cTPQExpecOneBodyGStart = "set %d step %d:expec_cisajs begins: %s";
const char* cTPQExpecOneBodyGFinish = "set %d step %d:expec_cisajs finishes: %s";
const char* cExpecOneBodyGFinish= "CG expec_cisajs finishes: %s";

//expec_cisajucktaltdc.c
const char*  cLogLanczosExpecTwoBodyGFinish= "End  : Calculate two bodies Green functions by Lanczos method.\n";
const char*  cLogCGExpecTwoBodyGFinish= "End  : Calculate two bodies Green functions by CG method.\n";
const char*  cLanczosExpecTwoBodyGFinish= "Lanczos expec_cisajacktalt finishes: %s";
const char*  cCGExpecTwoBodyGFinish= "CG expec_cisajacktalt begins: %s";
const char*  cTPQExpecTwoBodyGStart = "set %d step %d:expec_cisajscktaltdc finishes: %s";
const char*  cTPQExpecTwoBodyGFinish = "set %d step %d:expec_cisajscktaltdc finishes: %s";

//expec_energy.c
const char* cLogExpecEnergyStart="Start: Calculate Energy.\n";
const char* cLogLanczosExpecEnergyEnd="End  : Calculate Energy by Lanczos method.\n";
const char* cLogCGExpecEnergyEnd="End  : Calculate Energy by CG method.\n";
const char* cLogEnergy="energy=%lf \n";
const char* cLanczosExpecEnergyEnd= "Lanczos expec energy finishes: %s";
const char* cCGExpecEnergyEnd= "CG expec energy finishes: %s";
