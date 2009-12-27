//  (C) Copyright Johan Ditmar, Karel Hruda, Paul O'Neill & Luke Stedman 2009.
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#ifndef PROTEAN_EXCEPTION_INFO_HPP
#define PROTEAN_EXCEPTION_INFO_HPP

#include <protean/config.hpp>

#include <string>

namespace protean {

    class PROTEAN_DLLEXPORT exception_info
    {
    public:
        exception_info(const std::string& type, const std::string& message);
        exception_info(const std::exception& e);

        const std::string& type() const;
        const std::string& message() const;

        int compare(const exception_info& rhs) const;
        size_t hash() const;

        static std::string format(const char* class_name);

    private:
        const std::string m_type;
        const std::string m_message;
    };

} // namespace protean

#endif // PROTEAN_EXCEPTION_INFO_HPP
