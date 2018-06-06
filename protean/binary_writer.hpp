#ifndef PROTEAN_BINARY_WRITER_HPP
#define PROTEAN_BINARY_WRITER_HPP

#include <protean/config.hpp>

#include <protean/binary_common.hpp>
#include <protean/variant.hpp>

#if defined(_MSC_VER)
#    pragma warning(push)
#    pragma warning(disable:4251)
#endif

namespace protean {
    
    class PROTEAN_DECL binary_writer
    {
    public:
        binary_writer(std::ostream &os, int mode=binary_mode::Default);
        ~binary_writer();
        
        void write(const variant& value);
        void write(const std::string& value);
        void write(const detail::string& value);
        void write(bool value);
        void write(boost::int32_t value);
        void write(boost::uint32_t value);
        void write(boost::int64_t value);
        void write(boost::uint64_t value);
        void write(float value);
        void write(double value);
        void write(const variant::date_t& value);
        void write(const variant::time_t& value);
        void write(const variant::date_time_t& value);
        void write(const void* data, size_t length);

        // write to stream
        void write_bytes(const char* value, size_t length);
        void write_value(const variant& value);

	protected:
		void setup();
        void close();

    private:
        std::ostream&                       m_os;
        boost::iostreams::filtering_ostream m_filter;
        int                                 m_mode;

        friend PROTEAN_DECL binary_writer& operator<<(binary_writer& writer, const variant& v);
    };

} // namespace protean

#if defined(_MSC_VER)
#    pragma warning(pop)
#endif

#endif // PROTEAN_BINARY_WRITER_HPP
