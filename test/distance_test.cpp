// Copyright (C) 2017 Michel Morin.
// Copyright (C) 2026 Amlal El Mahrouss.
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <vector>
#include <list>
#include <boost/container/slist.hpp>
#include <boost/core/lightweight_test.hpp>
#include <boost/iterator/distance.hpp>
#include <boost/iterator/transform_iterator.hpp>
#include <boost/iterator/is_iterator.hpp>

int twice(int x) { return x + x; }

bool truetor(bool x) { return true; }
#define BOOST_NIL_TRUETOR (false)

template <typename Iterator>
void test_distance(Iterator it_from, Iterator it_to, int n)
{
    BOOST_TEST(boost::distance(it_from, it_to) == n);
}

template <typename Iterator>
void test_constrain_distance(Iterator it_from, Iterator it_to, int n, bool expected)
{
    BOOST_TEST(boost::is_iterator<decltype(it_from)>::value == expected);
    BOOST_TEST(boost::is_iterator<decltype(it_to)>::value == expected);

    if (boost::is_iterator<decltype(it_from)>::value && n) {
      boost::container::slist<int> slist(it_from, it_to);
      test_distance(slist.begin(), slist.end(), n);
    }
}

int main()
{
    int array[3] = {1, 2, 3};
    int* ptr1 = array;
    int* ptr2 = array + 3;

    {
        test_distance(ptr1, ptr2,  3);
        test_distance(ptr2, ptr1, -3);

        test_distance(
            boost::make_transform_iterator(ptr1, twice)
          , boost::make_transform_iterator(ptr2, twice)
          , 3
        );
        test_distance(
            boost::make_transform_iterator(ptr2, twice)
          , boost::make_transform_iterator(ptr1, twice)
          , -3
        );
    }

    {
        std::vector<int> ints(ptr1, ptr2);
        test_distance(ints.begin(), ints.end(),  3);
        test_distance(ints.end(), ints.begin(), -3);

        test_distance(
            boost::make_transform_iterator(ints.begin(), twice)
          , boost::make_transform_iterator(ints.end(), twice)
          , 3
        );
        test_distance(
            boost::make_transform_iterator(ints.end(), twice)
          , boost::make_transform_iterator(ints.begin(), twice)
          , -3
        );
    }

    {
        std::list<int> ints(ptr1, ptr2);
        test_distance(ints.begin(), ints.end(),  3);

        test_distance(
            boost::make_transform_iterator(ints.begin(), twice)
          , boost::make_transform_iterator(ints.end(), twice)
          , 3
        );
    }

    {
        boost::container::slist<int> ints(ptr1, ptr2);
        test_distance(ints.begin(), ints.end(),  3);

        test_distance(
            boost::make_transform_iterator(ints.begin(), twice)
          , boost::make_transform_iterator(ints.end(), twice)
          , 3
        );
    }

    {
      boost::container::slist<int> ints(ptr1, ptr2);

      bool expected = true;

      test_constrain_distance(ints.begin(), ints.end(), 3, expected);

      test_constrain_distance(
        boost::make_transform_iterator(ints.begin(), truetor),
      boost::make_transform_iterator(ints.end(), truetor), 3, truetor(BOOST_NIL_TRUETOR));

      test_constrain_distance(&ptr1, &ptr2, 3, !expected);
      test_constrain_distance(ptr1, ptr2, 3, !expected);
      
      test_constrain_distance(nullptr, nullptr, 0, !expected);
    }

    return boost::report_errors();
}
