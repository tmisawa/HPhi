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
#include "makeHam.h"

/** 
 * 
 * 
 * @param X 
 * 
 * @return 
 * @author Takahiro Misawa (The University of Tokyo)
 * @author Kazuyoshi Yoshimi (The University of Tokyo)
 */
int makeHam(struct BindStruct *X){

  long unsigned int i,j;
  long unsigned int iexchg;
  long unsigned int is1_spin;
  long unsigned int irght,ilft,ihfbit;
  double complex dmv;
  long unsigned int off;
  int isite1,isite2,isite3,isite4;
  int sigma1,sigma2,sigma3,sigma4;
  int isA_up, isB_up;
  double complex tmp_trans,tmp_V;
  long unsigned int Asum,Bsum,Adiff,Bdiff;
  long unsigned int tmp_off,tmp_off_2;
  int tmp_sgn;
  off=0;
  
  long unsigned int i_max;
  i_max=X->Check.idim_max;
  if(GetSplitBitByModel(X->Def.Nsite, X->Def.iCalcModel, &irght, &ilft, &ihfbit)!=0){
    return -1;
  }
  X->Large.i_max    = i_max;
  X->Large.irght    = irght;
  X->Large.ilft     = ilft;
  X->Large.ihfbit   = ihfbit;
  X->Large.prdct=0.0;
  X->Large.mode=M_Ham;

  for(i=0;i<=i_max;i++){
    for(j=0;j<=i_max;j++){
      Ham[i][j]=0;  
    }
  }
  #pragma omp parallel for default(none) firstprivate(i_max) private(j) shared(Ham, list_Diagonal,v0,v1)
  for(j=1;j<=i_max;j++){  
    Ham[j][j]+=list_Diagonal[j];  
    v0[j]     = 1.0;      
    v1[j]     = 1.0;
  }

  switch(X->Def.iCalcModel){
  case HubbardGC:
    //Transfer
    for(i = 0;i< X->Def.EDNTransfer; i++){
      isite1     = X->Def.EDGeneralTransfer[i][0]+1;
      isite2     = X->Def.EDGeneralTransfer[i][2]+1;
      sigma1     = X->Def.EDGeneralTransfer[i][1];
      sigma2     = X->Def.EDGeneralTransfer[i][3];
      if(child_general_hopp_GetInfo(X,isite1,isite2,sigma1,sigma2)!=0){
	return -1;
      }
      tmp_trans      = -X->Def.EDParaGeneralTransfer[i];

      for(j=1;j<=X->Large.i_max;j++){
	dmv=tmp_trans*GC_CisAjt(j, v0, v1, X, X->Large.is1_spin, X->Large.is2_spin, X->Large.isA_spin,X->Large.A_spin, tmp_trans, &tmp_off);
	Ham[tmp_off+1][j]  += dmv;
      }
    }
    
    //InterAll
    for(i = 0;i< X->Def.NInterAll_OffDiagonal; i++){    
      isite1 = X->Def.InterAll_OffDiagonal[i][0]+1;
      isite2 = X->Def.InterAll_OffDiagonal[i][2]+1;
      isite3 = X->Def.InterAll_OffDiagonal[i][4]+1;
      isite4 = X->Def.InterAll_OffDiagonal[i][6]+1;
      sigma1 = X->Def.InterAll_OffDiagonal[i][1];
      sigma2 = X->Def.InterAll_OffDiagonal[i][3];
      sigma3 = X->Def.InterAll_OffDiagonal[i][5];
      sigma4 = X->Def.InterAll_OffDiagonal[i][7];
      tmp_V  = X->Def.ParaInterAll_OffDiagonal[i];    

      child_general_int_GetInfo(
       i, 
       X,
       isite1,
       isite2,
       isite3,
       isite4,
       sigma1,
       sigma2,
       sigma3,
       sigma4,
       tmp_V
      );

      i_max  = X->Large.i_max;
      isite1 = X->Large.is1_spin;
      isite2 = X->Large.is2_spin;
      Asum   = X->Large.isA_spin;
      Adiff  = X->Large.A_spin;

      isite3 = X->Large.is3_spin;
      isite4 = X->Large.is4_spin;
      Bsum   = X->Large.isB_spin;
      Bdiff  = X->Large.B_spin;

      tmp_V  = X->Large.tmp_V;

      if(isite1 == isite2 && isite3 == isite4){

	for(j=1;j<=i_max;j++){
	  dmv =GC_child_CisAisCisAis_element(j, isite1, isite3, tmp_V, v0, v1, X, &tmp_off);  
	  Ham[j][j]   += dmv;
	}
      }else if(isite1 == isite2 && isite3 != isite4){

	for(j=1;j<=i_max;j++){
	  dmv = GC_child_CisAisCjtAku_element(j, isite1, isite3, isite4, Bsum, Bdiff, tmp_V, v0, v1, X, &tmp_off);
	  Ham[tmp_off+1][j] += dmv;  
	}
      }else if(isite1 != isite2 && isite3 == isite4){

	for(j=1;j<=i_max;j++){
	  dmv = GC_child_CisAjtCkuAku_element(j, isite1, isite2, isite3, Asum, Adiff, tmp_V, v0, v1, X, &tmp_off);
	  Ham[tmp_off+1][j] += dmv;
	}
      }else if(isite1 != isite2 && isite3 != isite4){

	for(j=1;j<=i_max;j++){
	  dmv = GC_child_CisAjtCkuAlv_element(j, isite1, isite2, isite3, isite4, Asum, Adiff, Bsum, Bdiff, tmp_V, v0, v1, X, &tmp_off_2);   
	  Ham[tmp_off_2+1][j] += dmv;
	} 
      }
    }

    //Pair hopping
    for(i = 0;i< X->Def.NPairHopping; i++){
      child_pairhopp_GetInfo(i, X);
 
      for(j=1;j<=X->Large.i_max;j++){
	dmv            = GC_child_pairhopp_element(j, v0, v1, X,&tmp_off);
	Ham[tmp_off+1][j] += dmv;
      }
    }
    //Exchange
    for(i = 0;i< X->Def.NExchangeCoupling; i++){
      child_exchange_GetInfo(i, X);
 
      for(j=1;j<=X->Large.i_max;j++){
	dmv            = GC_child_exchange_element(j, v0, v1, X,&tmp_off);
	Ham[tmp_off+1][j] += dmv;
      }    
    }
    break;
  case KondoGC:
  case Hubbard:
  case Kondo:
    printf("transfer\n");
    //Transfer
    for(i = 0;i< X->Def.EDNTransfer; i++){
      isite1     = X->Def.EDGeneralTransfer[i][0]+1;
      isite2     = X->Def.EDGeneralTransfer[i][2]+1;
      sigma1     = X->Def.EDGeneralTransfer[i][1];
      sigma2     = X->Def.EDGeneralTransfer[i][3];
      if(child_general_hopp_GetInfo( X,isite1,isite2,sigma1,sigma2)!=0){
	return -1;
      }
      tmp_trans      = -X->Def.EDParaGeneralTransfer[i];

      for(j=1;j<=X->Large.i_max;j++){
	dmv               = tmp_trans*X_CisAjt(list_1[j], X,X->Large.is1_spin,X->Large.is2_spin,X->Large.isA_spin,X->Large.A_spin,&iexchg, &tmp_off);
	Ham[tmp_off][j]  += dmv;
      }
    }

    printf("interall:%d\n",X->Def.NInterAll_OffDiagonal);
    //InterAll
    for(i = 0;i< X->Def.NInterAll_OffDiagonal; i++){    
      isite1 = X->Def.InterAll_OffDiagonal[i][0]+1;
      isite2 = X->Def.InterAll_OffDiagonal[i][2]+1;
      isite3 = X->Def.InterAll_OffDiagonal[i][4]+1;
      isite4 = X->Def.InterAll_OffDiagonal[i][6]+1;
      sigma1 = X->Def.InterAll_OffDiagonal[i][1];
      sigma2 = X->Def.InterAll_OffDiagonal[i][3];
      sigma3 = X->Def.InterAll_OffDiagonal[i][5];
      sigma4 = X->Def.InterAll_OffDiagonal[i][7];
      tmp_V  = X->Def.ParaInterAll_OffDiagonal[i];    

      child_general_int_GetInfo(
       i, 
       X,
       isite1,
       isite2,
       isite3,
       isite4,
       sigma1,
       sigma2,
       sigma3,
       sigma4,
       tmp_V
      );

      i_max  = X->Large.i_max;
      isite1 = X->Large.is1_spin;
      isite2 = X->Large.is2_spin;
      Asum   = X->Large.isA_spin;
      Adiff  = X->Large.A_spin;

      isite3 = X->Large.is3_spin;
      isite4 = X->Large.is4_spin;
      Bsum   = X->Large.isB_spin;
      Bdiff  = X->Large.B_spin;

      tmp_V  = X->Large.tmp_V;

      if(isite1 == isite2 && isite3 == isite4){

	for(j=1;j<=i_max;j++){
	  dmv = child_CisAisCisAis_element(j, isite1, isite3, tmp_V, v0, v1, X, &tmp_off);
	  Ham[j][j]   += dmv;  
	}
      }else if(isite1 == isite2 && isite3 != isite4){

	for(j=1;j<=i_max;j++){
	  dmv =child_CisAisCjtAku_element(j, isite1, isite3, isite4, Bsum, Bdiff, tmp_V, v0, v1, X, &tmp_off);
	  Ham[tmp_off][j] += dmv; 
	}  
      }else if(isite1 != isite2 && isite3 == isite4){

	for(j=1;j<=i_max;j++){
	  dmv=child_CisAjtCkuAku_element(j, isite1, isite2, isite3, Asum, Adiff, tmp_V, v0, v1, X, &tmp_off);
	  Ham[tmp_off][j] += dmv; 
	}
      }else if(isite1 != isite2 && isite3 != isite4){

	for(j=1;j<=i_max;j++){
	  dmv=child_CisAjtCkuAlv_element(j, isite1, isite2, isite3, isite4, Asum, Adiff, Bsum, Bdiff, tmp_V, v0, v1, X, &tmp_off_2);
	  Ham[tmp_off_2][j] += dmv;  
	} 
      }
    }

    printf("pairhopp\n");
    //Pair hopping
    for(i = 0;i< X->Def.NPairHopping; i++){
      child_pairhopp_GetInfo(i, X);        

      for(j=1;j<=X->Large.i_max;j++){
	dmv          = child_pairhopp_element(j, v0, v1, X,&tmp_off);
	Ham[tmp_off][j] += dmv;
      }
    }
    
    printf("exchange\n");
    //Exchange
    for(i = 0;i< X->Def.NExchangeCoupling; i++){
      child_exchange_GetInfo(i, X);

      for(j=1;j<=X->Large.i_max;j++){
	dmv          = child_exchange_element(j, v0, v1, X,&tmp_off);
	Ham[tmp_off][j] += dmv;
      }
    }
    break;
    
  case SpinGC:
    
    //Transfer
    for(i=0;i< X->Def.EDNTransfer;i++){
      isite1     = X->Def.EDGeneralTransfer[i][0]+1;
      isite2     = X->Def.EDGeneralTransfer[i][2]+1;
      sigma1     = X->Def.EDGeneralTransfer[i][1];
      sigma2     = X->Def.EDGeneralTransfer[i][3];
      if(child_general_hopp_GetInfo( X,isite1,isite2,sigma1,sigma2)!=0){
	return -1;
      }
      tmp_trans  = -X->Def.EDParaGeneralTransfer[i];
      
      if(isite1 == isite2){ 
        is1_spin = X->Def.Tpow[isite1-1];
        if(sigma1==sigma2){  
          // longitudinal magnetic field
	  for(j=1;j<=i_max;j++){
	    Ham[j][j]+=tmp_trans*X_Spin_CisAis(j,X,is1_spin, sigma1); 
	  } 
        }else{
          // transverse magnetic field
          is1_spin = X->Def.Tpow[isite1-1];

	  for(j=1;j<=i_max;j++){
	    Ham[off+1][j] +=  X_SpinGC_CisAit(j,X, is1_spin, sigma2, &off); 
	  } 
        }
      }else{
        // hopping is not allowed in localized spin system
	return -1;
      }
    }
    
 //InterAll
    for(i = 0;i< X->Def.NInterAll_OffDiagonal; i++){    
      isite1 = X->Def.InterAll_OffDiagonal[i][0]+1;
      isite2 = X->Def.InterAll_OffDiagonal[i][4]+1;
      sigma1 = X->Def.InterAll_OffDiagonal[i][1];
      sigma2 = X->Def.InterAll_OffDiagonal[i][3];
      sigma3 = X->Def.InterAll_OffDiagonal[i][5];
      sigma4 = X->Def.InterAll_OffDiagonal[i][7];
      tmp_V  = X->Def.ParaInterAll_OffDiagonal[i];
      child_general_int_spin_GetInfo( X, isite1, isite2, sigma1, sigma2, sigma3, sigma4, tmp_V);
      isA_up = X->Def.Tpow[isite1-1];
      isB_up = X->Def.Tpow[isite2-1];

      if(sigma1==sigma2 && sigma3==sigma4 ){ //diagonal	
	for(j=1;j<=i_max;j++){
	  dmv =GC_child_CisAisCisAis_spin_element(j, isA_up, isB_up, sigma2, sigma4, tmp_V, v0, v1, X);
	  Ham[j][j]      += dmv;
	}
      }
      else  if(sigma1 == sigma2 && sigma3 != sigma4){ 	
	for(j=1;j<=i_max;j++){
	  dmv = GC_child_CisAisCitAiu_spin_element(j, sigma2, sigma4, isA_up, isB_up, tmp_V, v0, v1, X, &tmp_off);
	  Ham[tmp_off+1][j]    += dmv;
	}
      }else if(sigma1 != sigma2 && sigma3 == sigma4){ 
	for(j=1;j<=i_max;j++){
	  dmv = GC_child_CisAitCiuAiu_spin_element(j, sigma2, sigma4, isA_up, isB_up, tmp_V, v0, v1, X, &tmp_off);
	  Ham[tmp_off+1][j]    += dmv;
	} 
      }else if(sigma1 != sigma2 && sigma3 != sigma4){ 
	for(j=1;j<=i_max;j++){
	  dmv = GC_child_CisAitCiuAiv_spin_element(j, sigma2, sigma4, isA_up, isB_up, tmp_V, v0, v1, X, &tmp_off_2);
	  Ham[tmp_off_2+1][j]    += dmv;
	}
      }	
    }

    //Exchange
    for(i = 0;i< X->Def.NExchangeCoupling; i++){
      child_exchange_spin_GetInfo(i, X);

      for(j=1;j<=X->Large.i_max;j++){
	dmv =GC_child_exchange_spin_element(j, v0, v1, X,&tmp_off);
	Ham[tmp_off+1][j] +=dmv;
      }
    }
    //PairLift
    for(i = 0;i< X->Def.NPairLiftCoupling; i++){
      child_pairlift_spin_GetInfo(i, X);

      for(j=1;j<=X->Large.i_max;j++){
	dmv =child_pairlift_spin_element(j, v0, v1, X,&tmp_off);
     	Ham[tmp_off+1][j] +=dmv;
      }
    }

    break;
    
  case Spin:
    //Transfer is abosrbed in diagonal term.
    
    //InterAll
    for(i = 0;i< X->Def.NInterAll_OffDiagonal; i++){    
      isite1 = X->Def.InterAll_OffDiagonal[i][0]+1;
      isite2 = X->Def.InterAll_OffDiagonal[i][4]+1;
      sigma1 = X->Def.InterAll_OffDiagonal[i][1];
      sigma2 = X->Def.InterAll_OffDiagonal[i][3];
      sigma3 = X->Def.InterAll_OffDiagonal[i][5];
      sigma4 = X->Def.InterAll_OffDiagonal[i][7];
      tmp_V  = X->Def.ParaInterAll_OffDiagonal[i];
      child_general_int_spin_GetInfo( X, isite1, isite2, sigma1, sigma2, sigma3, sigma4, tmp_V);
      isA_up = X->Large.is1_up;
      isB_up = X->Large.is2_up;
      
      for(j=1;j<=i_max;j++){
	tmp_sgn    =  X_child_exchange_spin_element(j,X, isA_up, isB_up, sigma2, sigma4,&tmp_off);
	dmv        = tmp_sgn*tmp_V;
	Ham[tmp_off][j]     += dmv;
      }
    }
    
    //Exchange
    for(i = 0;i< X->Def.NExchangeCoupling; i++){
      child_exchange_spin_GetInfo(i, X);

      for(j=1;j<=X->Large.i_max;j++){
	dmv          = child_exchange_spin_element(j, v0, v1, X,&tmp_off);
	Ham[tmp_off][j] += dmv;
      }
    }

    //PairLift
    for(i = 0;i< X->Def.NPairLiftCoupling; i++){
      child_pairlift_spin_GetInfo(i, X);

      for(j=1;j<=X->Large.i_max;j++){
	dmv =child_pairlift_spin_element(j, v0, v1, X,&tmp_off);
     	Ham[tmp_off][j] +=dmv;
      }
    }
    break;
  }
  return 0;
}
