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
/*-------------------------------------------------------------
 *[ver.2009.05.25]
 *-------------------------------------------------------------
 * Copyright (C) 2007-2009 Daisuke Tahara. All rights reserved.
 * Copyright (C) 2009-     Takahiro Misawa. All rights reserved.
 * Some functions are added by TM.
 *-------------------------------------------------------------*/
/*=================================================================================================*/

#define M_DSYEV dsyev_
#include "mfmemory.c"

int dgetrf_(int *m, int *n, double *a, int *lda, int *ipiv, int *info);
int dgetri_(int *n, double *a, int *lda, int *ipiv, double *work, int *lwork, int *info);
int dsyev_(char *jobz, char *uplo, int *n, double *a, int *lda, double *w, double *work, int *lwork, int *info);

double dlamch_(char *cmach);
int dsyevx_(char *jobz, char *range, char *uplo, int *n, double *a, int *lda, double *vl, double *vu, 
	int *il, int *iu, double *abstol, int *m, double *w, double *z__, int *ldz, 
	double *work, int *lwork, int *iwork, int *ifail, int *info);

int M_DSYEV(char *jobz, char *uplo, int *n, double *a, int *lda, double *w, double *work, int *lwork, int *info);
int zheev_(char *jobz, char *uplo, int *n, double complex *a, int *lda, double *w, double complex *work, int *lwork, double *rwork, int *info);

/** 
 * 
 * 
 * @param N 
 * @param M 
 * @param A 
 * @param a 
 * @author Takahiro Misawa (The University of Tokyo)
 * @author Kazuyoshi Yoshimi (The University of Tokyo)
 */
void to_f(int N, int M, double **A, double *a){
  int i,j,k;
  k=0;
  for(j=0;j<M;j++){
    for(i=0;i<N;i++){
      a[k] = A[i][j];
      k++;
    }
  }
}

/** 
 * 
 * 
 * @param xNsize 
 * @param A 
 * @param r 
 * 
 * @return 
 * @author Takahiro Misawa (The University of Tokyo)
 * @author Kazuyoshi Yoshimi (The University of Tokyo)
 */
int DSEVvalue(int xNsize, double **A, double *r){

  int k;
  char jobz, uplo;
  int n, lda, lwork, info;
  double *a, *w, *work;

  n = lda = xNsize;
  lwork = 4*xNsize; /* 3*xNsize OK?*/

  a = (double*)malloc(xNsize*xNsize*sizeof(double));
  w = (double*)malloc(xNsize*sizeof(double));
  work = (double*)malloc(lwork*sizeof(double));

  to_f(xNsize, xNsize, A, a);

  jobz = 'N';
  uplo = 'U';

  M_DSYEV(&jobz, &uplo, &n, a, &lda, w, work, &lwork, &info);

  if(info != 0){
    free(a);
    free(w);
    free(work);
    return 0;
  }

  for(k=0;k<xNsize;k++){
    r[k] = w[k];
  }

  free(a);
  free(w);
  free(work);

  return 1;
}

/** 
 * 
 * 
 * @param xNsize 
 * @param xM 
 * @param xIM 
 * 
 * @return 
 * @author Takahiro Misawa (The University of Tokyo)
 * @author Kazuyoshi Yoshimi (The University of Tokyo)
 */
int DInv(int xNsize, double **xM, double **xIM){

	int i,j,k;
	int m,n,lda,info,*piv,lwork;
	double *work;
	double *a;

	m=n=lda=lwork=xNsize;

	a = (double*)malloc(xNsize*xNsize*sizeof(double));
	work = (double*)malloc(xNsize*sizeof(double));
	piv = (int*)malloc(xNsize*sizeof(int));

	k=0;
	for(j=0;j<xNsize;j++){
		for(i=0;i<xNsize;i++){
			a[k] = xM[i][j];
			k++;
		}
	}

	dgetrf_(&m, &n, a, &lda, piv, &info);
	dgetri_(&n, a, &lda, piv, work, &lwork, &info);

	if(info != 0){
		free(a);
		free(work);
		free(piv);
		return 0;
	}

	for(k=0;k<xNsize*xNsize;k++){
		xIM[k%xNsize][k/xNsize] = a[k];
	}
	free(a);
	free(work);
	free(piv);

	return 1;
}

// added by Misawa 20090309 
// obtain eigen vectors 
/** 
 * 
 * 
 * @param xNsize 
 * @param A 
 * @param r 
 * @param vec 
 * 
 * @return 
 * @author Takahiro Misawa (The University of Tokyo)
 * @author Kazuyoshi Yoshimi (The University of Tokyo)
 */
