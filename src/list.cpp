//  (C) Copyright Johan Ditmar, Paul O'Neill & Luke Stedman 2009.
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#include <protean/detail/list.hpp>
#include <protean/detail/list_iterator.hpp>
#include <protean/variant.hpp>
#include <protean/variant_error.hpp>

#include <boost/functional/hash.hpp>
#include <boost/format.hpp>

namespace protean { namespace detail {

    list::list(size_t size) :
        m_value(size)
    {
    }

    int list::compare(const collection& rhs) const
    {
        const list* rhsList = dynamic_cast<const list*>(&rhs);
        if ( rhsList==NULL )
        {
            boost::throw_exception(variant_error("Unable to cast collection to list"));
        }
        if ( m_value.size()!=rhsList->m_value.size() )
        {
            return m_value.size()<rhsList->m_value.size();
        }
        return std::lexicographical_compare(
            m_value.begin(), m_value.end(), rhsList->m_value.begin(), rhsList->m_value.end() );
    }

    size_t list::hash() const
    {
        return boost::hash_range(m_value.begin(), m_value.end());
    }

    bool list::empty() const
    {
        return m_value.empty();
    }

    size_t list::size() const
    {
        return m_value.size();
    }

    void list::clear()
    {
        m_value.clear();
    }

    variant& list::push_back( const variant &value )
    {
        m_value.push_back( value );
        return *m_value.rbegin();
    }

    void list::pop_back()
    {
        if ( m_value.empty() )
        {
            boost::throw_exception(variant_error("Attempt to call pop_back() on empty List variant"));
        }
        m_value.pop_back();
    }

    const variant& list::at(size_t index) const
    {
        if ( index>=m_value.size() )
        {
            boost::throw_exception(variant_error((boost::format("Index %u is out of range for List of size %u") % index % m_value.size()).str()));
        }
        return m_value[index];
    }

    variant& list::at(size_t index)
    {
        if ( index>=m_value.size() )
        {
            boost::throw_exception(variant_error((boost::format("Index %u is out of range for List of size %u") % index % m_value.size()).str()));
        }
        return m_value[index];
    }
    variant_const_iterator_base* list::begin() const
    {
        return new list_const_iterator(m_value.begin());
    }
    variant_const_iterator_base* list::end() const
    {
        return new list_const_iterator(m_value.end());
    }
    variant_iterator_base* list::begin()
    {
        return new list_iterator(m_value.begin());
    }
    variant_iterator_base* list::end()
    {
        return new list_iterator(m_value.end());
    }

}} // namespace protean