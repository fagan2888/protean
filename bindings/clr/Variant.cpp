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

using namespace System::Runtime::InteropServices;

namespace protean { namespace clr {

	Variant::Variant() :
		m_variant(new protean::variant()),
		m_reference(false)
	{
	}

	Variant::Variant(Variant^ arg) :
		m_reference(false),
		m_variant(new protean::variant(arg->get_internals()))
	{
	}

	Variant::Variant(EnumType type) :
		m_reference(false)
	{
        BEGIN_TRANSLATE_ERROR();

		m_variant = new protean::variant((protean::variant::enum_type_t)type);

        END_TRANSLATE_ERROR();
	}
	Variant::Variant(bool arg) :
		m_reference(false)
	{
        BEGIN_TRANSLATE_ERROR();

		m_variant = new protean::variant(arg);

        END_TRANSLATE_ERROR();
	}
	Variant::Variant(System::String^ arg) :
		m_reference(false)
	{
        BEGIN_TRANSLATE_ERROR();

        System::IntPtr arg_handle = Marshal::StringToHGlobalAnsi(arg);

        try
        {
            const char* arg_str = static_cast<const char*>(arg_handle.ToPointer());
		    m_variant = new protean::variant(arg_str);
        }
        finally
        {
            Marshal::FreeHGlobal(arg_handle);
        }

        END_TRANSLATE_ERROR();
	}
	Variant::Variant(System::Int32 arg) :
		m_reference(false)
	{
        BEGIN_TRANSLATE_ERROR();

		m_variant = new protean::variant(arg);

        END_TRANSLATE_ERROR();
	}
	Variant::Variant(System::UInt32 arg) :
		m_reference(false)
	{
        BEGIN_TRANSLATE_ERROR();

		m_variant = new protean::variant(arg);

        END_TRANSLATE_ERROR();
	}
	Variant::Variant(System::Int64 arg) :
		m_reference(false)
	{
        BEGIN_TRANSLATE_ERROR();

		m_variant = new protean::variant(arg);

        END_TRANSLATE_ERROR();
	}
	Variant::Variant(System::UInt64 arg) :
		m_reference(false)
	{
        BEGIN_TRANSLATE_ERROR();

		m_variant = new protean::variant(arg);

        END_TRANSLATE_ERROR();
	}

	Variant::Variant(float arg) :
		m_reference(false)
	{
        BEGIN_TRANSLATE_ERROR();

		m_variant = new protean::variant(arg);

        END_TRANSLATE_ERROR();
	}

	Variant::Variant(double arg) :
		m_reference(false)
	{
        BEGIN_TRANSLATE_ERROR();

		m_variant = new protean::variant(arg);

        END_TRANSLATE_ERROR();
	}

	Variant::Variant(System::TimeSpan arg) :
		m_reference(false)
	{
        BEGIN_TRANSLATE_ERROR();

		protean::variant::time_t time(arg.Hours, arg.Minutes, arg.Seconds);
		m_variant = new protean::variant(time);

        END_TRANSLATE_ERROR();
	}

	Variant::Variant(System::DateTime arg) :
		m_reference(false)
	{
        BEGIN_TRANSLATE_ERROR();

		protean::variant::date_t date(arg.Date.Year, arg.Date.Month, arg.Date.Month);
		protean::variant::time_t time(arg.TimeOfDay.Hours, arg.TimeOfDay.Minutes, arg.TimeOfDay.Seconds);
		protean::variant::date_time_t dateTime(date, time);
		m_variant = new protean::variant(dateTime);

        END_TRANSLATE_ERROR();
	}

	Variant::Variant(System::DateTime arg, bool ignoreTime) :
		m_reference(false)
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
	Variant::Variant(array<System::Byte>^ arg) :
		m_reference(false)
	{
        BEGIN_TRANSLATE_ERROR();

		pin_ptr<System::Byte> pinned(&arg[0]);
		m_variant = new protean::variant(pinned, arg->Length);

        END_TRANSLATE_ERROR();
	}

