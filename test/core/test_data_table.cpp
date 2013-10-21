//  (C) Copyright Johan Ditmar, Karel Hruda, Paul O'Neill & Luke Stedman 2009.
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#include <boost/test/test_tools.hpp>
#include <boost/test/unit_test.hpp>  

using boost::unit_test::test_suite;    

#include <protean/variant.hpp>
#include <protean/detail/data_table.hpp>
#include <protean/xml_writer.hpp>
#include <protean/xml_reader.hpp>
#include <boost/date_time.hpp>
#include <protean/binary_writer.hpp>
#include <protean/binary_reader.hpp>
using namespace protean;

BOOST_AUTO_TEST_SUITE(data_table_suite);

BOOST_AUTO_TEST_CASE(test_data_table_basic)
{
    variant v1(variant::DataTable);
    BOOST_CHECK(v1.is<variant::DataTable>());
    BOOST_CHECK(v1.is<variant::Collection>());
    BOOST_CHECK(!v1.is<variant::Primitive>());
    BOOST_CHECK(!v1.is<variant::Mapping>());

    BOOST_CHECK(v1.empty());
    BOOST_CHECK_EQUAL(v1.size(), 0u);

    v1.add_column(variant::DateTime)
      .add_column(variant::Int32)
      .add_column(variant::Double);

    data_table_row<variant::DateTime, variant::Int32, variant::Double>::type
        row1 = boost::tuples::make_tuple(boost::posix_time::time_from_string("2002-01-20 09:00:00.000"), 42, 3.141);
    data_table_row<variant::DateTime, variant::Int32, variant::Double>::type
        row2 = boost::tuples::make_tuple(boost::posix_time::time_from_string("2002-01-20 10:00:00.000"), 1, 2.718);

    v1.push_back(row1)
      .push_back(row2);

    BOOST_CHECK(!v1.empty());
    BOOST_CHECK(v1.size()==2);

    BOOST_CHECK_THROW(v1.add_column(variant::String), variant_error);

    variant v2(variant::DataTable);
    v2.add_column(variant::DateTime)
      .add_column(variant::Int32)
      .add_column(variant::Double);
    BOOST_CHECK(v1!=v2);

    v2.push_back(row1)
      .push_back(row2);
    BOOST_CHECK(v1==v2);

    variant v3(v2);
    BOOST_CHECK(v3.is<variant::DataTable>());
    BOOST_CHECK(v3==v2);

    variant v4;
    v4 = v3;
    BOOST_CHECK(v4.is<variant::DataTable>());
    BOOST_CHECK(v4==v3);
}

BOOST_AUTO_TEST_CASE(test_data_table_types)
{
    variant v1(variant::DataTable);
    
    BOOST_CHECK_THROW(v1.add_column(variant::Dictionary), variant_error);
    BOOST_CHECK_THROW(v1.add_column(variant::Collection), variant_error);
    BOOST_CHECK_THROW(v1.add_column(variant::DataTable), variant_error);

    v1.add_column(variant::DateTime)
      .add_column(variant::Int32)
      .add_column(variant::Double);

    BOOST_CHECK_THROW(v1.push_back( boost::tuples::make_tuple(std::string("Incompatible columns")) ), variant_error);

    data_table_row<variant::DateTime, variant::Int32, variant::Double>::type
        row1 = boost::tuples::make_tuple(boost::posix_time::time_from_string("2002-01-20 09:00:00.000"), 42, 3.141);
    data_table_row<variant::DateTime, variant::Int32, variant::Double>::type
        row2 = boost::tuples::make_tuple(boost::posix_time::time_from_string("2002-01-20 10:00:00.000"), 1, 2.718);

    v1.push_back(row1)
      .push_back(row2);

    BOOST_CHECK_THROW(v1.begin<variant::String>(), variant_error);
    BOOST_CHECK_THROW(v1.end<variant::String>(), variant_error);
}

