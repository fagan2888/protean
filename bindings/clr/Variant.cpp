//  (C) Copyright Johan Ditmar, Karel Hruda, Paul O'Neill & Luke Stedman 2009.
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

#include "Variant.hpp"
#include "VariantEnumerator.hpp"
#include "IVariantObject.hpp"
#include "VariantObjectProxy.hpp"
#include "ExceptionInfo.hpp"
#include "VariantException.hpp"
#include "StringTranslator.hpp"
#include "StrongReference.hpp"

namespace protean { namespace clr {

    Variant::Variant() :
        m_variant(new protean::variant())
    {
    }

    Variant::Variant(Variant^ arg) :
        m_variant(new protean::variant(arg->get_internals()))
    {
    }

    Variant::Variant(EnumType type)
    {
        BEGIN_TRANSLATE_ERROR();

        m_variant = new protean::variant((protean::variant::enum_type_t)type);

        END_TRANSLATE_ERROR();
    }

    Variant::Variant(EnumType type, System::Int32 size)
    {
        BEGIN_TRANSLATE_ERROR();

        m_variant = new protean::variant((protean::variant::enum_type_t)type, size);

        END_TRANSLATE_ERROR();
    }

    Variant::Variant(bool arg)
    {
        BEGIN_TRANSLATE_ERROR();

        m_variant = new protean::variant(arg);

        END_TRANSLATE_ERROR();
    }

    Variant::Variant(System::String^ arg)
    {
        BEGIN_TRANSLATE_ERROR();

        std::string arg_str(StringTranslator(arg).c_str());

        m_variant = new protean::variant(arg_str);

        END_TRANSLATE_ERROR();
    }

    Variant::Variant(System::Int32 arg)
    {
        BEGIN_TRANSLATE_ERROR();

        m_variant = new protean::variant(arg);

        END_TRANSLATE_ERROR();
    }

    Variant::Variant(System::UInt32 arg)
    {
        BEGIN_TRANSLATE_ERROR();

        m_variant = new protean::variant(arg);

        END_TRANSLATE_ERROR();
    }

    Variant::Variant(System::Int64 arg)
    {
        BEGIN_TRANSLATE_ERROR();

        m_variant = new protean::variant(arg);

        END_TRANSLATE_ERROR();
    }

    Variant::Variant(System::UInt64 arg)
    {
        BEGIN_TRANSLATE_ERROR();

        m_variant = new protean::variant(arg);

        END_TRANSLATE_ERROR();
    }

    Variant::Variant(float arg)
    {
        BEGIN_TRANSLATE_ERROR();

        m_variant = new protean::variant(arg);

        END_TRANSLATE_ERROR();
    }

    Variant::Variant(double arg)
    {
        BEGIN_TRANSLATE_ERROR();

        m_variant = new protean::variant(arg);

        END_TRANSLATE_ERROR();
    }

    Variant::Variant(System::TimeSpan arg)
    {
        BEGIN_TRANSLATE_ERROR();

        protean::variant::time_t time = boost::posix_time::time_duration(arg.Hours + 24*arg.Days, arg.Minutes, arg.Seconds) + boost::posix_time::millisec(arg.Milliseconds);
        m_variant = new protean::variant(time);

        END_TRANSLATE_ERROR();
    }

    Variant::Variant(System::DateTime arg)
    {
        BEGIN_TRANSLATE_ERROR();

        protean::variant::date_t date(arg.Date.Year, arg.Date.Month, arg.Date.Day);
        protean::variant::time_t time = boost::posix_time::time_duration(arg.TimeOfDay.Hours, arg.TimeOfDay.Minutes, arg.TimeOfDay.Seconds) + boost::posix_time::millisec(arg.TimeOfDay.Milliseconds);
        protean::variant::date_time_t dateTime(date, time);
        m_variant = new protean::variant(dateTime);

        END_TRANSLATE_ERROR();
    }

    Variant::Variant(System::DateTime arg, bool ignoreTime)
    {
        BEGIN_TRANSLATE_ERROR();

        protean::variant::date_t date(arg.Date.Year, arg.Date.Month, arg.Date.Month);
        if (ignoreTime)
        {
            m_variant = new protean::variant(date);
        }
        else
        {
            protean::variant::time_t time(arg.TimeOfDay.Hours, arg.TimeOfDay.Minutes, arg.TimeOfDay.Seconds);
            protean::variant::date_time_t dateTime(date, time);
            m_variant = new protean::variant(dateTime);
        }

        END_TRANSLATE_ERROR();
    }

