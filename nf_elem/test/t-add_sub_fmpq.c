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

    Copyright (C) 2020 Vincent Delecroix

******************************************************************************/

#include <stdio.h>
#include "e-antic/nf.h"
#include "e-antic/nf_elem.h"

int
main(void)
{
    int i, result;
    flint_rand_t state;

    flint_randinit(state);

    /* test b + c - c = b */
    for (i = 0; i < 100; i++)
    {
        nf_t nf;
        nf_elem_t a, b, t;
        fmpq_t c;

        nf_init_randtest(nf, state, 40, 200);

        nf_elem_init(a, nf);
        nf_elem_init(b, nf);
        nf_elem_init(t, nf);
        fmpq_init(c);

        nf_elem_randtest(b, state, 200, nf);
        fmpq_randtest(c, state, 200);

        nf_elem_add_fmpq(t, b, c, nf);
        nf_elem_sub_fmpq(a, t, c, nf);

        if (!nf_elem_equal(a, b, nf))
        {
           printf("FAIL:\n");
           printf("nf = "); nf_print(nf); printf("\n");
           printf("a = "); nf_elem_print_pretty(a, nf, "x"); printf("\n");
           printf("b = "); nf_elem_print_pretty(b, nf, "x"); printf("\n");
           printf("c = "); fmpq_print(c); printf("\n");
           abort();
        }

        nf_elem_clear(a, nf);
        nf_elem_clear(b, nf);
        nf_elem_clear(t, nf);
        fmpq_clear(c);

        nf_clear(nf);
    }

    /* test aliasing a and b */
    for (i = 0; i < 100; i++)
    {
        nf_t nf;
        nf_elem_t a, b;
        fmpq_t c;

        nf_init_randtest(nf, state, 40, 200);

        nf_elem_init(a, nf);
        nf_elem_init(b, nf);
        fmpq_init(c);

        nf_elem_randtest(b, state, 200, nf);
        fmpq_randtest(c, state, 200);

        nf_elem_set(a, b, nf);
        nf_elem_add_fmpq(b, b, c, nf);
        nf_elem_sub_fmpq(b, b, c, nf);

        if (!nf_elem_equal(a, b, nf))
        {
           printf("FAIL:\n");
           printf("(with aliasing)\n");
           printf("nf = "); nf_print(nf); printf("\n");
           printf("a = "); nf_elem_print_pretty(a, nf, "x"); printf("\n");
           printf("b = "); nf_elem_print_pretty(b, nf, "x"); printf("\n");
           printf("c = "); fmpq_print(c); printf("\n");
           abort();
        }

        nf_elem_clear(a, nf);
        nf_elem_clear(b, nf);
        fmpq_clear(c);

        nf_clear(nf);
    }

    flint_randclear(state);

    return 0;
}
