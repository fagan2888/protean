#include <protean/variant.hpp>
#include <protean/detail/hash.hpp>
#include <protean/detail/data_table.hpp>

#include <boost/algorithm/string/predicate.hpp>

namespace protean {

    variant_base::variant_base()
    {
    }

    void variant_base::initialise(enum_type_t type, size_t size)
    {
        if ((type & Number)!=0)
        {
            m_value.set<UInt64>(0);
        }
        else
        {
            switch( type )
            {
                case None:
                    m_value.set<None>(boost::blank());
                    break;
                case Date:
                    m_value.set<Date>(boost::gregorian::date());
                    break;
                case Time:
                    m_value.set<Time>(boost::posix_time::time_duration());
                    break;
                case DateTime:
                    m_value.set<DateTime>(boost::posix_time::ptime());
                    break;
                case TimeSeries:
                    m_value.set<TimeSeries>(detail::timeseries());
                    break;
                case DataTable:
                    m_value.set<DataTable>(detail::data_table(size));
                    break;
                case Any:
                    m_value.set<Any>(detail::string());
                    break;
                case String:
                    m_value.set<String>(detail::string());
                    break;
                case Dictionary:
                    m_value.set<Dictionary>(detail::dictionary());
                    break;
                case Bag:
                    m_value.set<Bag>(detail::bag());
                    break;
                case List:
                    m_value.set<List>(detail::list(size));
                    break;
                case Tuple:
                    m_value.set<Tuple>(detail::tuple(size));
                    break;
                case Buffer:
                    m_value.set<Buffer>(handle<detail::buffer>(new detail::buffer(size)));
                    break;
                default:
                    boost::throw_exception(variant_error(std::string("Cannot default construct variant of type ") + enum_to_string(type)));
            }
        }
    }

    variant_base::variant_base(enum_type_t type, const variant_base& rhs)
    {
        assign(type, rhs);
    }

    void variant_base::assign(enum_type_t type, const variant_base& rhs)
    {
        if ((type & Number)!=0)
        {
            m_value.set<UInt64>(rhs.m_value.get<UInt64>());
        }
        else
        {
            switch(type)
            {
                case None:
                    m_value.set<None>(rhs.m_value.get<None>());
                    break;
                case Any:
                    m_value.set<Any>(rhs.m_value.get<Any>());
                    break;
                case String:
                    m_value.set<String>(rhs.m_value.get<String>());
                    break;
                case Date:
                    m_value.set<Date>(rhs.m_value.get<Date>());
                    break;
                case Time:
                    m_value.set<Time>(rhs.m_value.get<Time>());
                    break;
                case DateTime:
                    m_value.set<DateTime>(rhs.m_value.get<DateTime>());
                    break;
                case List:
                    m_value.set<List>(rhs.m_value.get<List>());
                    break;
                case Dictionary:
                    m_value.set<Dictionary>(rhs.m_value.get<Dictionary>());
                    break;
                case Bag:
                    m_value.set<Bag>(rhs.m_value.get<Bag>());
                    break;
                case TimeSeries:
                    m_value.set<TimeSeries>(rhs.m_value.get<TimeSeries>());
                    break;
                case DataTable:
                    m_value.set<DataTable>(rhs.m_value.get<DataTable>());
                    break;
                case Buffer:
                    m_value.set<Buffer>(rhs.m_value.get<Buffer>());
                    break;
                case Tuple:
                    m_value.set<Tuple>(rhs.m_value.get<Tuple>());
                    break;
                case Object:
                    m_value.set<Object>(rhs.m_value.get<Object>());
                    break;
                case Exception:
                    m_value.set<Exception>(rhs.m_value.get<Exception>());
                    break;
                case Array:
                    m_value.set<Array>(rhs.m_value.get<Array>());
                    break;
                default:
                    boost::throw_exception(variant_error(std::string("Attempt to copy unknown variant type ") + enum_to_string(type)));
            }
        }
    }

    void variant_base::destroy(enum_type_t type)
    {
        if ((type & Number)!=0)
        {
            m_value.set<UInt64>(0);
        }
        else
        {
            switch( type )
            {
                case None:
                    m_value.destroy<None>();
                    break;
                case Any:
                    m_value.destroy<Any>();
                    break;
                case String:
                    m_value.destroy<String>();
                    break;
                case Date:
                    m_value.destroy<Date>();
                    break;
                case Time:
                    m_value.destroy<Time>();
                    break;
                case DateTime:
                    m_value.destroy<DateTime>();
                    break;
                case List:
                    m_value.destroy<List>();
                    break;
                case Dictionary:
                    m_value.destroy<Dictionary>();
                    break;
                case Bag:
                    m_value.destroy<Bag>();
                    break;
                case TimeSeries:
                    m_value.destroy<TimeSeries>();
                    break;
                case DataTable:
                    m_value.destroy<DataTable>();
                    break;
                case Buffer:
                    m_value.destroy<Buffer>();
                    break;
                case Tuple:
                    m_value.destroy<Tuple>();
                    break;
                case Object:
                    m_value.destroy<Object>();
                    break;
                case Exception:
                    m_value.destroy<Exception>();
                    break;
                case Array:
                    m_value.destroy<Array>();
                    break;
                default:
                    boost::throw_exception(variant_error(std::string("Attempt to destruct unknown variant type ") + enum_to_string(type)));
            }
        }
    }