    Variant::Variant(array<System::Byte>^ arg)
    {
        BEGIN_TRANSLATE_ERROR();

        pin_ptr<System::Byte> pinned(&arg[0]);
        m_variant = new protean::variant(pinned, arg->Length);

        END_TRANSLATE_ERROR();
    }

    Variant::Variant(IVariantObject^ arg)
    {
        BEGIN_TRANSLATE_ERROR();

        VariantObjectProxy^ proxy = gcnew VariantObjectProxy(arg->ClassName);
        proxy->Inflate(arg->Deflate(), arg->Version);

        m_variant = new protean::variant(proxy->get_internals());

        END_TRANSLATE_ERROR();
    }

    Variant::Variant(VariantObjectProxy^ arg)
    {
        BEGIN_TRANSLATE_ERROR();

        m_variant = new protean::variant(arg->get_internals());

        END_TRANSLATE_ERROR();
    }

    Variant::Variant(System::Exception^ arg)
    {
        BEGIN_TRANSLATE_ERROR();

        std::string type_str(StringTranslator(arg->GetType()->ToString()).c_str());
        std::string message_str(StringTranslator(arg->Message).c_str());
        std::string source_str(StringTranslator(arg->Source).c_str());
        std::string stack_str(StringTranslator(arg->StackTrace).c_str());

        m_variant = new protean::variant(protean::exception_data(type_str, message_str, source_str, stack_str));

        END_TRANSLATE_ERROR();
    }

    Variant::Variant(ExceptionInfo^ arg)
    {
        BEGIN_TRANSLATE_ERROR();

        m_variant = new protean::variant(arg->get_internals());

        END_TRANSLATE_ERROR();
    }

    Variant::~Variant()
    {
        this->!Variant();
    }

    Variant::!Variant()
    {
        delete m_variant;
    }

    bool Variant::Is(EnumType type)
    {
        BEGIN_TRANSLATE_ERROR();

        STRONG_REFERENCE(this);

        return (m_variant->type() & static_cast<protean::variant::enum_type_t>(type))!=0;

        END_TRANSLATE_ERROR();
    }

    System::String^ Variant::AsString()
    {
        BEGIN_TRANSLATE_ERROR();

        STRONG_REFERENCE(this);

        return gcnew System::String(m_variant->as<std::string>().c_str());

        END_TRANSLATE_ERROR();
    }
    bool Variant::AsBoolean()
    {
        BEGIN_TRANSLATE_ERROR();

        STRONG_REFERENCE(this);

        return m_variant->as<bool>();

        END_TRANSLATE_ERROR();
    }
    System::Int32 Variant::AsInt32()
    {
        BEGIN_TRANSLATE_ERROR();

        STRONG_REFERENCE(this);

        return m_variant->as<boost::int32_t>();

        END_TRANSLATE_ERROR();
    }
    System::UInt32 Variant::AsUInt32()
    {
        BEGIN_TRANSLATE_ERROR();

        STRONG_REFERENCE(this);

        return m_variant->as<boost::uint32_t>();

        END_TRANSLATE_ERROR();
    }
    System::Int64 Variant::AsInt64()
    {
        BEGIN_TRANSLATE_ERROR();

        STRONG_REFERENCE(this);

        return m_variant->as<boost::int64_t>();

        END_TRANSLATE_ERROR();
    }
    System::UInt64 Variant::AsUInt64()
    {
        BEGIN_TRANSLATE_ERROR();

        STRONG_REFERENCE(this);

        return m_variant->as<boost::uint64_t>();

        END_TRANSLATE_ERROR();
    }
    float Variant::AsFloat()
    {
        BEGIN_TRANSLATE_ERROR();

        STRONG_REFERENCE(this);

        return m_variant->as<float>();

        END_TRANSLATE_ERROR();
    }
    double Variant::AsDouble()
    {
        BEGIN_TRANSLATE_ERROR();

        STRONG_REFERENCE(this);

        return m_variant->as<double>();

        END_TRANSLATE_ERROR();
    }
    System::DateTime Variant::AsDate()
    {
        BEGIN_TRANSLATE_ERROR();

        STRONG_REFERENCE(this);

        protean::variant::date_t date(m_variant->as<protean::variant::date_t>());
        return System::DateTime(date.year(), date.month(), date.day());

        END_TRANSLATE_ERROR();
    }
    System::TimeSpan Variant::AsTime()
    {
        BEGIN_TRANSLATE_ERROR();

        STRONG_REFERENCE(this);

        return System::TimeSpan(10000 * m_variant->as<protean::variant::time_t>().total_milliseconds());

        END_TRANSLATE_ERROR();
    }
    System::DateTime Variant::AsDateTime()
    {
        BEGIN_TRANSLATE_ERROR();

        STRONG_REFERENCE(this);

		boost::int64_t total_millis = (m_variant->as<protean::variant::date_time_t>() - variant::min_date_time()).total_milliseconds();

		return System::DateTime(1400, 1, 1) + System::TimeSpan(total_millis * 10000);

		//new TimeSpan(total_millis * 10000);

        //protean::variant::date_time_t dt(m_variant->as<protean::variant::date_time_t>());
        //return System::DateTime(dt.date().year(), dt.date().month(), dt.date().day(), dt.time_of_day().hours(), dt.time_of_day().minutes(), dt.time_of_day().seconds() );

        END_TRANSLATE_ERROR();
    }
    array<System::Byte>^ Variant::AsBuffer()
    {
        BEGIN_TRANSLATE_ERROR();

        STRONG_REFERENCE(this);

        array<System::Byte>^ local = gcnew array<System::Byte>(static_cast<int>(m_variant->size()));
        pin_ptr<System::Byte> pLocal(&local[0]);
        memcpy(pLocal, m_variant->as<void*>(), m_variant->size());
        return local;

        END_TRANSLATE_ERROR();
    }

