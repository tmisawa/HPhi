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
#include "bitcalc.h"

/** 
 * 
 * 
 * @param Nsite 
 * @param irght 
 * @param ilft 
 * @param ihfbit 
 * 
 * @author Takahiro Misawa (The University of Tokyo) 
 * @author Kazuyoshi Yoshimi (The University of Tokyo) 
 * @return 
 */
int GetSplitBit(
		const int Nsite,
		long unsigned int *irght,
		long unsigned int *ilft,
		long unsigned int *ihfbit
){
  if(Nsite<1){
    printf("%s", cErrSiteNumber);
    return -1;
  }
  *irght=pow(2,((Nsite+1)/2))-1;
  *ilft=pow(2,Nsite)-1;
  *ilft=*ilft ^ *irght; 
  *ihfbit=pow(2,((Nsite+1)/2));
  return 0;
}

/** 
 * 
 * 
 * @param Nsite 
 * @param iCalcModel 
 * @param irght 
 * @param ilft 
 * @param ihfbit 
 * 
 * @author Takahiro Misawa (The University of Tokyo) 
 * @author Kazuyoshi Yoshimi (The University of Tokyo) 
 * @return 
 */
int GetSplitBitByModel(
		       const int Nsite,
		       const int iCalcModel,
		       long unsigned int *irght,
		       long unsigned int *ilft,
		       long unsigned int *ihfbit
		       )
{
  int tmpNsite=Nsite;
  switch(iCalcModel){    
  case HubbardGC:
  case KondoGC:
  case Hubbard:
  case Kondo:
    tmpNsite *= 2;
    break;
  case Spin:
  case SpinGC:   
    break;
  default:
    printf(cErrNoModel, iCalcModel);
    return -1;
  }

  if(!GetSplitBit(tmpNsite, irght, ilft, ihfbit)==0){
    return -1;
  }
  
  return 0;
}

/** 
 * 
 * 
 * @param ibit 
 * @param irght 
 * @param ilft 
 * @param ihfbit 
 * @param isplited_Bit_right 
 * @param isplited_Bit_left 
 * @author Takahiro Misawa (The University of Tokyo) 
 * @author Kazuyoshi Yoshimi (The University of Tokyo) 
 */
void SplitBit(
		  const long unsigned int ibit,
		  const long unsigned int irght,
		  const long unsigned int ilft,
		  const long unsigned int ihfbit,
		  long unsigned int *isplited_Bit_right,
		  long unsigned int *isplited_Bit_left
)
{
  *isplited_Bit_right=ibit & irght;
  *isplited_Bit_left=ibit & ilft;
  *isplited_Bit_left=*isplited_Bit_left/ihfbit;
}

/** 
 * 
 * 
 * @param _list_2_1 
 * @param _list_2_2 
 * @param _ibit 
 * @param _irght 
 * @param _ilft 
 * @param _ihfbit 
 * @param _ioffComp 
 * @author Takahiro Misawa (The University of Tokyo) 
 * @author Kazuyoshi Yoshimi (The University of Tokyo) 
 */
void GetOffComp(
	        long unsigned int *_list_2_1,
	        long unsigned int *_list_2_2,
		const long unsigned int _ibit,
		const long unsigned int _irght,
		const long unsigned int _ilft,
		const long unsigned int _ihfbit,
		long unsigned int *_ioffComp
)
{
  long unsigned int ia, ib;
  SplitBit(_ibit, _irght, _ilft, _ihfbit, &ia, &ib);
  *_ioffComp =_list_2_1[ia];
  *_ioffComp+=_list_2_2[ib];
}

// this function only used for 32bit
// how to modify for 64 bit calculations ? 
/** 
 * 
 * 
 * @param org_bit 
 * @param sgn 
 * @author Takahiro Misawa (The University of Tokyo) 
 * @author Kazuyoshi Yoshimi (The University of Tokyo) 
 */
void SgnBit_old( 
		  const long unsigned int org_bit,
                  int *sgn
)
{
   long unsigned int bit;

   bit     = org_bit^(org_bit>>1);
   bit     = (bit^(bit>>2) ) & 0x11111111;
   bit     = bit*0x11111111;
   *sgn    = 1-2*((bit>>28) & 1); // sgn = pm 1
}


// for 64 bit
/** 
 * 
 * 
 * @param org_bit 
 * @param sgn 
 *
 * @author Takahiro Misawa (The University of Tokyo) 
 * @author Kazuyoshi Yoshimi (The University of Tokyo) 
 */
void SgnBit( 
		  const long unsigned int org_bit,
                  int *sgn
)
{
   long unsigned int bit;

   bit =  org_bit^(org_bit>>1);
   bit =  bit^(bit>>2);
   bit =  bit^(bit>>4);
   bit =  bit^(bit>>8);
   bit =  bit^(bit>>16);
   bit =  bit^(bit>>32);
   *sgn    = 1-2*(bit & 1); // sgn = pm 1
}

// bit check
/** 
 * 
 * 
 * @param org_bit 
 * @param target_bit 
 * 
 * @author Takahiro Misawa (The University of Tokyo) 
 * @author Kazuyoshi Yoshimi (The University of Tokyo) 
 * @return 
 */
int BitCheck( 
	     const long unsigned int org_bit,
	     const long unsigned int target_bit
)
{
   return  (org_bit >> target_bit) &1;
   // (org_bit & (2^target_bit))/2^target_bit
}
