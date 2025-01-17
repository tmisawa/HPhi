/*
HPhi  -  Quantum Lattice Model Simulator
Copyright (C) 2015 Takahiro Misawa, Kazuyoshi Yoshimi, Mitsuaki Kawamura, Youhei Yamaji, Synge Todo, Naoki Kawashima

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <stdio.h>
#include <stdlib.h>
#include "StdFace_vals.h"

/**
 *
 * Add transfer to the list
 *
 * @author Mitsuaki Kawamura (The University of Tokyo)
 */
void StdFace_trans(
  int *ktrans /**< [inout] The counter for the transfer*/, 
  double trans0 /**< [in] Hopping integral t, mu, etc. */,
  int isite /**< [in] i for c_{i sigma}^dagger*/, 
  int ispin /**< [in] sigma for c_{i sigma}^dagger*/,
  int jsite /**< [in] j for c_{j sigma'}*/,
  int jspin /**< [in] sigma' for c_{j sigma'}*/)
{
  trans[*ktrans] = trans0;
  transindx[*ktrans][0] = isite; transindx[*ktrans][1] = ispin;
  transindx[*ktrans][2] = jsite; transindx[*ktrans][3] = jspin;
  *ktrans = *ktrans + 1;
}

/**
 *
 * Add interaction to the list
 *
 * @author Mitsuaki Kawamura (The University of Tokyo)
 */
void StdFace_intr(
  int *kintr /**< [inout] The counter for the interaction*/,
  double intr0 /**< [in] Interaction U, V, J, etc.*/,
  int site1 /**< [in] i1 for c_{i1 sigma1}^dagger*/,
  int spin1 /**< [in] sigma11 for c_{i1 sigma1}^dagger*/,
  int site2 /**< [in] i2 for c_{i2 sigma2}*/,
  int spin2 /**< [in] sigma12 for c_{i2 sigma2}*/,
  int site3 /**< [in] i3 for c_{i3 sigma3}^dagger*/,
  int spin3 /**< [in] sigma13 for c_{i3 sigma3}^dagger*/,
  int site4 /**< [in] i2 for c_{i2 sigma2}*/,
  int spin4 /**< [in] sigma12 for c_{i2 sigma2}*/)
{
  intr[*kintr] = intr0;
  intrindx[*kintr][0] = site1; intrindx[*kintr][1] = spin1;
  intrindx[*kintr][2] = site2; intrindx[*kintr][3] = spin2;
  intrindx[*kintr][4] = site3; intrindx[*kintr][5] = spin3;
  intrindx[*kintr][6] = site4; intrindx[*kintr][7] = spin4;
  *kintr = *kintr + 1;
}

/**
 *
 * Add Exchange term (S_i^+ S_j^- + S_i^- S_j^+) to the list
 *
 * @author Mitsuaki Kawamura (The University of Tokyo)
 */
void StdFace_exchange(
  int *kintr /**< [inout] The counter for the interaction*/,
  double Jexc /**< [in] Interaction J_{x y}, etc.*/,
  int isite /**< [in] i of S_i */, 
  int jsite /**< [in] j of S_j */)
{
  int ispin;
  for (ispin = 0; ispin < 2; ispin++){
    StdFace_intr(kintr, Jexc * 0.5,
      isite, ispin, isite, 1 - ispin, jsite, 1 - ispin, jsite, ispin);
  }
}

/**
 *
 * Add S^+ S^+ + S^-S^- term to the list
 *
 * @author Mitsuaki Kawamura (The University of Tokyo)
 */
void StdFace_Kitaev(
  int *kintr /**< [inout] The counter for the interaction*/,
  double Jflip /**< [in] Interaction J_x - J_y, etc.*/,
  int isite /**< [in] i of S_i */,
  int jsite /**< [in] j of S_j */)
{
  int ispin;
  for (ispin = 0; ispin < 2; ispin++){
    StdFace_intr(kintr, Jflip,
      isite, ispin, isite, 1 - ispin, jsite, ispin, jsite, 1 - ispin);
  }
}

/**
 *
 * Add S_z S_z term to the list
 *
 * @author Mitsuaki Kawamura (The University of Tokyo)
 */
