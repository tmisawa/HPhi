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
#include "expec_cisajs.h"

/** 
 * 
 * 
 * @param X 
 * @param vec 
 * 
 * @author Takahiro Misawa (The University of Tokyo)
 * @author Kazuyoshi Yoshimi (The University of Tokyo)
 * @return 
 */
int expec_cisajs(struct BindStruct *X,double complex *vec){

  FILE *fp;
  char sdt[D_FileNameMax];

  long unsigned int i,j;
  long unsigned int iexchg;
  long unsigned int irght,ilft,ihfbit;
  long unsigned int isite1,isite2;
  long unsigned int org_isite1,org_isite2,org_sigma1,org_sigma2;
  long unsigned int Asum,Adiff;
  long unsigned int tmp_off=0;
  double complex dam_pr;
  long int i_max;
  int tmp_sgn;

  //For TPQ
  int step=0;
  int rand_i=0;

  i_max = X->Check.idim_max;      
  if(GetSplitBitByModel(X->Def.Nsite, X->Def.iCalcModel, &irght, &ilft, &ihfbit)!=0){
    return -1;
  }
  X->Large.i_max    = i_max;
  X->Large.irght    = irght;
  X->Large.ilft     = ilft;
  X->Large.ihfbit   = ihfbit;
  X->Large.mode     = M_ENERGY;
 

  dam_pr=0.0;
  switch(X->Def.iCalcType){
  case Lanczos:
    if(X->Def.St==0){
      sprintf(sdt, cFileName1BGreen_Lanczos, X->Def.CDataFileHead);
      printf("%s", cLogLanczosExpecOneBodyGStart);
    }else if(X->Def.St==1){
      sprintf(sdt, cFileName1BGreen_CG, X->Def.CDataFileHead);
      printf("%s", cLogCGExpecOneBodyGStart);
    }
    //vec=v0;
    break;
  case TPQCalc:
    step=X->Def.istep;
    rand_i=X->Def.irand;
    TimeKeeperWithRandAndStep(X, cFileNameTimeKeep,  cTPQExpecOneBodyGStart, "a", rand_i, step);
    sprintf(sdt, cFileName1BGreen_TPQ, X->Def.CDataFileHead, rand_i, step);
    //vec=v0;
    break;
  case FullDiag:
    sprintf(sdt, cFileName1BGreen_FullDiag, X->Def.CDataFileHead, X->Phys.eigen_num);
    //vec=v0;
    break;
  }
  
  if(!childfopen(sdt, "w", &fp)==0){
    return -1;
  } 
  switch(X->Def.iCalcModel){
  case HubbardGC:

    for(i=0;i<X->Def.NCisAjt;i++){
      org_isite1 = X->Def.CisAjt[i][0]+1;
      org_isite2 = X->Def.CisAjt[i][2]+1;
      org_sigma1 = X->Def.CisAjt[i][1];
      org_sigma2 = X->Def.CisAjt[i][3];
      //	  printf(" %4ld %4ld %4ld %4ld \n",i,org_isite1-1, org_isite2-1, org_sigma1);
      if(child_general_hopp_GetInfo( X,org_isite1,org_isite2,org_sigma1,org_sigma2)!=0){
		return -1;
      }
      isite1 = X->Large.is1_spin;
      isite2 = X->Large.is2_spin;
      Asum   = X->Large.isA_spin;
      Adiff  = X->Large.A_spin;
      if(isite1==isite2 ){
		dam_pr =0.0;
#pragma omp parallel for default(none) reduction(+:dam_pr) private(j) firstprivate(i_max,X,isite1) shared(vec)
		for(j=1;j<=i_max;j++){
		  dam_pr += conj(vec[j])*vec[j]*X_CisAis(j-1,X,isite1); 
		}
		fprintf(fp," %4ld %4ld %4ld %4ld %.10lf %.10lf\n",org_isite1-1,org_sigma1,org_isite2-1,org_sigma2,creal(dam_pr),cimag(dam_pr));
      }else{
        dam_pr =0.0;
#pragma omp parallel for default(none) reduction(+:dam_pr) private(j,tmp_sgn, iexchg) firstprivate(i_max,X,Asum,Adiff,isite1,isite2,tmp_off) shared(vec)
        for(j=1;j<=i_max;j++){
          tmp_sgn = X_CisAjt((j-1),X,isite1,isite2,Asum,Adiff,&iexchg, &tmp_off);
          dam_pr += conj(vec[tmp_off+1])*vec[j]*tmp_sgn;
        }
        fprintf(fp," %4ld %4ld %4ld %4ld %.10lf %.10lf\n",org_isite1-1,org_sigma1,org_isite2-1,org_sigma2,creal(dam_pr),cimag(dam_pr));
      }
	}
	break;
  case KondoGC:
  case Hubbard:
  case Kondo:
    for(i=0;i<X->Def.NCisAjt;i++){
      org_isite1 = X->Def.CisAjt[i][0]+1;
      org_isite2 = X->Def.CisAjt[i][2]+1;
      org_sigma1 = X->Def.CisAjt[i][1];
      org_sigma2 = X->Def.CisAjt[i][3];
      //printf(" %4ld %4ld %4ld %4ld %4ld \n",i,org_isite1-1, org_isite2-1, org_sigma1, org_sigma2);
      if(child_general_hopp_GetInfo( X,org_isite1,org_isite2,org_sigma1,org_sigma2)!=0){
	return -1;
      }
      isite1 = X->Large.is1_spin;
      isite2 = X->Large.is2_spin;
      Asum   = X->Large.isA_spin;
      Adiff  = X->Large.A_spin;
      if(isite1==isite2){
	dam_pr =0.0;
#pragma omp parallel for default(none) reduction(+:dam_pr) private(j) firstprivate(i_max,X,isite1) shared(vec, list_1)
	for(j=1;j<=i_max;j++){
	  dam_pr += conj(vec[j])*vec[j]*X_CisAis(list_1[j],X,isite1); 
	}
	fprintf(fp," %4ld %4ld %4ld %4ld %.10lf %.10lf\n",org_isite1-1,org_sigma1,org_isite2-1,org_sigma2,creal(dam_pr),cimag(dam_pr));
      }else{
        dam_pr =0.0;
#pragma omp parallel for default(none) reduction(+:dam_pr) private(j,tmp_sgn, iexchg) firstprivate(i_max,X,Asum,Adiff,isite1,isite2,tmp_off) shared(vec, list_1)
        for(j=1;j<=i_max;j++){
          tmp_sgn = X_CisAjt(list_1[j],X,isite1,isite2,Asum,Adiff, &iexchg, &tmp_off);
          dam_pr += conj(vec[tmp_off])*vec[j]*tmp_sgn;
        }
        fprintf(fp," %4ld %4ld %4ld %4ld %.10lf %.10lf\n",org_isite1-1,org_sigma1,org_isite2-1,org_sigma2,creal(dam_pr),cimag(dam_pr));
      }
    }
    break;
  case Spin: // for the Sz-conserved spin system
    for(i=0;i<X->Def.NCisAjt;i++){
      org_isite1 = X->Def.CisAjt[i][0]+1;
      org_isite2 = X->Def.CisAjt[i][2]+1;
      org_sigma1 = X->Def.CisAjt[i][1];
      org_sigma2 = X->Def.CisAjt[i][3];

      if(org_sigma1 == org_sigma2){
	if(org_isite1==org_isite2){
	  isite1     = X->Def.Tpow[org_isite1-1];
	  dam_pr=0.0;
#pragma omp parallel for default(none) reduction(+:dam_pr) private(j) firstprivate(i_max, isite1, org_sigma1, X) shared(vec)
	  for(j=1;j<=i_max;j++){
	    dam_pr+=X_Spin_CisAis(j,X, isite1,org_sigma1)*conj(vec[j])*vec[j]; 
	  } 
	  fprintf(fp," %4ld %4ld %4ld %4ld %.10lf %.10lf\n",org_isite1-1, org_isite2-1,org_sigma1,org_sigma2,creal(dam_pr),cimag(dam_pr));
	}
	else{
	  
	  fprintf(fp," %4ld %4ld %4ld %4ld %.10lf %.10lf\n",org_isite1-1,org_isite2-1,org_sigma1,org_sigma2,0.0,0.0);
	}	
      }else{
	// for the canonical case 
	fprintf(fp," %4ld %4ld %4ld %4ld %.10lf %.10lf\n",org_isite1-1,org_isite2-1,org_sigma1,org_sigma2,0.0,0.0);
      }
    }
    break;
  case SpinGC: //for the Sz-nonconserved spin system
    for(i=0;i<X->Def.NCisAjt;i++){
      org_isite1 = X->Def.CisAjt[i][0]+1;
      org_isite2 = X->Def.CisAjt[i][2]+1;
      org_sigma1 = X->Def.CisAjt[i][1];
      org_sigma2 = X->Def.CisAjt[i][3];

      if(org_isite1 == org_isite2){ 
        isite1 = X->Def.Tpow[org_isite1-1];
        if(org_sigma1==org_sigma2){  
          // longitudinal magnetic field
		  dam_pr=0.0;
#pragma omp parallel for default(none) reduction(+:dam_pr) private(j) firstprivate(i_max, isite1, org_sigma1, X) shared(vec)
		  for(j=1;j<=i_max;j++){
			dam_pr+=X_Spin_CisAis(j,X,isite1,org_sigma1)*conj(vec[j])*vec[j]; 
		  } 
		  fprintf(fp," %4ld %4ld %4ld %4ld %.10lf %.10lf\n",org_isite1-1, org_isite2-1,org_sigma1,org_sigma2,creal(dam_pr),cimag(dam_pr));
        }else{
          // transverse magnetic field
          isite1 = X->Def.Tpow[org_isite1-1];
		  dam_pr=0.0;
#pragma omp parallel for default(none) reduction(+:dam_pr) private(j, tmp_sgn) firstprivate(i_max, isite1, org_sigma2, X,tmp_off) shared(vec)
		  for(j=1;j<=i_max;j++){
            tmp_sgn  =  X_SpinGC_CisAit(j,X, isite1,org_sigma2,&tmp_off);   
			dam_pr  +=  tmp_sgn*conj(vec[tmp_off+1])*vec[j]; 
		  } 
     	  fprintf(fp," %4ld %4ld %4ld %4ld %.10lf %.10lf\n",org_isite1-1,org_isite2-1,org_sigma1,org_sigma2,creal(dam_pr),cimag(dam_pr));
        }
      }else{
        // hopping is not allowed in localized spin system
		fprintf(fp," %4ld %4ld %4ld %4ld %.10lf %.10lf\n",org_isite1-1,org_isite2-1,org_sigma1,org_sigma2,0.0,0.0);
      }
    }
    break;
  default:
    return -1;
  }

  fclose(fp);
  if(X->Def.St==0){
    if(X->Def.iCalcType==Lanczos){
      TimeKeeper(X, cFileNameTimeKeep, cLanczosExpecOneBodyGFinish, "a");
      printf("%s", cLogLanczosExpecOneBodyGEnd);
    }
    else if(X->Def.iCalcType==TPQCalc){
      TimeKeeperWithRandAndStep(X, cFileNameTimeKeep, cTPQExpecOneBodyGFinish, "a", rand_i, step);     
    }
  }else if(X->Def.St==1){
    TimeKeeper(X, cFileNameTimeKeep, cExpecOneBodyGFinish, "a");
    printf("%s", cLogLanczosExpecOneBodyGEnd);
  }
  return 0;
}

/** 
 * 
 * 
 * @param j 
 * @param is1 
 * @param sigma1 
 * @param X 
 * @param vec 
 * 
 * @author Takahiro Misawa (The University of Tokyo)
 * @author Kazuyoshi Yoshimi (The University of Tokyo)
 * @return 
 */
double complex child_Green_0_Spin
(
 long int j,
 long int is1,
 int sigma1,
 struct BindStruct *X,
 double complex *vec
 )
{  
  long int ibit_1;
  double complex dam_pr;
    
  ibit_1=list_1[j]&is1;
  if(ibit_1 == sigma1){
	dam_pr = conj(vec[j])*vec[j];
  }else{
    dam_pr = 0.0;
  }
  return dam_pr;
}
