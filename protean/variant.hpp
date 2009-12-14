//  (C) Copyright Johan Ditmar, Paul O'Neill & Luke Stedman 2009.
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#ifndef PROTEAN_VARIANT_HPP
#define PROTEAN_VARIANT_HPP

#include <protean/config.hpp>
#include <protean/variant_base.hpp>

namespace protean {

    class PROTEAN_DLLEXPORT variant : public variant_base
    {
    public:
        typedef boost::gregorian::date              date_t;
        typedef boost::posix_time::time_duration    time_t;
        typedef boost::posix_time::ptime            date_time_t;

        typedef long                                int32_t;
        typedef unsigned                            long uint32_t;
        typedef long long                           int64_t;
        typedef unsigned long long                  uint64_t;

        BOOST_STATIC_ASSERT (sizeof (int32_t)==4 && sizeof (uint32_t)==4);
        BOOST_STATIC_ASSERT (sizeof (int64_t)==8 && sizeof (uint64_t)==8);

    public:
        /* Construction */
        /****************/
        variant();
        variant(const variant& value);

        template<typename T>
        variant(T value, typename boost::enable_if<boost::is_pod<T> >::type* dummy = 0);

        explicit variant(enum_type_t type, size_t size=0);
        explicit variant(enum_type_t type, const variant& base);
        explicit variant(const std::string& value);
        explicit variant(const char *value);
        explicit variant(bool value);
        explicit variant(const date_t& value);
        explicit variant(const time_t& value);
        explicit variant(const date_time_t& value);
        explicit variant(void* data, size_t size, bool clone=true);
        explicit variant(const exception_info& value);
        explicit variant(const std::exception& e);
        explicit variant(const object& o);
        explicit variant(const object_handle& o);


        /* Assignment */
        /**************/
        variant& operator=( const variant &value );

        template<typename T>
        typename boost::enable_if<boost::is_pod<T>, variant&>::type
        operator=(T value);

        template<typename T>
        typename boost::disable_if<boost::is_pod<T>, variant&>::type
        operator=(const T& value);

        /* Destructor */
        /**************/
        ~variant();

        /* Type inspection */
        /*******************/
        enum_type_t type() const { return m_type; }

        template<typename T> 
        typename boost::disable_if<boost::is_base_of<object,T>, bool>::type
        is() const;

        template<typename T> 
        typename boost::enable_if<boost::is_base_of<object,T>, bool>::type
        is() const;

        template<int N>
        bool is() const;

        /* Type casting */
        /****************/
        template<typename T>
        typename boost::disable_if<boost::is_base_of<object,T>, T>::type
        as() const;

        template<typename T>
        typename boost::enable_if<boost::is_base_of<object,T>, const T&>::type
        as() const;

        template<typename T>
        typename boost::enable_if<boost::is_base_of<object,T>, T&>::type
        as();

        /* Collection interface */
        /************************/
        enum enum_return_trait_t {ReturnSelf, ReturnItem};

        typedef variant_iterator<const_iterator_traits> const_iterator;
        typedef variant_iterator<iterator_traits> iterator;

        const_iterator begin() const;
        const_iterator end() const;
        iterator begin();
        iterator end();

        bool empty() const;
        size_t size() const;
        void clear();

        /* List interface */
        /******************/
        variant& push_back(const variant& value, enum_return_trait_t ret = ReturnSelf);
        variant& pop_back();

        /* TimeSeries interface */
        /************************/
        variant& push_back(const date_time_t& time, const variant& value, enum_return_trait_t ret = ReturnSelf );

        /* Sequence interface */
        /*********************/
        const variant& at(size_t index) const;
        variant& at(size_t index);
        const variant& operator[](size_t index) const;
        variant& operator[](size_t index);

        /* Mapping interface */
        /*********************/
        variant& insert(const std::string& key, const variant& value, enum_return_trait_t ret = ReturnSelf);
        bool has_key(const std::string& key) const;

        // TODO: deprecate
        bool contains(const std::string& key) const;
        const variant& at(const std::string& key) const;
        variant& at(const std::string& key);
        const variant& operator[](const std::string& key) const;
        variant& operator[]( const std::string &key );
        variant& remove(const std::string& key);
        variant range(const std::string& key) const;

        /* Comparison/Equality */
        /***********************/
        int compare(const variant& rhs) const;
        bool operator<(const variant& rhs) const;
        bool operator==(const variant& rhs) const;
        bool operator!=(const variant& rhs) const;

        /* Miscelaneous */
        /****************/
        variant select(const std::string& path) const;

        template<typename TARGET, typename SOURCE>
        static TARGET lexical_cast(const SOURCE& toCast);

        static variant make_any(const std::string& value);

        void swap(variant& rhs);

        size_t hash() const;

        variant downCast(enum_type_t type) const;
        variant upCast() const;

        std::string str(bool summarise=false, const std::string& indent = "") const;

    private:
        static void select_impl(const variant& input, const std::string& path, variant& result);

        // type discriminator
        enum_type_t m_type; 
    };

    PROTEAN_DLLEXPORT size_t hash_value(const variant& val);

    struct const_iterator_traits
    {
        typedef const variant value_type;
        typedef std::vector<variant>::const_iterator list_iterator_type;
        typedef std::map<std::string, variant>::const_iterator dictionary_iterator_type;
        typedef std::list<std::pair<std::string, variant> >::const_iterator bag_iterator_type;
        typedef std::vector<std::pair<boost::posix_time::ptime, variant> >::const_iterator timeseries_iterator_type;
        typedef const variant* tuple_iterator_type;
        typedef size_t difference_type;
    };

    struct iterator_traits
    {
        typedef variant value_type;
        typedef std::vector<variant>::iterator list_iterator_type;
        typedef std::map<std::string, variant>::iterator dictionary_iterator_type;
        typedef std::list<std::pair<std::string, variant> >::iterator bag_iterator_type;
        typedef std::vector<std::pair<boost::posix_time::ptime, variant> >::iterator timeseries_iterator_type;
        typedef variant* tuple_iterator_type;
        typedef size_t difference_type;
    };

} // namespace protean

#include <protean/variant.ipp>

#endif // PROTEAN_VARIANT_HPP