    std::string variant_base::enum_to_string(enum_type_t type)
    {
        return variant_impl_t::enum_to_string(type);
    }
    variant_base::enum_type_t variant_base::string_to_enum(const std::string& type)
    {
        return static_cast<variant_base::enum_type_t>(variant_impl_t::string_to_enum(type));
    }

    template<typename T>
    static int compare_using_less(const T& lhs, const T& rhs)
    {
        if (lhs==rhs)
        {
            return 0;
        }
        else
        {
            return lhs<rhs ? -1 : 1;
        }
    }

    int variant_base::compare(enum_type_t type, const variant_base& rhs) const
    {
        // structures are of same type
        switch (type)
        {
            case None:
                return 0;
            case Any:
                return m_value.get<Any>().compare(rhs.m_value.get<Any>());
            case String:
                return m_value.get<String>().compare(rhs.m_value.get<String>());
            case Int32:
                return compare_using_less(m_value.get<Int32>(), rhs.m_value.get<Int32>());
            case UInt32:
                return compare_using_less(m_value.get<UInt32>(), rhs.m_value.get<UInt32>());
            case Int64:
                return compare_using_less(m_value.get<Int64>(), rhs.m_value.get<Int64>());
            case UInt64:
                return compare_using_less(m_value.get<UInt64>(), rhs.m_value.get<UInt64>());
            case Float:
                return compare_using_less(m_value.get<Float>(), rhs.m_value.get<Float>());
            case Double:
                return compare_using_less(m_value.get<Double>(), rhs.m_value.get<Double>());
            case Boolean:
                if ( m_value.get<Boolean>()==rhs.m_value.get<Boolean>() )
                    return 0;
                else if ( m_value.get<Boolean>() )
                    return 1;
                else
                    return -1;
                break;
            case Date:
                return compare_using_less(m_value.get<Date>(), rhs.m_value.get<Date>());
            case Time:
                return compare_using_less(m_value.get<Time>(), rhs.m_value.get<Time>());
            case DateTime:
                return compare_using_less(m_value.get<DateTime>(), rhs.m_value.get<DateTime>());
            case Buffer:
                return m_value.get<Buffer>()->compare(*rhs.m_value.get<Buffer>());
            case List:
            case Tuple:
            case Dictionary:
            case Bag:
            case TimeSeries:
            case DataTable:
                return m_value.get<Collection>().compare(rhs.m_value.get<Collection>());
            case Exception:
                return m_value.get<Exception>().compare(rhs.m_value.get<Exception>());
            case Object:
                return m_value.get<Object>()->compare(*rhs.m_value.get<Object>());
                break;
            case Array:
                return m_value.get<Array>()->compare(*rhs.m_value.get<Array>());
                break;
            default:
                boost::throw_exception(variant_error("Unrecognised variant type " + enum_to_string(type)));
        }
    }

    boost::uint64_t variant_base::hash(enum_type_t type, boost::uint64_t seed) const
    {
        switch (type)
        {
            case None:
                return 0;
            case Any:
                return detail::hash_value(m_value.get<Any>().value(), seed);
            case String:
                return detail::hash_value(m_value.get<String>().value(), seed);
            case Int32:
                return detail::hash_value(m_value.get<Int32>(), seed);
            case UInt32:
                return detail::hash_value(m_value.get<UInt32>(), seed);
            case Int64:
                return detail::hash_value(m_value.get<Int64>(), seed);
            case UInt64:
                return detail::hash_value(m_value.get<UInt64>(), seed);
            case Float:
                return detail::hash_value(m_value.get<Float>(), seed);
            case Double:
                return detail::hash_value(m_value.get<Double>(), seed);
            case Boolean:
                return detail::hash_value(m_value.get<Boolean>(), seed);
            case Date:
                return detail::hash_value(m_value.get<Date>(), seed);
            case Time:
                return detail::hash_value(m_value.get<Time>(), seed);
            case DateTime:
                return detail::hash_value(m_value.get<DateTime>(), seed);
            case List:
            case Dictionary:
            case Bag:
            case TimeSeries:
            case DataTable:
            {
                return m_value.get<Collection>().hash(seed);
            }
            case Buffer:
            {
                return m_value.get<Buffer>()->hash(seed);
            }
            case Tuple:
            {
                return m_value.get<Tuple>().hash(seed);
            }
            case Exception:
            {
                return m_value.get<Exception>().hash(seed);
            }
            case Object:
            {
                return m_value.get<Object>()->hash(seed);
            }
            case Array:
            {
                return m_value.get<Array>()->hash(seed);
            }
            default:
            {
                boost::throw_exception(variant_error("Unrecognised variant type"));
            }
        }
    }

    void variant_base::swap(variant_base& rhs)
    {
        m_value.swap(rhs.m_value);
    }

} // namespace protean
