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

    Copyright (C) 2018 Vincent Delecroix

******************************************************************************/

#include <stdio.h>
#include <flint/flint.h>
#include <flint/fmpq_poly.h>
#include "e-antic/nf.h"
#include "e-antic/nf_elem.h"

int main(void)
{
    int i;
    flint_rand_t state;

    flint_printf("set_si_ui...");
    fflush(stdout);

    flint_randinit(state);

    for (i = 0; i < 1000; i++)
    {
        fmpz_t z;
        fmpq_t q;
        fmpq_poly_t f;

        nf_t nf;
        nf_elem_t a;

        fmpq_init(q);
        fmpz_init(z);

        nf_init_randtest(nf, state, 20, 200);
        nf_elem_init(a, nf);

        fmpq_poly_init(f);

        fmpq_poly_randtest(f, state, nf_degree(nf) - 1, 200);
        nf_elem_set_fmpq_poly(a, f, nf);

        fmpq_poly_get_coeff_fmpq(q, f, 0);
        if (nf_elem_equal_fmpq(a, q, nf) != (fmpq_poly_length(f) <= 1))
        {
                flint_printf("nf_elem_equal_fmpq wrong\n");
                flint_printf("nf = "); nf_print(nf); flint_printf("\n");
                flint_printf("f = "); fmpq_poly_print_pretty(f, "x"); flint_printf("\n");
                flint_printf("a = "); nf_elem_print_pretty(a, nf, "x"); flint_printf("\n");
                flint_printf("q = "); fmpq_print(q); flint_printf("\n");
                abort();
        }

        fmpz_set(z, fmpq_numref(q));
        if (nf_elem_equal_fmpz(a, z, nf) != (fmpq_poly_length(f) <= 1 && fmpz_is_one(fmpq_denref(q))))
        {
                flint_printf("nf_elem_equal_fmpz wrong\n");
                flint_printf("nf = "); nf_print(nf); flint_printf("\n");
                flint_printf("f = "); fmpq_poly_print_pretty(f, "x"); flint_printf("\n");
                flint_printf("a = "); nf_elem_print_pretty(a, nf, "x"); flint_printf("\n");
                flint_printf("z = "); fmpz_print(z); flint_printf("\n");
                abort();
        }

        fmpq_add_si(q, q, 1);
        if (nf_elem_equal_fmpq(a, q, nf))
        {
                flint_printf("nf_elem_equal_fmpq wrong\n");
                flint_printf("nf = "); nf_print(nf); flint_printf("\n");
                flint_printf("f = "); fmpq_poly_print_pretty(f, "x"); flint_printf("\n");
                flint_printf("a = "); nf_elem_print_pretty(a, nf, "x"); flint_printf("\n");
                flint_printf("q = "); fmpq_print(q); flint_printf("\n");
                abort();
        }

        fmpz_add_ui(z, z, 1);
        if (nf_elem_equal_fmpz(a, z, nf))
        {
                flint_printf("nf_elem_equal_fmpz wrong\n");
                flint_printf("nf = "); nf_print(nf); flint_printf("\n");
                flint_printf("f = "); fmpq_poly_print_pretty(f, "x"); flint_printf("\n");
                flint_printf("a = "); nf_elem_print_pretty(a, nf, "x"); flint_printf("\n");
                flint_printf("z = "); fmpz_print(z); flint_printf("\n");
                abort();
        }

        fmpz_clear(z);
        fmpq_clear(q);
        fmpq_poly_clear(f);
        nf_elem_clear(a, nf);
        nf_clear(nf);
    }

    flint_randclear(state);
    flint_printf("PASS\n");
    return 0;
}