int DSEVvector(int xNsize, double **A, double *r, double **vec ){

	int i,j,k;
	char jobz, uplo;
	int n, lda, lwork, info;
	double *a, *w, *work;

	n = lda = xNsize;
	lwork = 4*xNsize; /* 3*xNsize OK?*/

	a = (double*)malloc(xNsize*xNsize*sizeof(double));
	w = (double*)malloc(xNsize*sizeof(double));
	work = (double*)malloc(lwork*sizeof(double));

	k=0;
	for(j=0;j<xNsize;j++){
		for(i=0;i<xNsize;i++){
			a[k] = A[i][j];
			k++;
		}
	}

	jobz = 'V';
	uplo = 'U';

	dsyev_(&jobz, &uplo, &n, a, &lda, w, work, &lwork, &info);

	k=0;
	for(i=0;i<xNsize;i++){
		for(j=0;j<xNsize;j++){
			vec[i][j]=a[k];
			k++;
		}
	}

	if(info != 0){
		free(a);
		free(w);
		free(work);
		return 0;
	}

	for(k=0;k<xNsize;k++){
		r[k] = w[k];
	}

	free(a);
	free(w);
	free(work);

	return 1;
}


/* SX=rXとなるr,X (上からxNev個だけ求める 結果の格納箇所と書き換え箇所に注意)*/
/** 
 * 
 * 
 * @param xNsize 
 * @param A 
 * @param r 
 * @param X 
 * @param xNev 
 * 
 * @return 
 * @author Takahiro Misawa (The University of Tokyo)
 * @author Kazuyoshi Yoshimi (The University of Tokyo)
 */
int DSEVXU(int xNsize, double **A, double *r, double **X, int xNev){

	int i,j,k;
	char jobz, range, uplo;
	int n, lda, il, iu, m, ldz, lwork, *iwork, *ifail, info;
	double *a, *w, *work, vl, vu, abstol, *z;

	n = lda = ldz = xNsize;
	lwork = 8*xNsize;

	a = (double*)malloc(xNsize*xNsize*sizeof(double));
	w = (double*)malloc(xNsize*sizeof(double));
	z = (double*)malloc(xNsize*xNsize*sizeof(double));
	work  = (double*)malloc(lwork*sizeof(double));
	iwork = (int*)malloc(5*xNsize*sizeof(int));
	ifail = (int*)malloc(xNsize*sizeof(int));

	abstol = 2.0*dlamch_("S");
	vl = vu = 0.0;

	k=0;
	for(j=0;j<xNsize;j++){
		for(i=0;i<xNsize;i++){
			a[k] = A[i][j];
			k++;
		}
	}

	jobz  = 'V';
	range = 'I';
	uplo  = 'U';

	il = xNsize-xNev+1;
	iu = xNsize;
	m = iu-il+1;

	dsyevx_(&jobz, &range, &uplo, &n, a, &lda, &vl, &vu, &il, &iu, &abstol, 
			&m, w, z, &ldz, work, &lwork, iwork, ifail, &info);

	if(info != 0){
		free(a);
		free(w);
		free(z);
		free(work);
		free(iwork);
		free(ifail);
		return 0;
	}

	for(k=0;k<xNev;k++){
		r[k+xNsize-xNev] = w[k];
	}

	for(k=0;k<xNsize*xNev;k++){
		X[k%xNsize][k/xNsize+xNsize-xNev] = z[k];
	}
	free(a);
	free(w);
	free(z);
	free(work);
	free(iwork);
	free(ifail);

	return 1;
}

//added by Misawa 130121
//For complex Hermite matrix
/** 
 * 
 * 
 * @param xNsize 
 * @param A 
 * @param r 
 * @param vec 
 * 
 * @return 
 * @author Takahiro Misawa (The University of Tokyo)
 * @author Kazuyoshi Yoshimi (The University of Tokyo)
 */
int ZHEEVall(int xNsize, double complex **A, double complex *r,double complex **vec){

	int i,j,k;
	char jobz, uplo;
	int n, lda, lwork, info;
        double *rwork;
	double *w;
	double complex *a, *work;

	n = lda = xNsize;
	lwork = 4*xNsize; /* 3*xNsize OK?*/

	a = (double complex*)malloc(xNsize*xNsize*sizeof(double complex));
	w = (double*)malloc(xNsize*sizeof(double));
	work = (double complex*)malloc(lwork*sizeof(double complex));
	rwork = (double*)malloc(lwork*sizeof(double));

	k=0;
	for(j=0;j<xNsize;j++){
		for(i=0;i<xNsize;i++){
			a[k] = A[i][j];
			k++;
		}
	}

	jobz = 'V';
	uplo = 'U';

	zheev_(&jobz, &uplo, &n, a, &lda, w, work, &lwork, rwork, &info);

	if(info != 0){
		free(a);
		free(w);
		free(work);
		free(rwork);
		return 0;
	}

	k=0;
	for(i=0;i<xNsize;i++){
		for(j=0;j<xNsize;j++){
			vec[i][j]=a[k];
			k++;
		}
	}

  
	for(k=0;k<xNsize;k++){
		r[k] = w[k];
	}

	free(a);
	free(w);
	free(work);
	free(rwork);

	return 1;
}