    generic <class T>
    where T : IVariantObject, gcnew()
    T Variant::AsObject()
    {
        BEGIN_TRANSLATE_ERROR();

        STRONG_REFERENCE(this);

        const protean::object& obj(m_variant->as<protean::object>());

        T newObj = gcnew T();

        if (newObj->ClassName->Equals(gcnew System::String(obj.name().c_str())))
        {
            protean::variant params;
            obj.deflate(params);

            Variant^ clrParams = gcnew Variant();

            // do a swap to save copying data
            clrParams->get_internals().swap(params);

            try
            {
                newObj->Inflate(clrParams, obj.version());
            }
            finally
            {
                delete clrParams;
            }
        }
        else
        {
            std::string className_str(StringTranslator(newObj->ClassName).c_str());

            std::ostringstream oss;
            oss << "Attempt to coerce object of type '" << obj.name() << "' into '" << className_str << "'.";

            throw gcnew VariantException(gcnew System::String(oss.str().c_str()));
        }

        return newObj;

        END_TRANSLATE_ERROR();
    }

    VariantObjectProxy^ Variant::AsObject()
    {
        BEGIN_TRANSLATE_ERROR();

        STRONG_REFERENCE(this);

        return gcnew VariantObjectProxy(m_variant->as<protean::object_proxy>());

        END_TRANSLATE_ERROR();
    }

    ExceptionInfo^ Variant::AsException()
    {
        BEGIN_TRANSLATE_ERROR();

        STRONG_REFERENCE(this);

        return gcnew ExceptionInfo(m_variant->as<protean::exception_data>());

        END_TRANSLATE_ERROR();
    }

    void Variant::Add(Variant^ value)
    {
        BEGIN_TRANSLATE_ERROR();

        STRONG_REFERENCE(this);
        STRONG_REFERENCE(value);

        m_variant->push_back(value->get_internals());

        END_TRANSLATE_ERROR();
    }

    bool Variant::ContainsKey(System::String^ key)
    {
        BEGIN_TRANSLATE_ERROR();

        STRONG_REFERENCE(this);

        std::string key_str(StringTranslator(key).c_str());

        return m_variant->has_key(key_str);

        END_TRANSLATE_ERROR();
    }