	Variant::Variant(IVariantObject^ arg) :
		m_reference(false)
	{
        BEGIN_TRANSLATE_ERROR();

		VariantObjectProxy^ proxy = gcnew VariantObjectProxy(arg->ClassName);
		proxy->Inflate(arg->Deflate(), arg->Version);

		m_variant = new protean::variant(proxy->get_internals());

        END_TRANSLATE_ERROR();
	}

	Variant::Variant(VariantObjectProxy^ arg) :
		m_reference(false)
	{
        BEGIN_TRANSLATE_ERROR();

		m_variant = new protean::variant(arg->get_internals());

        END_TRANSLATE_ERROR();
	}

    Variant::Variant(System::Exception^ arg) :
		m_reference(false)
    {
        BEGIN_TRANSLATE_ERROR();

        System::IntPtr type_handle = Marshal::StringToHGlobalAnsi(arg->GetType()->ToString());
        System::IntPtr message_handle = Marshal::StringToHGlobalAnsi(arg->Message);
        System::IntPtr source_handle = Marshal::StringToHGlobalAnsi(arg->Source);
        System::IntPtr stack_handle = Marshal::StringToHGlobalAnsi(arg->StackTrace);

        try
        {
            const char* type = static_cast<const char*>(type_handle.ToPointer());
            const char* message = static_cast<const char*>(message_handle.ToPointer());
            const char* source = static_cast<const char*>(source_handle.ToPointer());
            const char* stack = static_cast<const char*>(stack_handle.ToPointer());

            m_variant = new protean::variant(protean::exception_data(type, message, source, stack));
        }
        finally
        {
            Marshal::FreeHGlobal(type_handle);
            Marshal::FreeHGlobal(message_handle);
            Marshal::FreeHGlobal(source_handle);
            Marshal::FreeHGlobal(stack_handle);
        }

        END_TRANSLATE_ERROR();
    }

    Variant::Variant(ExceptionInfo^ arg) :
		m_reference(false)
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
		if (!m_reference)
		{
			delete m_variant;
		}
	}

	bool Variant::Is(EnumType type)
    {
        BEGIN_TRANSLATE_ERROR();

        return (m_variant->type() & static_cast<protean::variant::enum_type_t>(type))!=0;

        END_TRANSLATE_ERROR();
    }

    System::String^ Variant::AsString()
	{
        BEGIN_TRANSLATE_ERROR();

		return gcnew System::String(m_variant->as<std::string>().c_str());

        END_TRANSLATE_ERROR();
	}
	bool Variant::AsBoolean()
	{
        BEGIN_TRANSLATE_ERROR();

		return m_variant->as<bool>();

        END_TRANSLATE_ERROR();
	}
	System::Int32 Variant::AsInt32()
	{
        BEGIN_TRANSLATE_ERROR();

		return m_variant->as<boost::int32_t>();

        END_TRANSLATE_ERROR();
	}
	System::UInt32 Variant::AsUInt32()
	{
        BEGIN_TRANSLATE_ERROR();

		return m_variant->as<boost::uint32_t>();

        END_TRANSLATE_ERROR();
	}
	System::Int64 Variant::AsInt64()
	{
        BEGIN_TRANSLATE_ERROR();

		return m_variant->as<boost::int64_t>();

        END_TRANSLATE_ERROR();
	}
	System::UInt64 Variant::AsUInt64()
	{
        BEGIN_TRANSLATE_ERROR();

		return m_variant->as<boost::uint64_t>();

        END_TRANSLATE_ERROR();
	}
	float Variant::AsFloat()
	{
        BEGIN_TRANSLATE_ERROR();

		return m_variant->as<float>();

        END_TRANSLATE_ERROR();
	}
	double Variant::AsDouble()
	{
        BEGIN_TRANSLATE_ERROR();

		return m_variant->as<double>();

        END_TRANSLATE_ERROR();
	}
	System::DateTime Variant::AsDate()
	{
        BEGIN_TRANSLATE_ERROR();

		protean::variant::date_t date(m_variant->as<protean::variant::date_t>());
		return System::DateTime(date.year(), date.month(), date.day());

        END_TRANSLATE_ERROR();
	}
	System::TimeSpan Variant::AsTime()
	{
        BEGIN_TRANSLATE_ERROR();

		protean::variant::time_t t(m_variant->as<protean::variant::time_t>());
		return System::TimeSpan(t.hours(), t.minutes(), t.seconds());

        END_TRANSLATE_ERROR();
	}
	System::DateTime Variant::AsDateTime()
	{
        BEGIN_TRANSLATE_ERROR();

		protean::variant::date_time_t dt(m_variant->as<protean::variant::date_time_t>());
		return System::DateTime(dt.date().year(), dt.date().month(), dt.date().day(), dt.time_of_day().hours(), dt.time_of_day().minutes(), dt.time_of_day().seconds() );

        END_TRANSLATE_ERROR();
	}
	array<System::Byte>^ Variant::AsBuffer()
	{
        BEGIN_TRANSLATE_ERROR();

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

		const protean::object& obj(m_variant->as<protean::object>());

		T newObj = gcnew T();

		if (newObj->ClassName->Equals(gcnew System::String(obj.name().c_str())))
		{
			protean::variant params;
			obj.deflate(params);
			newObj->Inflate(gcnew Variant(params), obj.version());
		}
		else
		{
            System::IntPtr className_handle = Marshal::StringToHGlobalAnsi(newObj->ClassName);

            const char* className = static_cast<const char*>(className_handle.ToPointer());
    		std::ostringstream oss;
			oss << "Attempt to coerce object of type '" << obj.name() << "' into '" << className << "'.";

            Marshal::FreeHGlobal(className_handle);

			throw gcnew VariantException(gcnew System::String(oss.str().c_str()));
		}

		return newObj;

        END_TRANSLATE_ERROR();
	}