void StdFace_SzSz(
  int *kintr /**< [inout] The counter for the interaction*/,
  double Jexc /**< [in] Interaction J_z, etc.*/,
  int isite /**< [in] i of S_i */,
  int jsite /**< [in] j of S_j */)
{
  int ispin, jspin;
  double intr0;
  for (ispin = 0; ispin < 2; ispin++){
    for (jspin = 0; jspin < 2; jspin++){
      if (ispin == jspin) intr0 = Jexc * 0.25;
      else intr0 = - Jexc * 0.25;
      StdFace_intr(kintr, intr0,
        isite, ispin, isite, ispin, jsite, jspin, jsite, jspin);
    }
  }
}

/**
 *
 * Add onsite/offsite Coulomb term to the list
 *
 * @author Mitsuaki Kawamura (The University of Tokyo)
 */
void StdFace_Coulomb(
  int *kintr /**< [inout] The counter for the interaction*/,
  double V /**< [in] Coulomb integral U, V, etc.*/,
  int isite /**< [in] i of n_i */,
  int jsite /**< [in] j of n_j */)
{
  int ispin, jspin;
  for (ispin = 0; ispin < 2; ispin++){
    for (jspin = 0; jspin < 2; jspin++){
      StdFace_intr(kintr, V, isite, ispin, isite, ispin, jsite, jspin, jsite, jspin);
    }
  }
}

/**
 *
 * Print a valiable (real) read from the input file
 *
 * @author Mitsuaki Kawamura (The University of Tokyo)
 */
void StdFace_PrintVal_d(
  char* valname /**< [in] Name of the valiable*/, 
  double *val /**< [inout] Valiable to be set*/, 
  double val0 /**< [in] The default value*/)
{
  if (*val == 9999.9) {
    *val = val0;
    printf("  %15s = %-10.5f  ######  DEFAULT VALUE IS USED  ######\n", valname, *val);
  }
  else printf("  %15s = %-10.5f\n", valname, *val);
}

/**
 *
 * Print a valiable (integer) read from the input file
 *
 * @author Mitsuaki Kawamura (The University of Tokyo)
 */
void StdFace_PrintVal_i(
  char* valname /**< [in] Name of the valiable*/,
  int *val /**< [inout] Valiable to be set*/,
  int val0 /**< [in] The default value*/)
{
  if (*val == 9999) {
    *val = val0;
    printf("  %15s = %-10d  ######  DEFAULT VALUE IS USED  ######\n", valname, *val);
  }
  else printf("  %15s = %-10d\n", valname, *val);
}

/**
 *
 * Stop HPhi if a variable (real) not used is specified
 *
 * @author Mitsuaki Kawamura (The University of Tokyo)
 */
void StdFace_NotUsed_d(
  char* valname /**< [in] Name of the valiable*/,
  double val /**< [in]*/)
{
  if (val != 9999.9) {
    printf("\n Check !  %s is SPECIFIED but will NOT be USED. \n", valname);
    printf("            Please COMMENT-OUT this line \n");
    printf("            or check this input is REALLY APPROPRIATE for your purpose ! \n\n");
    exit(-1);
  }
}

/**
 *
 * Stop HPhi if a variable (integer) not used is specified
 *
 * @author Mitsuaki Kawamura (The University of Tokyo)
 */
void StdFace_NotUsed_i(
  char* valname /**< [in] Name of the valiable*/,
  int val /**< [in]*/)
{
  if (val != 9999) {
    printf("\n Check !  %s is SPECIFIED but will NOT be USED. \n", valname);
    printf("            Please COMMENT-OUT this line \n");
    printf("            or check this input is REALLY APPROPRIATE for your purpose ! \n\n");
    exit(-1);
  }
}

/**
 *
 * Stop HPhi if a variable (integer) which must be specified
 * is absent in the input file.
 *
 * @author Mitsuaki Kawamura (The University of Tokyo)
 */
void StdFace_RequiredVal_i(
  char* valname /**< [in] Name of the valiable*/,
  int val /**< [in]*/)
{
  if (val == 9999){
    printf("ERROR ! %s is NOT specified !\n", valname);
    exit(-1);
  }
  else printf("  %15s = %-3d\n", valname, val);
}