    void Variant::Add(System::String^ key, Variant^ value)
    {
        BEGIN_TRANSLATE_ERROR();

        STRONG_REFERENCE(this);
        STRONG_REFERENCE(value);

        std::string key_str(StringTranslator(key).c_str());

        m_variant->insert(key_str, value->get_internals());

        END_TRANSLATE_ERROR();
    }

    void Variant::Remove(System::String^ key)
    {
        BEGIN_TRANSLATE_ERROR();

        STRONG_REFERENCE(this);

        std::string key_str(StringTranslator(key).c_str());

        m_variant->remove(key_str);

        END_TRANSLATE_ERROR();
    }

    void Variant::Add(System::DateTime index, Variant^ value)
    {
        BEGIN_TRANSLATE_ERROR();

        STRONG_REFERENCE(this);

        protean::variant::date_t date(index.Date.Year, index.Date.Month, index.Date.Day);
        protean::variant::time_t time(index.TimeOfDay.Hours, index.TimeOfDay.Minutes, index.TimeOfDay.Seconds);
        protean::variant::date_time_t dateTime(date,time);

        m_variant->push_back(dateTime, value->get_internals());

        END_TRANSLATE_ERROR();
    }

    int Variant::Count::get()
    {
        BEGIN_TRANSLATE_ERROR();

        STRONG_REFERENCE(this);

        return static_cast<int>(m_variant->size());

        END_TRANSLATE_ERROR();
    }

    bool Variant::Empty::get()
    {
        BEGIN_TRANSLATE_ERROR();

        STRONG_REFERENCE(this);

        return m_variant->empty();

        END_TRANSLATE_ERROR();
    }

    Variant^ Variant::Item::get(System::String^ key)
    {
        BEGIN_TRANSLATE_ERROR();

        STRONG_REFERENCE(this);

        std::string key_str(StringTranslator(key).c_str());

        return gcnew Variant(m_variant->at(key_str));

        END_TRANSLATE_ERROR();
    }

    void Variant::Item::set(System::String^ key, Variant^ value)
    {
        BEGIN_TRANSLATE_ERROR();

        STRONG_REFERENCE(this);
        STRONG_REFERENCE(value);

        std::string key_str(StringTranslator(key).c_str());

        m_variant->at(key_str) = value->get_internals();

        END_TRANSLATE_ERROR();
    }

    Variant^ Variant::Item::get(System::Int32 index)
    {
        BEGIN_TRANSLATE_ERROR();

        STRONG_REFERENCE(this);

        return gcnew Variant(m_variant->at(index));

        END_TRANSLATE_ERROR();
    }

    void Variant::Item::set(System::Int32 index, Variant^ value)
    {
        BEGIN_TRANSLATE_ERROR();

        STRONG_REFERENCE(this);

        m_variant->at(index) = value->get_internals();

        END_TRANSLATE_ERROR();
    }

    Variant^ Variant::Range(System::String^ key)
    {
        BEGIN_TRANSLATE_ERROR();

        STRONG_REFERENCE(this);

        std::string key_str(StringTranslator(key).c_str());

        return gcnew Variant(m_variant->range(key_str));

        END_TRANSLATE_ERROR();
    }

    Variant^ Variant::Select(System::String^ path)
    {
        BEGIN_TRANSLATE_ERROR();

        STRONG_REFERENCE(this);

        std::string path_str(StringTranslator(path).c_str());

        return gcnew Variant(m_variant->select(path_str));

        END_TRANSLATE_ERROR();
    }

    System::Collections::Generic::IEnumerator<VariantItem^>^ Variant::GetEnumerator_Generic()
    {
        BEGIN_TRANSLATE_ERROR();

        return gcnew VariantEnumerator(this);

        END_TRANSLATE_ERROR();
    }
        
    System::Collections::IEnumerator^ Variant::GetEnumerator_NonGeneric()
    {
        return GetEnumerator_Generic();
    }

    System::String^ Variant::ToString()
    {
        BEGIN_TRANSLATE_ERROR();

        return ToString(false);

        END_TRANSLATE_ERROR();
    }

    System::String^ Variant::ToString(bool summarise)
    {
        BEGIN_TRANSLATE_ERROR();

        STRONG_REFERENCE(this);

        return gcnew System::String(m_variant->str(summarise).c_str());

        END_TRANSLATE_ERROR();
    }