	VariantObjectProxy^ Variant::AsObject()
	{
        BEGIN_TRANSLATE_ERROR();

		return gcnew VariantObjectProxy(m_variant->as<protean::object_proxy>());

        END_TRANSLATE_ERROR();
	}

	ExceptionInfo^ Variant::AsException()
	{
        BEGIN_TRANSLATE_ERROR();

		return gcnew ExceptionInfo(m_variant->as<protean::exception_data>());

        END_TRANSLATE_ERROR();
	}

	void Variant::Add(Variant^ value)
	{
        BEGIN_TRANSLATE_ERROR();

		m_variant->push_back(value->get_internals());

        END_TRANSLATE_ERROR();
	}

	bool Variant::ContainsKey(System::String^ key)
	{
        BEGIN_TRANSLATE_ERROR();

        System::IntPtr key_handle = Marshal::StringToHGlobalAnsi(key);

        try
        {
            const char* key_str = static_cast<const char*>(key_handle.ToPointer());
		    return m_variant->has_key(key_str);
        }
        finally
        {
            Marshal::FreeHGlobal(key_handle);
        }

        END_TRANSLATE_ERROR();
	}

	void Variant::Add(System::String^ key, Variant^ value)
	{
        BEGIN_TRANSLATE_ERROR();

        System::IntPtr key_handle = Marshal::StringToHGlobalAnsi(key);

        try
        {
            const char* key_str = static_cast<const char*>(key_handle.ToPointer());
            m_variant->insert(key_str, value->get_internals());
        }
        finally
        {
            Marshal::FreeHGlobal(key_handle);
        }

        END_TRANSLATE_ERROR();
	}

	void Variant::Remove(System::String^ key)
	{
        BEGIN_TRANSLATE_ERROR();

        System::IntPtr key_handle = Marshal::StringToHGlobalAnsi(key);

        try
        {
            const char* key_str = static_cast<const char*>(key_handle.ToPointer());
            m_variant->remove(key_str);
        }
        finally
        {
            Marshal::FreeHGlobal(key_handle);
        }

        END_TRANSLATE_ERROR();
	}

