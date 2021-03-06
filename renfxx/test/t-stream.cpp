/*
    Copyright (C) 2018 Vincent Delecroix

    This file is part of e-antic

    e-antic is free software: you can redistribute it and/or modify it under
    the terms of the GNU Lesser General Public License (LGPL) as published
    by the Free Software Foundation; either version 3.0 of the License, or
    (at your option) any later version.  See <http://www.gnu.org/licenses/>.
*/

#include "e-antic/renfxx.h"

#include <string>
#include <iostream>
#include <sstream>
#include <regex>

using namespace eantic;
using std::make_shared;

int main(void)
{
    auto K1 = renf_class::make("A^3 - 2", "A", "1.25 +/- 0.1");
    auto K2 = renf_class::make("2*abc^4 - 5*abc + 1", "abc", "0.2 +/- 0.1");

    std::string numerical_noise = "\\d+ \\+\\/\\- \\d+\\.\\d+\\e\\-\\d+\\]\\)";

    {
        std::stringstream s;
        s << *K1;
        if (! std::regex_match(s.str(), std::regex("NumberField\\(A\\^3 \\- 2, \\[1.2599210498948731" + numerical_noise)))
            throw std::runtime_error("wrong K1 string, got " + s.str());
    }

    {
        std::stringstream s;
        s << *K2;
        if (! std::regex_match(s.str(), std::regex("NumberField\\(2\\*abc\\^4 \\- 5\\*abc \\+ 1, \\[0.2006483391818005" + numerical_noise)))
            throw std::runtime_error("wrong K2 string, got " + s.str());
    }

    const renf_elem_class g1 = K1->gen();
    const renf_elem_class g2 = K2->gen();

    {
        std::stringstream s;
        renf_elem_class a(mpz_class("0"));
        s << a;
        if (s.str() != "0")
            throw std::runtime_error("wrong 0 string, got " + s.str());
    }

    {
        std::stringstream s;
        renf_elem_class a(mpq_class("2/3"));
        s << a;
        if (s.str() != "(2/3 ~ 0.666667)")
            throw std::runtime_error("wrong 0 string, got " + s.str());

        renf_elem_class b;
        s >> b;
        if (a != b)
            throw std::runtime_error("could not recover original element");
    }

    {
        std::stringstream s;
        renf_elem_class a = K1->gen();
        s << a;
        if (s.str() != "(A ~ 1.2599210)")
            throw std::runtime_error("wrong string, got " + s.str());
    }

    {
        renf_elem_class a(K1);

        std::stringstream sin1("3/5*A+2");
        K1->set_pword(sin1);
        sin1 >> a;
        if (a != 3*g1/5 + 2)
            throw std::runtime_error("wrong nf initialization");

        std::stringstream sin2("A-1");
        K1->set_pword(sin2);
        sin2 >> a;
        if (a != g1 - 1)
            throw std::runtime_error("wrong nf reinitialization");
    }

    {
        // initializing as a rational number
        renf_elem_class a;
        std::stringstream sin("3/5");
        sin >> a;

        if (5*a != 3)
            throw std::runtime_error("bad rational initialization");
    }

    {
        // initialization via istream_set_renf
        renf_elem_class a;

        std::stringstream sin1("3*A^2-7");
        K1->set_istream(sin1) >> a;
        if (a != 3*g1*g1-7)
            throw std::runtime_error("bad stream_set_renf initialization");

        std::stringstream sin2("A+1");
        K1->set_istream(sin2) >> a;
        if (a != g1+1)
            throw std::runtime_error("bad stream_set_renf reinitialization 1");

        std::stringstream sin3("abc^2-1");
        K2->set_istream(sin3) >> a;
        if (a != g2*g2 - 1)
            throw std::runtime_error("bad stream_set_renf reinitialization 2");
    }

    {
        // use the result of writing in a stream <<
        renf_elem_class a(K1, "3*A^2-1");
        renf_elem_class b(K1);
        std::stringstream s;

        s << a;
        K1->set_pword(s);
        s >> b;

        if (a != b)
            throw std::runtime_error("not able to reconstruct input from output");
    }

    return 0;
}
