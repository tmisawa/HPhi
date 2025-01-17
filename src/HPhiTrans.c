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
#include "HPhiTrans.h"

/** 
 * 
 * 
 * @param X 
 * 
 * @author Takahiro Misawa (The University of Tokyo)
 * @author Kazuyoshi Yoshimi (The University of Tokyo)
 *
 * @return 
 */
int HPhiTrans(struct BindStruct *X){ 
  FILE *fp_err;
  char sdt_err[D_FileNameMax];

  int i,k;
  int cnt_trans,cnt_chemi;

  strcpy(sdt_err, cFileNameWarningOnTransfer);
  if(childfopen(sdt_err, "w", &fp_err)!=0){
    return -1;
  }
  fclose(fp_err);
	 
  //Transefer
  cnt_trans=0;
  cnt_chemi=0;

  for(i=0;i<X->Def.NTransfer;i++){
    // chemical potential = diagonal
    // check site 
    if(X->Def.GeneralTransfer[i][0]==X->Def.GeneralTransfer[i][2]){
      // check spin
      if(X->Def.GeneralTransfer[i][1]==X->Def.GeneralTransfer[i][3]){
	X->Def.EDChemi[cnt_chemi]     = X->Def.GeneralTransfer[i][0];      
	X->Def.EDSpinChemi[cnt_chemi] = X->Def.GeneralTransfer[i][1];      
	X->Def.EDParaChemi[cnt_chemi] = creal(X->Def.ParaGeneralTransfer[i]);
	cnt_chemi+=1;
      }
      else{
	X->Def.EDGeneralTransfer[cnt_trans][0]  = X->Def.GeneralTransfer[i][0];      
	X->Def.EDGeneralTransfer[cnt_trans][1]  = X->Def.GeneralTransfer[i][1];      
	X->Def.EDGeneralTransfer[cnt_trans][2]  = X->Def.GeneralTransfer[i][2];
	X->Def.EDGeneralTransfer[cnt_trans][3]  = X->Def.GeneralTransfer[i][3];      
	X->Def.EDParaGeneralTransfer[cnt_trans] = X->Def.ParaGeneralTransfer[i];
	cnt_trans+=1;
      }
    }else{
      // eliminate double counting
      for(k=0;k<cnt_trans;k++){
	if( X->Def.GeneralTransfer[i][1] == X->Def.EDGeneralTransfer[k][1]
	    && X->Def.GeneralTransfer[i][3] == X->Def.EDGeneralTransfer[k][3]){
	  if(X->Def.GeneralTransfer[i][0] == X->Def.EDGeneralTransfer[k][0]
	     && X->Def.GeneralTransfer[i][2] == X->Def.EDGeneralTransfer[k][2]){
	    sprintf(sdt_err,cErrTransfer);
	    childfopen(sdt_err,"a", &fp_err);
	    fprintf(fp_err,cErrDoubleCounting, X->Def.GeneralTransfer[i][0] ,X->Def.EDGeneralTransfer[k][2], X->Def.EDGeneralTransfer[k][1], X->Def.EDGeneralTransfer[k][3]);
	    fclose(fp_err);
	    return -1;
	  }
	}
      }
      X->Def.EDGeneralTransfer[cnt_trans][0]  = X->Def.GeneralTransfer[i][0];      
      X->Def.EDGeneralTransfer[cnt_trans][1]  = X->Def.GeneralTransfer[i][1];      
      X->Def.EDGeneralTransfer[cnt_trans][2]  = X->Def.GeneralTransfer[i][2];
      X->Def.EDGeneralTransfer[cnt_trans][3]  = X->Def.GeneralTransfer[i][3];      
      X->Def.EDParaGeneralTransfer[cnt_trans] = X->Def.ParaGeneralTransfer[i];
      cnt_trans+=1;
    }
  }
// PossibleError
  if(cnt_chemi>0 && cnt_chemi!=2*X->Def.Nsite){
    sprintf(sdt_err,cErrTransfer);
    childfopen(sdt_err,"a", &fp_err);
    fprintf(fp_err,cErrChemicalPotential);
    fclose(fp_err);
    return -1;
  }
//

  printf(cProEDNTrans, cnt_trans);
  printf(cProEDNChemi, cnt_chemi);
  X->Def.EDNTransfer=cnt_trans;
  X->Def.EDNChemi=cnt_chemi;

  return 0;
}    
