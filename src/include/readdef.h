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
#include "xsetmem.h"
#define D_FileNameMaxReadDef 256 /*!<  Max length of words for file name*/
#define D_CharTmpReadDef     200 /*!<  Max length of reading words from input files*/
#define D_iKWNumDef 13 /*!<  Max number of keyword set in def file*/
#define D_CharKWDMAX     200 /*!<  Max length of words for keyword*/

/*!< Number of ignore lines in def files */
#define IgnoreLinesInDef 5

/*!< Mode of define files.*/
#define EXPERT_MODE 0
#define STANDARD_MODE 1
#define STANDARD_DRY_MODE 2

/**
 * Number of Keyword List in NameListFile for this prrogram.  
 **/
#define KWCalcMod 0
#define KWModPara 1
#define KWLocSpin 2
#define KWTrans 3
#define KWCoulombIntra 4
#define KWCoulombInter 5
#define KWHund 6
#define KWPairHop 7
#define KWExchange 8
#define KWInterAll 9
#define KWOneBodyG 10
#define KWTwoBodyG 11
#define KWPairLift 12

/**
 * Keyword List in NameListFile.  
 **/
static char cKWListOfFileNameList[D_iKWNumDef][D_CharTmpReadDef]={
  "CalcMod",
  "ModPara",
  "LocSpin",
  "Trans",
  "CoulombIntra",
  "CoulombInter",
  "Hund",
  "PairHop",
  "Exchange",
  "InterAll",
  "OneBodyG",
  "TwoBodyG",
  "PairLift"
};

/**
 * File Name List in NameListFile.  
 **/
static char cFileNameListFile[D_iKWNumDef][D_CharTmpReadDef];



int CheckSite(
	      const int iListToSite,
	      const int iMaxNum
	      );


int CheckPairSite(
	      const int iList1ToSite,
	      const int iList2ToSite,
	      const int iMaxNum
	      );


int CheckQuadSite(
	      const int iList1ToSite,
	      const int iList2ToSite,
	      const int iList3ToSite,
	      const int iList4ToSite,
	      const int iMaxNum
	      );

int CheckTransferHermite
(
 const struct DefineList *X
);

int CheckInterAllHermite
(
 const struct DefineList *X
);

int GetDiagonalInterAll
(
 struct DefineList *X
 );

int JudgeDefType
(
 const int argc,
 char *argv[],
 int *mode
 );

int CheckFormatForSpinInt
(
 const int site1,
 const int site2,
 const int site3,
 const int site4
 );

int CheckFormatForKondoInt
(
 struct DefineList *X
 );

int CheckFormatForKondoTrans
(
 struct DefineList *X
 );

void SetConvergenceFactor
(
 struct DefineList *X
 );