    System::String^ Variant::TypeToString(EnumType type)
    {
        BEGIN_TRANSLATE_ERROR();

        return gcnew System::String(
            protean::variant::enum_to_string(static_cast<protean::variant::enum_type_t>(type)).c_str()
        );

        END_TRANSLATE_ERROR();
    }

    Variant::EnumType Variant::Type::get()
    {
        BEGIN_TRANSLATE_ERROR();

        STRONG_REFERENCE(this);

        return static_cast<Variant::EnumType>(m_variant->type());

        END_TRANSLATE_ERROR();
    }

    System::TypeCode Variant::GetTypeCode()
    {
        return System::TypeCode::Object;
    }

    bool Variant::ToBoolean(System::IFormatProvider^ /*provider*/ )
    {
        return AsBoolean();
    }

    System::Byte Variant::ToByte(System::IFormatProvider^ /*provider*/)
    {
        throw gcnew VariantException("Variant does not support ToByte");
        return 0;
    }

    System::Char Variant::ToChar(System::IFormatProvider^ /*provider*/)
    {
        throw gcnew VariantException("Variant does not support ToChar");
        return 0;
    }

    System::DateTime Variant::ToDateTime(System::IFormatProvider^ /*provider*/)
    {
        return AsDateTime();
    }

    System::Decimal Variant::ToDecimal(System::IFormatProvider^ /*provider*/)
    {
        throw gcnew VariantException("Variant does not support ToDecimal");
        return 0;
    }

    double Variant::ToDouble(System::IFormatProvider^ /*provider*/)
    {
        return AsDouble();
    }

    short Variant::ToInt16(System::IFormatProvider^ /*provider*/)
    {
        throw gcnew VariantException("Variant does not support ToInt16");
        return 0;
    }

    int Variant::ToInt32(System::IFormatProvider^ /*provider*/)
    {
        return AsInt32();
    }

    System::Int64 Variant::ToInt64(System::IFormatProvider^ /*provider*/)
    {
        return AsInt64();
    }

    signed char Variant::ToSByte(System::IFormatProvider^ /*provider*/)
    {
        throw gcnew VariantException("Variant does not support ToSByte");
        return 0;
    }

    float Variant::ToSingle(System::IFormatProvider^ /*provider*/)
    {
        throw gcnew VariantException("Variant does not support ToSingle");
        return 0;
    }

    System::String^ Variant::ToString(System::IFormatProvider^ /*provider*/)
    {
        return AsString();
    }

    System::Object^ Variant::ToType(System::Type^ conversionType, System::IFormatProvider^ /*provider*/)
    {
        array<System::Type^>^ types = gcnew array<System::Type^>(1);

        try
        {
            types[0] = Variant::typeid;
            System::Reflection::MethodInfo^ conversionMethod = conversionType->GetMethod("op_Implicit", types);

            if (conversionMethod==nullptr)
            {
                return nullptr;
            }

            array<System::Object^>^ args = gcnew array<System::Object^>(1);

            try
            {
                args[0] = this;
                return conversionMethod->Invoke(nullptr, args);
            }
            finally
            {
                delete args;
            }
        }
        finally
        {
            delete types;
        }
    }

    System::UInt16 Variant::ToUInt16(System::IFormatProvider^ /*provider*/)
    {
        throw gcnew VariantException("Variant does not support ToUInt16");
        return 0;
    }

    System::UInt32 Variant::ToUInt32(System::IFormatProvider^ /*provider*/)
    {
        return AsUInt32();
    }

    System::UInt64 Variant::ToUInt64(System::IFormatProvider^ /*provider*/)
    {
        return AsUInt64();
    }

    System::Int32 Variant::CompareTo(Object^ obj)
    {
        if (obj->GetType()==Variant::typeid)
        {
            Variant^ temp = dynamic_cast<Variant^>(obj);

            return CompareTo(temp);
        }
        throw gcnew System::ArgumentException("Object is not a Variant");
    }

    System::Int32 Variant::CompareTo(Variant^ value)
    {
        BEGIN_TRANSLATE_ERROR();

        STRONG_REFERENCE(this);
        STRONG_REFERENCE(value);

        return get_internals().compare(value->get_internals());

        END_TRANSLATE_ERROR();
    }

    bool Variant::Equals(Variant^ value)
    {
        return CompareTo(value)==0;
    }

}} // protean::clr
