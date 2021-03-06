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
        fmpq_poly_t f;
        ulong m, n;
        slong sm, sn;
        int f_is_m;

        nf_t nf;
        nf_elem_t a;
        nf_elem_t b;

        nf_init_randtest(nf, state, 20, 200);
        nf_elem_init(a, nf);
        nf_elem_init(b, nf);

        fmpq_poly_init(f);

        /* unsigned words */
        m = n_randtest(state);
        n = n_randtest(state);
        nf_elem_set_ui(a, m, nf);
        nf_elem_set_ui(b, n, nf);

        if (!nf_elem_equal_ui(a, m, nf) ||
            !nf_elem_equal_ui(b, n, nf) ||
            nf_elem_equal_ui(a, n, nf) != (m == n) ||
            nf_elem_equal_ui(b, m, nf) != (m == n))
        {
            flint_printf("set_ui/equal_ui wrong with\n");
            flint_printf("nf = "); nf_print(nf); flint_printf("\n");
            flint_printf("m = %wu\n", m);
            flint_printf("n = %wu\n", n);
            flint_printf("a = "); nf_elem_print_pretty(a, nf, "x"); printf("\n");
            flint_printf("b = "); nf_elem_print_pretty(b, nf, "x"); printf("\n");
            abort();
        }

        fmpq_poly_randtest(f, state, nf_degree(nf) - 1, 200);
        nf_elem_set_fmpq_poly(a, f, nf);

        if (fmpq_poly_length(f) == 0)
            f_is_m = m == 0;
        else
            f_is_m = fmpq_poly_length(f) == 1 &&
                     fmpz_equal_ui(fmpq_poly_numref(f), m) &&
                     fmpz_is_one(fmpq_poly_denref(f));

        if (nf_elem_equal_ui(a, m, nf) != f_is_m)
        {
            flint_printf("equal_ui wrong with\n");
            flint_printf("nf = "); nf_print(nf); flint_printf("\n");
            flint_printf("m = %wu\n", m);
            flint_printf("f = "); fmpq_poly_print_pretty(f, "x"); flint_printf("\n");
            flint_printf("a = "); nf_elem_print_pretty(a, nf, "x"); flint_printf("\n");
            abort();
        }

        /* with signed words */
        sm = (slong) m;
        sn = (slong) n;

        nf_elem_set_si(a, sm, nf);
        nf_elem_set_si(b, sn, nf);

        if (!nf_elem_equal_si(a, sm, nf) ||
            !nf_elem_equal_si(b, sn, nf) ||
            nf_elem_equal_si(a, sn, nf) != (m == n) ||
            nf_elem_equal_si(b, sm, nf) != (m == n))
        {
            flint_printf("set_si/equal_si wrong with\n");
            flint_printf("nf = "); nf_print(nf); flint_printf("\n");
            flint_printf("sm = %wd\n", sm);
            flint_printf("sn = %wd\n", sn);
            flint_printf("a = "); nf_elem_print_pretty(a, nf, "x"); flint_printf("\n");
            flint_printf("b = "); nf_elem_print_pretty(b, nf, "x"); flint_printf("\n");
            abort();
        }

        nf_elem_set_fmpq_poly(a, f, nf);
        if (sm == 0)
            f_is_m = fmpq_poly_length(f) == 0;
        else
            f_is_m = fmpq_poly_length(f) == 1 &&
                     fmpz_equal_si(fmpq_poly_numref(f), sm) &&
                     fmpz_is_one(fmpq_poly_denref(f));

        if (nf_elem_equal_si(a, sm, nf) != f_is_m)
        {
            flint_printf("equal_si wrong with\n");
            flint_printf("nf = "); nf_print(nf); flint_printf("\n");
            flint_printf("sm = %wd\n", sm);
            flint_printf("f = "); fmpq_poly_print_pretty(f, "x"); flint_printf("\n");
            flint_printf("a = "); nf_elem_print_pretty(a, nf, "x"); flint_printf("\n");
            abort();
        }

        /* cleaning */

        fmpq_poly_clear(f);
        nf_elem_clear(a, nf);
        nf_elem_clear(b, nf);
        nf_clear(nf);
    }

    flint_randclear(state);
    flint_printf("PASS\n");
    return 0;
}