	void Variant::Add(System::DateTime index, Variant^ value)
	{
        BEGIN_TRANSLATE_ERROR();

		protean::variant::date_t date(index.Date.Year, index.Date.Month, index.Date.Month);
		protean::variant::time_t time(index.TimeOfDay.Hours, index.TimeOfDay.Minutes, index.TimeOfDay.Seconds);
		protean::variant::date_time_t dateTime(date,time);

		m_variant->push_back(dateTime, value->get_internals());

        END_TRANSLATE_ERROR();
	}

	int Variant::Count::get()
	{
        BEGIN_TRANSLATE_ERROR();

		return static_cast<int>(m_variant->size());

        END_TRANSLATE_ERROR();
	}

	bool Variant::Empty::get()
	{
        BEGIN_TRANSLATE_ERROR();

		return m_variant->empty();

        END_TRANSLATE_ERROR();
	}

	Variant^ Variant::Item::get(System::String^ key)
	{
        BEGIN_TRANSLATE_ERROR();

        System::IntPtr key_handle = Marshal::StringToHGlobalAnsi(key);

        try
        {
            const char* key_str = static_cast<const char*>(key_handle.ToPointer());
		    return gcnew Variant(&m_variant->at(key_str));
        }
        finally
        {
            Marshal::FreeHGlobal(key_handle);
        }

        END_TRANSLATE_ERROR();
	}

	void Variant::Item::set(System::String^ key, Variant^ value)
	{
        BEGIN_TRANSLATE_ERROR();

        System::IntPtr key_handle = Marshal::StringToHGlobalAnsi(key);

        try
        {
            const char* key_str = static_cast<const char*>(key_handle.ToPointer());
		    m_variant->at(key_str) = value->get_internals();
        }
        finally
        {
            Marshal::FreeHGlobal(key_handle);
        }

        END_TRANSLATE_ERROR();
	}

	Variant^ Variant::Item::get(System::UInt32 index)
	{
        BEGIN_TRANSLATE_ERROR();

		return gcnew Variant(&m_variant->at(index));

        END_TRANSLATE_ERROR();
	}

	void Variant::Item::set(System::UInt32 index, Variant^ value)
	{
        BEGIN_TRANSLATE_ERROR();

		m_variant->at(index) = value->get_internals();

        END_TRANSLATE_ERROR();
	}

	System::Collections::IEnumerator^ Variant::GetEnumerator()
	{
        BEGIN_TRANSLATE_ERROR();

		return gcnew VariantEnumerator(this);

        END_TRANSLATE_ERROR();
	}

	System::String^ Variant::ToString()
	{
        BEGIN_TRANSLATE_ERROR();

		return gcnew System::String(m_variant->str().c_str());

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

        return static_cast<Variant::EnumType>(m_variant->type());

        END_TRANSLATE_ERROR();
    }

	System::TypeCode Variant::GetTypeCode()
	{
        BEGIN_TRANSLATE_ERROR();

		return System::TypeCode::Object;

        END_TRANSLATE_ERROR();
	}

	bool Variant::ToBoolean(System::IFormatProvider^ /*provider*/ )
	{
        BEGIN_TRANSLATE_ERROR();

		return AsBoolean();

        END_TRANSLATE_ERROR();
	}

	System::Byte Variant::ToByte(System::IFormatProvider^ /*provider*/)
	{
        BEGIN_TRANSLATE_ERROR();

		throw gcnew VariantException("Variant does not support ToByte");
		return 0;

        END_TRANSLATE_ERROR();
	}

	System::Char Variant::ToChar(System::IFormatProvider^ /*provider*/)
	{
        BEGIN_TRANSLATE_ERROR();

		throw gcnew VariantException("Variant does not support ToChar");
		return 0;

        END_TRANSLATE_ERROR();
	}

	System::DateTime Variant::ToDateTime(System::IFormatProvider^ /*provider*/)
	{
        BEGIN_TRANSLATE_ERROR();

		return AsDateTime();

        END_TRANSLATE_ERROR();
	}

	System::Decimal Variant::ToDecimal(System::IFormatProvider^ /*provider*/)
	{
        BEGIN_TRANSLATE_ERROR();

		throw gcnew VariantException("Variant does not support ToDecimal");
		return 0;

        END_TRANSLATE_ERROR();
	}

