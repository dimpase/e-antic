/*=============================================================================

    This file is part of FLINT.

    FLINT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    FLINT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with FLINT; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA

=============================================================================*/
/******************************************************************************

    Copyright (C) 2015 William Hart

******************************************************************************/

#include "e-antic/nf_elem.h"

void nf_elem_scalar_mul_si(nf_elem_t a, const nf_elem_t b, 
                                                        slong c, const nf_t nf)
{
   if (nf->flag & NF_LINEAR)
   {
      fmpz * den = LNF_ELEM_DENREF(a);
	  fmpz * num = LNF_ELEM_NUMREF(a);
	  const fmpz * const den2 = LNF_ELEM_DENREF(b);
	  const fmpz * const num2 = LNF_ELEM_NUMREF(b);
	  
      fmpz_mul_si(num, num2, c);
      fmpz_set(den, den2);
	  _fmpq_canonicalise(num, den);
   }
   else if (nf->flag & NF_QUADRATIC)
   {
      fmpz * den = QNF_ELEM_DENREF(a);
	  fmpz * num = QNF_ELEM_NUMREF(a);
	  const fmpz * const den2 = QNF_ELEM_DENREF(b);
	  const fmpz * const num2 = QNF_ELEM_NUMREF(b);
	  
      _fmpz_vec_scalar_mul_si(num, num2, 2, c);
	  fmpz_set(den, den2);
	  _fmpq_poly_canonicalise(num, den, 2);
   } else
   {
      fmpq_poly_scalar_mul_si(NF_ELEM(a), NF_ELEM(b), c);
   }

}
