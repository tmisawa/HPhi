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
#include "xsetmem.h"
void setmem_HEAD
(
 struct BindStruct *X
 )
{
  X->Def.CDataFileHead = (char*)malloc(D_FileNameMax*sizeof(char));
  X->Def.CParaFileHead = (char*)malloc(D_FileNameMax*sizeof(char));
}

void setmem_def
(
 struct BindStruct *X
 )
{
  li_malloc1(X->Def.Tpow, 2*X->Def.Nsite+2);
  i_malloc1(X->Def.LocSpn, X->Def.Nsite);
  d_malloc1(X->Phys.spin_real_cor, X->Def.Nsite*X->Def.Nsite);
  d_malloc1(X->Phys.charge_real_cor, X->Def.Nsite*X->Def.Nsite);
  d_malloc1(X->Phys.loc_spin_z, X->Def.Nsite*X->Def.Nsite);
  
  i_malloc1(X->Def.EDChemi, 2*X->Def.Nsite);
  i_malloc1(X->Def.EDSpinChemi, 2*X->Def.Nsite);
  d_malloc1(X->Def.EDParaChemi, 2*X->Def.Nsite);

  i_malloc2(X->Def.EDGeneralTransfer, X->Def.NTransfer, 4);
  i_malloc2(X->Def.GeneralTransfer, X->Def.NTransfer, 4);
  c_malloc1(X->Def.EDParaGeneralTransfer, X->Def.NTransfer);
  c_malloc1(X->Def.ParaGeneralTransfer, X->Def.NTransfer);  
  
  i_malloc2(X->Def.CoulombIntra, X->Def.NCoulombIntra, 1);
  d_malloc1(X->Def.ParaCoulombIntra, X->Def.NCoulombIntra);
  i_malloc2(X->Def.CoulombInter, X->Def.NCoulombInter, 2);
  d_malloc1(X->Def.ParaCoulombInter, X->Def.NCoulombInter);
  i_malloc2(X->Def.HundCoupling, X->Def.NHundCoupling, 2);
  d_malloc1(X->Def.ParaHundCoupling, X->Def.NHundCoupling);
  i_malloc2(X->Def.PairHopping, X->Def.NPairHopping, 2);
  d_malloc1(X->Def.ParaPairHopping, X->Def.NPairHopping); 
  i_malloc2(X->Def.ExchangeCoupling, X->Def.NExchangeCoupling, 2);
  d_malloc1(X->Def.ParaExchangeCoupling, X->Def.NExchangeCoupling);
  i_malloc2(X->Def.PairLiftCoupling, X->Def.NPairLiftCoupling, 2);
  d_malloc1(X->Def.ParaPairLiftCoupling, X->Def.NPairLiftCoupling);

  i_malloc2(X->Def.InterAll, X->Def.NInterAll, 8);
  c_malloc1(X->Def.ParaInterAll, X->Def.NInterAll);
    
  i_malloc2(X->Def.CisAjt, X->Def.NCisAjt, 4);
  i_malloc2(X->Def.CisAjtCkuAlvDC, X->Def.NCisAjtCkuAlvDC, 8);
}

int setmem_large
(
 struct BindStruct *X
 )
{
  int j=0;
  lui_malloc1(list_1, X->Check.idim_max+1);
  if(X->Def.iCalcModel==Spin &&X->Def.Nsite%2==1){
      lui_malloc1(list_2_1, X->Check.sdim*2+2);
  }
  else{
    lui_malloc1(list_2_1, X->Check.sdim+2);
  }
  
  lui_malloc1(list_2_2, X->Check.sdim+2);
  lui_malloc1(list_jb, X->Check.sdim+2);
  i_malloc1(list_3, X->Check.sdim+1);
  d_malloc1(list_Diagonal, X->Check.idim_max+1);
  c_malloc1(v0, X->Check.idim_max+1);
  c_malloc1(v1, X->Check.idim_max+1);
  d_malloc1(alpha, X->Def.Lanczos_max+1);
  d_malloc1(beta, X->Def.Lanczos_max+1);
  c_malloc1(vg, X->Check.idim_max+1);
  if(list_1==NULL
     || list_2_1==NULL
     || list_2_2==NULL
     || list_jb==NULL
     || list_3==NULL
     || list_Diagonal==NULL
     || v0==NULL
     || v1==NULL
     || alpha==NULL
     || beta==NULL
     || vg==NULL
     ){
    return -1;
  }
  c_malloc2(vec,X->Def.nvec+1, X->Def.Lanczos_max+1);
  for(j=0; j<X->Def.nvec+1; j++){
    if(vec[j]==NULL){
      return -1;
    }
  }
  
  if(X->Def.iCalcType == FullDiag){
    d_malloc1(X->Phys.all_num_down, X->Check.idim_max+1);
    d_malloc1(X->Phys.all_num_up, X->Check.idim_max+1);
    d_malloc1(X->Phys.all_energy, X->Check.idim_max+1);
    d_malloc1(X->Phys.all_doublon, X->Check.idim_max+1);
    d_malloc1(X->Phys.all_s2, X->Check.idim_max+1);
    c_malloc2(Ham, X->Check.idim_max+1,X->Check.idim_max+1);
    c_malloc2(L_vec, X->Check.idim_max+1,X->Check.idim_max+1);

    if(X->Phys.all_num_down == NULL
       ||X->Phys.all_num_up == NULL
       ||X->Phys.all_energy == NULL
       ||X->Phys.all_doublon == NULL
       ||X->Phys.all_s2 ==NULL
       )
      {
	return -1;
      }
    for(j=0; j<X->Check.idim_max+1; j++){
      if(Ham[j]==NULL || L_vec[j]==NULL){
	return -1;
      }
    }
  }
  printf("%s", cProFinishAlloc);
  return 0;
}

void setmem_IntAll_Diagonal
(
 struct DefineList *X
 )
{
  i_malloc2(X->InterAll_OffDiagonal, X->NInterAll, 8);
    c_malloc1(X->ParaInterAll_OffDiagonal, X->NInterAll);
  //  c_malloc1(X->ParaInterAll_OffDiagonal, X->NInterAll_OffDiagonal);

  i_malloc2(X->InterAll_Diagonal, X->NInterAll, 4);
  //  d_malloc1(X->ParaInterAll_Diagonal, X->NInterAll_Diagonal);
    d_malloc1(X->ParaInterAll_Diagonal, X->NInterAll);
}