	double Variant::ToDouble(System::IFormatProvider^ /*provider*/)
	{
        BEGIN_TRANSLATE_ERROR();

		return AsDouble();

        END_TRANSLATE_ERROR();
	}

	short Variant::ToInt16(System::IFormatProvider^ /*provider*/)
	{
        BEGIN_TRANSLATE_ERROR();

		throw gcnew VariantException("Variant does not support ToInt16");
		return 0;

        END_TRANSLATE_ERROR();
	}

	int Variant::ToInt32(System::IFormatProvider^ /*provider*/)
	{
        BEGIN_TRANSLATE_ERROR();

		return AsInt32();

        END_TRANSLATE_ERROR();
	}

	System::Int64 Variant::ToInt64(System::IFormatProvider^ /*provider*/)
	{
        BEGIN_TRANSLATE_ERROR();

		return AsInt64();

        END_TRANSLATE_ERROR();
	}

	signed char Variant::ToSByte(System::IFormatProvider^ /*provider*/)
	{
        BEGIN_TRANSLATE_ERROR();

		throw gcnew VariantException("Variant does not support ToSByte");
		return 0;

        END_TRANSLATE_ERROR();
	}

	float Variant::ToSingle(System::IFormatProvider^ /*provider*/)
	{
        BEGIN_TRANSLATE_ERROR();

		throw gcnew VariantException("Variant does not support ToSingle");
		return 0;

        END_TRANSLATE_ERROR();
	}

	System::String^ Variant::ToString(System::IFormatProvider^ /*provider*/)
	{
        BEGIN_TRANSLATE_ERROR();

		return AsString();

        END_TRANSLATE_ERROR();
	}

	System::Object^ Variant::ToType(System::Type^ conversionType, System::IFormatProvider^ /*provider*/)
	{
        BEGIN_TRANSLATE_ERROR();

		array<System::Type^>^types = gcnew array<System::Type^>(1);
		types[0] = Variant::typeid;

		System::Reflection::MethodInfo^ conversionMethod = conversionType->GetMethod("op_Implicit", types);

		if (conversionMethod==nullptr)
		{
			return nullptr;
		}
		
		array<System::Object^>^args = gcnew array<System::Object^>(1);
		args[0] = this;

		return conversionMethod->Invoke(nullptr, args);

        END_TRANSLATE_ERROR();
	}

	System::UInt16 Variant::ToUInt16(System::IFormatProvider^ /*provider*/)
	{
        BEGIN_TRANSLATE_ERROR();

		throw gcnew VariantException("Variant does not support ToUInt16");
		return 0;

        END_TRANSLATE_ERROR();
	}

	System::UInt32 Variant::ToUInt32(System::IFormatProvider^ /*provider*/)
	{
        BEGIN_TRANSLATE_ERROR();

		return AsUInt32();

        END_TRANSLATE_ERROR();
	}

	System::UInt64 Variant::ToUInt64(System::IFormatProvider^ /*provider*/)
	{
        BEGIN_TRANSLATE_ERROR();

		return AsUInt64();

        END_TRANSLATE_ERROR();
	}

	System::Int32 Variant::CompareTo(Object^ obj)
	{
        BEGIN_TRANSLATE_ERROR();

		if (obj->GetType()==Variant::typeid)
		{
			Variant^ temp = dynamic_cast<Variant^>(obj);

			return CompareTo(temp);
		}
		throw gcnew System::ArgumentException("Object is not a Variant");

		END_TRANSLATE_ERROR();
    }

	System::Int32 Variant::CompareTo(Variant^ obj)
	{
        BEGIN_TRANSLATE_ERROR();

		return get_internals().compare(obj->get_internals());

		END_TRANSLATE_ERROR();
	}

	bool Variant::Equals(Variant^ obj)
	{
        BEGIN_TRANSLATE_ERROR();

		return CompareTo(obj)==0;

		END_TRANSLATE_ERROR();
	}

}} // protean::clr