BOOST_AUTO_TEST_CASE(test_data_table_iterator)
{
    variant v1(variant::DataTable);
    v1.add_column(variant::DateTime)
      .add_column(variant::Int32)
      .add_column(variant::Double);

    // Or one can write: data_table<variant::DateTime, variant::Int32, variant::Double>::iterator empty_begin = ...
    data_table_iterator<variant::DateTime, variant::Int32, variant::Double>
        empty_begin = v1.begin<variant::DateTime, variant::Int32, variant::Double>();
    data_table_iterator<variant::DateTime, variant::Int32, variant::Double>
        empty_end = v1.end<variant::DateTime, variant::Int32, variant::Double>();

    BOOST_CHECK(empty_begin==empty_end);

    data_table_row<variant::DateTime, variant::Int32, variant::Double>::type
        row1 = boost::tuples::make_tuple(boost::posix_time::time_from_string("2002-01-20 09:00:00.000"), 42, 3.141);
    data_table_row<variant::DateTime, variant::Int32, variant::Double>::type
        row2 = boost::tuples::make_tuple(boost::posix_time::time_from_string("2002-01-20 10:00:00.000"), 666, 2.718);

    v1.push_back(row1)
      .push_back(row2);

    data_table_iterator<variant::DateTime, variant::Int32, variant::Double>
        begin =	v1.begin<variant::DateTime, variant::Int32, variant::Double>();
    data_table_iterator<variant::DateTime, variant::Int32, variant::Double>
        end = v1.end<variant::DateTime, variant::Int32, variant::Double>();

    BOOST_CHECK_EQUAL(begin->get<0>(), row1.get<0>());
    BOOST_CHECK_EQUAL(begin->get<1>(), row1.get<1>());
    BOOST_CHECK_EQUAL(begin->get<2>(), row1.get<2>());

    ++begin;

    BOOST_CHECK(begin!=end);

    BOOST_CHECK_EQUAL(begin->get<0>(), row2.get<0>());
    BOOST_CHECK_EQUAL(begin->get<1>(), row2.get<1>());
    BOOST_CHECK_EQUAL(begin->get<2>(), row2.get<2>());

    ++begin;

    BOOST_CHECK(begin==end);

    --begin;
    BOOST_CHECK(begin->get<1>()==666);

    begin->get<1>() = 1729;

    BOOST_CHECK(begin->get<1>()==1729);
}

BOOST_AUTO_TEST_CASE(test_data_table_variant_iterator)
{
    variant v1(variant::DataTable);

    BOOST_CHECK(v1.begin()==v1.end());

    v1.add_column(variant::DateTime)
      .add_column(variant::Int32)
      .add_column(variant::Double);

    BOOST_CHECK(v1.begin()==v1.end());

    // Or one can write: data_table<variant::DateTime, variant::Int32, variant::Double>::value_type row1 = ...
    data_table_row<variant::DateTime, variant::Int32, variant::Double>::type
        row1 = boost::tuples::make_tuple(boost::posix_time::time_from_string("2002-01-20 09:00:00.000"), 42, 3.141);
    data_table_row<variant::DateTime, variant::Int32, variant::Double>::type
        row2 = boost::tuples::make_tuple(boost::posix_time::time_from_string("2002-01-20 10:00:00.000"), 1, 2.718);

    v1.push_back(row1)
      .push_back(row2);

    BOOST_CHECK(v1.begin()!=v1.end());

    variant::iterator begin(v1.begin());
    variant::iterator end(v1.end());

    BOOST_CHECK_THROW(begin.key(), variant_error);
    BOOST_CHECK_THROW(begin.time(), variant_error);

    BOOST_CHECK(begin->is<variant::Tuple>());
    BOOST_CHECK(begin->at(0).is<variant::DateTime>());
    BOOST_CHECK(begin->at(1).is<variant::Int32>());
    BOOST_CHECK(begin->at(2).is<variant::Double>());

    data_table_row<variant::DateTime, variant::Int32, variant::Double>::type
        tuple1 = boost::tuples::make_tuple(
            begin->at(0).as<variant::date_time_t>(),
            begin->at(1).as<boost::int32_t>(),
            begin->at(2).as<double>()
        );
    BOOST_CHECK_EQUAL(row1.get<0>(), tuple1.get<0>());
    BOOST_CHECK_EQUAL(row1.get<1>(), tuple1.get<1>());
    BOOST_CHECK_EQUAL(row1.get<2>(), tuple1.get<2>());

    ++begin;

    data_table_row<variant::DateTime, variant::Int32, variant::Double>::type
        tuple2 = boost::tuples::make_tuple(
            begin->at(0).as<variant::date_time_t>(),
            begin->at(1).as<boost::int32_t>(),
            begin->at(2).as<double>()
        );
    BOOST_CHECK_EQUAL(row2.get<0>(), tuple2.get<0>());
    BOOST_CHECK_EQUAL(row2.get<1>(), tuple2.get<1>());
    BOOST_CHECK_EQUAL(row2.get<2>(), tuple2.get<2>());

    ++begin;

    BOOST_CHECK(begin==end);
}

