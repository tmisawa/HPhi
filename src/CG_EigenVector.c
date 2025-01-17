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

#include "CG_EigenVector.h"

/** 
 * 
 * 
 * @param X 
 * @author Takahiro Misawa (The University of Tokyo)
 * @author Kazuyoshi Yoshimi (The University of Tokyo)
 * @return 
 */
int CG_EigenVector(struct BindStruct *X){

  printf("%s", cLogCG_EigenVecStart);
  
  time_t start,mid;
  FILE *fp_0;
  char sdt_1[D_FileNameMax];
  dsfmt_t dsfmt;
  long unsigned int u_long_i;

  long int i,j;
  double Eig;
  int i_itr,itr,iv,itr_max;
  int t_itr;
  double bnorm,xnorm,rnorm,rnorm2;
  double complex alpha,beta,xb,rp,yp,gosa1,tmp_r,gosa2;
  double complex *y,*b;
  long int L_size;
  long int i_max; 
  iv=0;
  i_max=X->Check.idim_max;    
  Eig=X->Phys.Target_energy;
    
  strcpy(sdt_1, cFileNameTimeEV_CG);
  if(childfopen(sdt_1, "w", &fp_0) !=0){
    return -1;
  }
    
  L_size=sizeof(double complex)*(i_max+1);
    
  b=(double complex *)malloc(L_size);
  y=(double complex *)malloc(L_size);
    
  if(y==NULL){
    fprintf(fp_0,"BAD in CG_EigenVector  \n");
  }else{
    fprintf(fp_0,"allocate succeed !!! \n");
  }
  fclose(fp_0);
        

  start=time(NULL);  
// add random components
  iv = X->Def.initial_iv;
  bnorm = 0.0;
  u_long_i = 123432 + abs(iv);
  dsfmt_init_gen_rand(&dsfmt, u_long_i);    
  for(i=1;i<=i_max;i++){
    b[i]=v0[i]+2.0*(dsfmt_genrand_close_open(&dsfmt)-0.5)*0.001;
    bnorm+=conj(b[i])*b[i];
  }
#pragma omp parallel for default(none) private(i) shared(b) firstprivate(i_max,bnorm)
  for(i=1;i<=i_max;i++){
    b[i]=b[i]/bnorm;
  }

  t_itr=0;
  for(i_itr=0;i_itr<=50;i_itr++){
    //CG start!!
    bnorm=0.0;
    //initialization
#pragma omp parallel for reduction(+:bnorm) default(none) private(i) shared(b, v1, vg, v0) firstprivate(i_max)
    for(i=1;i<=i_max;i++){
      bnorm+=conj(b[i])*b[i];
      v1[i]=b[i];
      vg[i]=b[i];
      v0[i]=0.0;
    }
    childfopen(sdt_1,"a",&fp_0);
    fprintf(fp_0,"b[%d]=%lf bnorm== %lf \n ",iv,creal(b[iv]),bnorm);
    fclose(fp_0);           

    //iteration
    if(i_itr==0){
      itr_max=500;
    }else{
      itr_max=500;
    }
    
    for(itr=1;itr<=itr_max;itr++){
#pragma omp parallel for default(none) private(j) shared(y, vg) firstprivate(i_max, Eig,eps_CG)
      for(j=1;j<=i_max;j++){  
	y[j]=(-Eig+eps_CG)*vg[j];   //y = -E*p
      }

      mltply(X,y,vg);      // y += H*p

      // (H-E)p=y finish!
      rp=0.0;
      yp=0.0;
#pragma omp parallel for reduction(+:rp, yp) default(none) private(i) shared(v1, vg, y) firstprivate(i_max) 
      for(i=1;i<=i_max;i++){
	rp+=v1[i]*conj(v1[i]);
	yp+=y[i]*conj(vg[i]);
      }
      alpha=rp/yp;
      rnorm=0.0;
#pragma omp parallel for reduction(+:rnorm) default(none) private(i) shared(v0, v1, vg)firstprivate(i_max, alpha) 
      for(i=1;i<=i_max;i++){
	v0[i]+=alpha*vg[i];
	rnorm+=conj(v1[i])*v1[i];
      }
      rnorm2=0.0;
      gosa1=0.0;
#pragma omp parallel for reduction(+:rnorm2, gosa1) default(none) private(i) shared(v1 , y) firstprivate(i_max, alpha) private(tmp_r) 
      for(i=1;i<=i_max;i++){
	tmp_r=v1[i]-alpha*y[i];
	gosa1+=conj(tmp_r)*v1[i];// old r and new r should be orthogonal -> gosa1=0
	v1[i]=tmp_r; 
	rnorm2+=conj(v1[i])*v1[i];
      }
            
      gosa2=0.0;
#pragma omp parallel for reduction(+:gosa2) default(none) private(i) shared(v1, vg) firstprivate(i_max) 
      for(i=1;i<=i_max;i++){
	gosa2+=v1[i]*conj(vg[i]); // new r and old p should be orthogonal
      }
            
      beta=rnorm2/rnorm;
#pragma omp parallel for default(none) shared(v1, vg) firstprivate(i_max, beta)
      for(i=1;i<=i_max;i++){
	vg[i]=v1[i]+beta*vg[i];
      }
      if(itr%5==0){
	childfopen(sdt_1,"a", &fp_0);
	fprintf(fp_0,"i_itr=%d itr=%d %.10lf %.10lf \n ",
		i_itr,itr,sqrt(rnorm2),pow(10,-5)*sqrt(bnorm));
	fclose(fp_0);                
	if(sqrt(rnorm2)<eps*sqrt(bnorm)){
	  t_itr+=itr;
	  childfopen(sdt_1,"a", &fp_0);
	  fprintf(fp_0,"CG OK:   t_itr=%d \n ",t_itr);
	  fclose(fp_0); 
	  break;
	}
      }
    }
    //CG finish!!
    xnorm=0.0;
#pragma omp parallel for reduction(+:xnorm) default(none) private(i) shared(v0) firstprivate(i_max)
    for(i=1;i<=i_max;i++){
      xnorm+=conj(v0[i])*v0[i];
    }
    xnorm=sqrt(xnorm);

#pragma omp parallel for default(none) shared(v0) firstprivate(i_max, xnorm) 
    for(i=1;i<=i_max;i++){
      v0[i]=v0[i]/xnorm;     
    }
    xb=0.0;

#pragma omp parallel for default(none) reduction(+:xb) private(i) shared(v0, b) firstprivate(i_max)
    for(i=1;i<=i_max;i++){
      xb+=conj(v0[i])*b[i];
    }
       
    mid=time(NULL);
       
    childfopen(sdt_1,"a", &fp_0);
    fprintf(fp_0,"i_itr=%d itr=%d time=%lf  fabs(fabs(xb)-1.0)=%.16lf\n"
	    ,i_itr,itr,difftime(mid,start),fabs(cabs(xb)-1.0));
    fclose(fp_0);
        
    if(fabs(fabs(xb)-1.0)<eps){
      childfopen(sdt_1,"a", &fp_0);
      fprintf(fp_0,"number of iterations in inv1:i_itr=%d itr=%d t_itr=%d %lf\n ",
	      i_itr,itr,t_itr,fabs(cabs(xb)-1.0));
      fclose(fp_0);
          
      break;
    }else{
#pragma omp parallel for default(none) private(i) shared(b, v0) firstprivate(i_max)
      for(i=1;i<=i_max;i++){
	b[i]=v0[i];
      }
    }       
    //inv1 routine finish!!
  }
    
  free(b);
  free(y);

  TimeKeeper(X, cFileNameTimeKeep, cCG_EigenVecFinish, "a");
  printf("%s", cLogCG_EigenVecEnd);
  
  return 0;
}
