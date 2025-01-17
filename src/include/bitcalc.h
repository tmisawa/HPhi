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
#include "Common.h"

//inline int GetSplitBit(
int GetSplitBit(
		const int Nsite,
		long unsigned int *irght,
		long unsigned int *ilft,
		long unsigned int *ihfbit
		);

//inline int GetSplitBitByModel(
int GetSplitBitByModel(
		       const int Nsite,
		       const int iCalcModel,
		       long unsigned int *irght,
		       long unsigned int *ilft,
		       long unsigned int *ihfbit
		       );

//inline void SplitBit(
void SplitBit(
		  const long unsigned int ibit,
		  const long unsigned int irght,
		  const long unsigned int ilft,
		  const long unsigned int ihfbit,
		  long unsigned int *isplited_Bit_right,
		  long unsigned int *isplited_Bit_left
	      );

//inline void GetOffComp(
void GetOffComp(
	        long unsigned int *_list_2_1,
	        long unsigned int *_list_2_2,
		const long unsigned int _ibit,
		const long unsigned int _irght,
		const long unsigned int _ilft,
		const long unsigned int _ihfbit,
		long unsigned int *_ioffComp
		);

void SgnBit_old(
		  const long unsigned int bit,
                  int *sgn
);

void SgnBit(
		  const long unsigned int bit,
                  int *sgn
);

int BitCheck( 
	     const long unsigned int org_bit,
	     const long unsigned int target_bit
);
