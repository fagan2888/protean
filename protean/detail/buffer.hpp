#ifndef PROTEAN_DETAIL_BUFFER_HPP
#define PROTEAN_DETAIL_BUFFER_HPP

#include <protean/config.hpp>

#include <protean/detail/ref_counted.hpp>

#include <boost/cstdint.hpp>

#include <cstddef>

namespace protean { namespace detail {
    
    class PROTEAN_DECL buffer : public detail::ref_counted
    {
    public:
        buffer(size_t size);
        buffer(const void* data, size_t size);
        buffer(const buffer& rhs);
        buffer& operator=(const buffer& rhs);
        ~buffer();

        void*  data() const;
        size_t size() const;

        int compare(const buffer& rhs) const;
        boost::uint64_t hash(boost::uint64_t seed) const;

        void swap(buffer& rhs);

    private:
        size_t  m_size;
        void*   m_data;
    };

}} // namespace protean::detail

#endif // PROTEAN_DETAIL_BUFFER_HPP