BOOST_AUTO_TEST_CASE(test_data_table_large_number_of_columns)
{
    variant dt(variant::DataTable);

    #define NUM_COLUMNS 50

    for (int i = 0; i < NUM_COLUMNS; ++i)
        dt.add_column(variant::Int32);

    BOOST_CHECK_THROW(dt.add_column(variant::Int32), variant_error);

    data_table_row<
        BOOST_PP_ENUM(NUM_COLUMNS, IDENTITY, variant::Int32)
    >::type row =
        make_row(BOOST_PP_ENUM_PARAMS(NUM_COLUMNS, (int)));

    dt.push_back(row);

    data_table_const_iterator<
        BOOST_PP_ENUM(NUM_COLUMNS, IDENTITY, variant::Int32)
    > begin =
        dt.begin<
            BOOST_PP_ENUM(NUM_COLUMNS, IDENTITY, variant::Int32)
        >();

    #define CHECK_EQUAL_TUPLE_ELEMENT_N(z, n, _) \
        BOOST_CHECK_EQUAL(begin->get<n>(), boost::get<n>(row));

    BOOST_PP_REPEAT(NUM_COLUMNS, CHECK_EQUAL_TUPLE_ELEMENT_N, _)
}

BOOST_AUTO_TEST_CASE(test_data_table_binary_serialization_performance)
{
    /* // Start of commented-out performance test (uncomment to run)
    {
        variant dt(variant::DataTable);
        dt.add_column(variant::DateTime, "Time")
          .add_column(variant::Double,   "Price")
          .add_column(variant::String,   "Data");

        const boost::posix_time::ptime initial_time = boost::posix_time::time_from_string("2002-01-20 09:00:00.000");

        clock_t start = clock();
        for (int i = 0; i < 1000000; ++i)
            dt.push_back(boost::tuples::make_tuple(
                initial_time + boost::posix_time::hours(i),
                100.4 + i,
                detail::string("value0")
            ));
        clock_t finish = clock();

        std::cout << "Construction took " << static_cast<double>(finish - start) / CLOCKS_PER_SEC << " seconds." << std::endl;

        std::ostringstream oss2;
        binary_writer bw2(oss2);

        start = clock();
        bw2 << dt;
        finish = clock();

        std::cout << "Serialization took " << static_cast<double>(finish - start) / CLOCKS_PER_SEC << " seconds." << std::endl;

        std::istringstream iss2(oss2.str());
        binary_reader br2(iss2);
        variant read_dt;

        start = clock();
        br2 >> read_dt;
        finish = clock();

        std::cout << "Deserialization took " << static_cast<double>(finish - start) / CLOCKS_PER_SEC << " seconds." << std::endl;
    }
    */ // End of commented-out performance test (uncomment to run)
}

BOOST_AUTO_TEST_SUITE_END()
