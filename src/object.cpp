//  (C) Copyright Johan Ditmar, Karel Hruda, Paul O'Neill & Luke Stedman 2009.
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#include <protean/object.hpp>
#include <protean/variant.hpp>

#include <protean/detail/hash.hpp>

namespace protean {

    object::~object()
    {
    }

    void object::coerce(const object& rhs)
    {
        if (name()!=rhs.name())
        {
            boost::throw_exception(variant_error(
                (boost::format("Attempt to coerce object of type %s into %s")
                    % rhs.name()
                    % name()).str()));
        }

        variant params;
        rhs.deflate(params);
        inflate(params, rhs.version());
    }

    int object::compare(const object& rhs) const
    {
        if ( name()!=rhs.name() )
        {
            return name().compare(rhs.name());
        }
        else
        {
            variant lhs_params, rhs_params;
            deflate(lhs_params);
            rhs.deflate(rhs_params);
            return lhs_params.compare(rhs_params);
        }
    }

    boost::uint64_t object::hash(boost::uint64_t seed) const
    {
        seed = detail::hash_value(name(), seed);

        variant params;
        deflate(params);
        seed = detail::hash_value(params, seed);

        return seed;
    }

} // namespace protean
