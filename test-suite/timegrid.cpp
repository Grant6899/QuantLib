/* -*- mode: c++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

/*
 Copyright (C) 2005 StatPro Italia srl

 This file is part of QuantLib, a free-software/open-source library
 for financial quantitative analysts and developers - http://quantlib.org/

 QuantLib is free software: you can redistribute it and/or modify it
 under the terms of the QuantLib license.  You should have received a
 copy of the license along with this program; if not, please email
 <quantlib-dev@lists.sf.net>. The license is also available online at
 <http://quantlib.org/license.shtml>.

 This program is distributed in the hope that it will be useful, but WITHOUT
 ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 FOR A PARTICULAR PURPOSE.  See the license for more details.
*/

#include "timegrid.hpp"
#include "utilities.hpp"
#include <ql/timegrid.hpp>
#include <ql/utilities/dataformatters.hpp>

using namespace QuantLib;
using namespace boost::unit_test_framework;

class FSquared : std::unary_function<Real,Real> {
  public:
    Real operator()(Real x) const { return x*x; }
};

void TimeGridTest::testConstruction() {

    BOOST_TEST_MESSAGE("Testing timegrid construction...");

    // empty timegrid
    TimeGrid tg1;
    if (!tg1.empty())
        BOOST_ERROR("default-initialized timegrid is not empty "
                    "(size = " << tg1.size() << ")");

    // sized timegrid
    Size steps = 10;
    Time end = 10.0;
    TimeGrid tg2(end, steps);
    if (tg2.size() != steps + 1)
        BOOST_ERROR("timegrid not of the required size"
                    << "\n    required:  " << steps + 1
                    << "\n    resulting: " << tg2.size());
    
    if (tg2.back() != end)
        BOOST_ERROR("timegrid end not set correctly"
                    << "\n    required:  " << end
                    << "\n    resulting: " << tg2.back());

    // Time grid with mandatory time points
    std::vector<Time> mandatoryTime;
    mandatoryTime.push_back(0.0);
    mandatoryTime.push_back(1.0);
    mandatoryTime.push_back(2.0);
    mandatoryTime.push_back(3.0);
    mandatoryTime.push_back(4.0);
    TimeGrid tg3(mandatoryTime.begin(), mandatoryTime.end());
    if (tg3.size() != mandatoryTime.size())
        BOOST_ERROR("timegrid not of the required size"
                    << "\n    required:  " << mandatoryTime.size()
                    << "\n    resulting: " << tg3.size());
    
    TimeGrid tg4(mandatoryTime.begin(), mandatoryTime.end(), steps);
    if (tg4.size() != steps)
        BOOST_ERROR("timegrid not of the required size"
                    << "\n    required:  " << steps 
                    << "\n    resulting: " << tg4.size());
}
/*
void TimeGridTest::testTimeGridFunctions() {

    BOOST_TEST_MESSAGE("Testing timegrid functions...");
    
    TimeGrid tg(10, 10);

    Array a(5);
    for (Size i=0; i < a.size(); ++i) {
        a[i] = std::sin(Real(i))+1.1;
    }

    const Real exponential = -2.3;
    const Array p = Pow(a, exponential);
    const Array e = Exp(a);
    const Array l = Log(a);
    const Array s = Sqrt(a);

    const Real tol = 10*QL_EPSILON;
    for (Size i=0; i < a.size(); ++i) {
        if (std::fabs(p[i]-std::pow(a[i], exponential)) > tol) {
            BOOST_FAIL("Array function test Pow failed");
        }
        if (std::fabs(e[i]-std::exp(a[i])) > tol) {
            BOOST_FAIL("Array function test Exp failed");
        }
        if (std::fabs(l[i]-std::log(a[i])) > tol) {
            BOOST_FAIL("Array function test Log failed");
        }
        if (std::fabs(s[i]-std::sqrt(a[i])) > tol) {
            BOOST_FAIL("Array function test Sqrt failed");
        }
    }
}
/*
void ArrayTest::testArrayResize() {
    BOOST_TEST_MESSAGE("Testing array resize...");

    Array a(10,1.0,1.0);

    for (Size i=0; i < 10; ++i)
        BOOST_CHECK_CLOSE(a[i], Real(1+i), 10*QL_EPSILON);

    a.resize(5);
    BOOST_CHECK(a.size() == 5);

    for (Size i=0; i < 5; ++i)
        BOOST_CHECK_CLOSE(a[i], Real(1+i), 10*QL_EPSILON);

    a.resize(15);
    BOOST_CHECK(a.size() == 15);

    for (Size i=0; i < 5; ++i)
        BOOST_CHECK_CLOSE(a[i], Real(1+i), 10*QL_EPSILON);

    const Array::const_iterator iter = a.begin();
    a.resize(a.size());
    BOOST_CHECK(iter == a.begin());

    a.resize(10);
    BOOST_CHECK(a.size() == 10);
    BOOST_CHECK(iter == a.begin());
}

*/
test_suite* TimeGridTest::suite() {
    test_suite* suite = BOOST_TEST_SUITE("timegrid tests");
    suite->add(QUANTLIB_TEST_CASE(&TimeGridTest::testConstruction));
    //suite->add(QUANTLIB_TEST_CASE(&ArrayTest::testTimeGridFunctions));
    //suite->add(QUANTLIB_TEST_CASE(&ArrayTest::testArrayResize));
    return suite;
}

