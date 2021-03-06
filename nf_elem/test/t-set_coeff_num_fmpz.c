/*=============================================================================

    This file is part of ANTIC.

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

    Copyright (C) 2018 Tommy Hofmann

******************************************************************************/

#include <stdio.h>
#include "e-antic/nf.h"
#include "e-antic/nf_elem.h"

int
main(void)
{
    int i, result;
    flint_rand_t state;

    flint_printf("set_coeff_num_fmpz....");
    fflush(stdout);

    flint_randinit(state);

    for (i = 0; i < 1000; i++)
    {
        nf_t nf;
        nf_elem_t a, b;
        fmpz_t d, d2;
        slong coeff;
        fmpq_t newcoeff, tempcoeff;

        nf_init_randtest(nf, state, 40, 200);

        fmpz_init(d);
        fmpz_init(d2);
        fmpq_init(tempcoeff);
        fmpq_init(newcoeff);

        nf_elem_init(a, nf);
        nf_elem_init(b, nf);
        nf_elem_randtest(a, state, 200, nf);
        nf_elem_set(b, a, nf);

        coeff = (slong) n_randint(state, nf_degree(nf) + 1);
        
        fmpz_randtest(d, state, 200);

        nf_elem_get_den(fmpq_denref(tempcoeff), a, nf);
        fmpz_set(fmpq_numref(tempcoeff), d);
        fmpq_canonicalise(tempcoeff);

        _nf_elem_set_coeff_num_fmpz(a, coeff, d, nf);
        nf_elem_get_coeff_fmpq(newcoeff, a, coeff, nf);

        result = fmpq_equal(newcoeff, tempcoeff);
        if (!result)
        {
            flint_printf("FAIL:\n");
            flint_printf("a = "); nf_elem_print_pretty(a, nf, "x"); printf("\n");
            flint_printf("b = "); nf_elem_print_pretty(b, nf, "x"); printf("\n");
            flint_printf("coeff = %u\n", coeff);
            flint_printf("d = "); fmpz_print(d); printf("\n");
            flint_printf("newcoeff = "); fmpq_print(newcoeff); printf("\n");
            flint_printf("pol = "); fmpq_poly_print_pretty(nf->pol, "x"); printf("\n");
            abort();
        }

        nf_elem_clear(a, nf);
        nf_elem_clear(b, nf);
        
        nf_clear(nf);

        fmpz_clear(d);
        fmpz_clear(d2);

        fmpq_clear(tempcoeff);
        fmpq_clear(newcoeff);
    }

    flint_randclear(state);
    flint_printf("PASS\n");
    return 0;
}
