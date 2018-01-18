//  (C) Copyright Johan Ditmar, Karel Hruda, Paul O'Neill & Luke Stedman 2009.
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

using System;
using System.Collections.Generic;
using System.Text;
using System.Text.RegularExpressions;
using System.Data;
using System.Reflection;

namespace Protean {

    public class Variant :
        VariantBase,
        IEnumerable<VariantItem>,
        IConvertible,
        IEquatable<Variant>,
        IComparable<Variant>,
        IComparable
    {
        public Variant()
        { } 

        public Variant(Variant arg) :
            base(arg)
        { }

        public Variant(EnumType type) :
            base(type, 0)
        { }

        public Variant(EnumType type, string value) :
            base(type, value)
        { }

        public Variant(EnumType type, int size) :
            base(type, size)
        { }

        public Variant(string arg) {
            Value = new VariantPrimitive<String>(arg);
        }

        public Variant(bool arg) {
            Value = new VariantPrimitive<bool>(arg);
        }
        public Variant(Int32 arg) {
            Value = new VariantPrimitive<Int32>(arg);
        }

        public Variant(UInt32 arg) {
            Value = new VariantPrimitive<UInt32>(arg);
        }

        public Variant(Int64 arg) {
            Value = new VariantPrimitive<Int64>(arg);
        }

        public Variant(UInt64 arg) {
            Value = new VariantPrimitive<UInt64>(arg);
        }

        public Variant(float arg) {
            Value = new VariantPrimitive<float>(arg);
        }

        public Variant(double arg) {
            Value = new VariantPrimitive<double>(arg);
        }

        public Variant(TimeSpan arg) {
            Value = new VariantPrimitive<TimeSpan>(arg);
        }

        public Variant(DateTime arg) {
            Value = new VariantPrimitive<DateTime>(arg);
        }

        public Variant(byte[] arg) {
            Value = new VariantBuffer(arg);
        }

        public Variant(IVariantObject arg)  {
            Value = new VariantObjectData(arg);
        }

        public Variant(System.Exception arg) :
            this(new VariantExceptionInfo(arg))
        {  }

        public Variant(VariantExceptionInfo arg) {
            Value = arg;
        }

        public Variant(DataTable arg)
        {
            Value = new VariantDataTable(arg);
        }

        public Variant(TypedArray arg)
        {
            Value = arg;
        }

        public IVariantObject AsObject()
        {
            CheckType(EnumType.Object, "AsObject()");

            return ((VariantObjectData)Value).Value;
        }

        public T AsObject<T>()
            where T : class, IVariantObject, new()
        {
            CheckType(EnumType.Object, "AsObject()");

            IVariantObject o = ((VariantObjectData)Value).Value;

            T result = o as T;

            if (result==null)
            {
                result = new T();

                if (o.Class != result.Class)
                {
                    throw new VariantException(string.Format("Attempt to coerce object of type {0} into {1}", o.Class, result.Class));
                }

                Variant param = o.Deflate();
                result.Inflate(param, o.Version);

                if (o is VariantObjectProxy)
                {
                    Value = new VariantObjectData(result);
                }
            }

            return result;
        }

        public VariantExceptionInfo AsException()
        {
            CheckType(EnumType.Exception, "AsException()");

            return Value as VariantExceptionInfo;
        }

        public byte[] AsBuffer()
        {
            CheckType(EnumType.Buffer | EnumType.Any, "AsBuffer()");

            if (Value is VariantBuffer)
            {
                return ((VariantBuffer)Value).Value;
            }
            else
            {
                return System.Convert.FromBase64String(((VariantAny)Value).Value);
            }
        }

        public DataTable AsDataTable()
        {
            CheckType(EnumType.DataTable, "AsDataTable()");

            return ((VariantDataTable)Value).Value;
        }

        public TypedArray AsArray()
        {
            CheckType(EnumType.Array, "AsArray()");

            return (TypedArray)Value;
        }

        // Lists
        public void Add(Variant value)
        {
            CheckType(EnumType.List, "Add");

            (Value as VariantList).Value.Add(value);
        }

        public void AddRange(IEnumerable<Variant> collection)
        {
            CheckType(EnumType.List, "AddRange");

            (Value as VariantList).Value.AddRange(collection);
        }

        // Bags/Dictionaries
        public bool ContainsKey(string key)
        {
            CheckType(EnumType.Mapping, "ContainsKey('" + key + "')");

            return (Value as IVariantMapping).ContainsKey(key);
        }

        public void Add(string key, Variant value)
        {
            CheckType(EnumType.Mapping, "Add('" + key + "')");

            (Value as IVariantMapping).Add(key, value);
        }

        public void Remove(string key)
        {
            CheckType(EnumType.Mapping, "Remove('" + key + "')");

            (Value as IVariantMapping).Remove(key);
        }

        public Variant Range(string key)
        {
            CheckType(EnumType.Mapping, "Range('" + key + "')");

            return (Value as IVariantMapping).Range(key);
        }

        // TimeSeries
        public void Add(DateTime time, Variant value)
        {
            CheckType(EnumType.TimeSeries, "Add");

            (Value as VariantTimeSeries).Value.Add(new KeyValuePair<DateTime, Variant>(time, value));
        }

        public int Count
        {
            get {
                CheckType(EnumType.Collection | EnumType.Any, "Count");

                if (Value is IVariantCollection)
                {
                    return ((IVariantCollection)Value).Count;
                }
                else
                {
                    VariantAny any = (VariantAny)Value;
                    if (any.Value.Length==0)
                    {
                        return 0;
                    }
                    else
                    {
                        throw new VariantException("Attempt to call Count on non-empty primitive");
                    }
                }

            }
        }

        public void Clear()
        {
            CheckType(EnumType.Collection, "Clear");

            ((IVariantCollection)Value).Clear();
        }

        public bool Empty
        {
            get
            {
                CheckType(EnumType.Collection | EnumType.Any, "Empty");

                if (Value is IVariantCollection)
                {
                    return ((IVariantCollection)Value).Count == 0;
                }
                else
                {
                    return ((VariantAny)Value).Value.Length == 0;
                }
            }
        }

        public Variant this[string key]
        {
            get {
                CheckType(EnumType.Mapping, "this['" + key + "']");

                return (Value as IVariantMapping)[key];
            }
            set {
                CheckType(EnumType.Mapping, "this['" + key + "']");

                (Value as IVariantMapping)[key] = value;
            }
        }

        public Variant this[int index]
        {
            
            get {
                CheckType(EnumType.Sequence, "this[" + index + "]");

                return (Value as IVariantSequence)[index];
            }

            set  {
                CheckType(EnumType.Sequence, "this[" + index + "]");

                (Value as IVariantSequence)[index] = value;
            }
        }

        public Variant AnyCast()
        {
            CheckType(EnumType.Primitive, "AnyCast");

            if (Value is VariantAny)
            {
                return this;
            }
            else
            {
                return new Variant(EnumType.Any, Value.ToString());
            }
        }

        private StringBuilder ToString(bool summarise, string indent, StringBuilder sb)
        {
            const string tab = "   ";
            const string noIndent = "";

            sb.Append(indent);

            switch (Type)
            {
                case EnumType.None:
                    sb.Append("None");
                    break;
                case EnumType.Any:
                    sb.Append("Any('" + As<string>() + "')");
                    break;
                case EnumType.String:
                    sb.Append("'" + As<string>() + "'");
                    break;
                case EnumType.Int32:
                case EnumType.UInt32:
                case EnumType.Int64:
                case EnumType.UInt64:
                case EnumType.Float:
                case EnumType.Double:
                case EnumType.Boolean:
                case EnumType.Date:
                case EnumType.Time:
                case EnumType.DateTime:
                    sb.Append(Value);
                    break;
                case EnumType.List:
                    {
                        if (summarise)
                        {
                            sb.Append("List(size=" + Count.ToString() + ")");
                        }
                        else
                        {
                            sb.Append("[\n");
                            int count = Count;
                            foreach (VariantItem item in this)
                            {
                                item.Value.ToString(false, indent + tab, sb);
                                if (--count > 0)
                                {
                                    sb.Append(",");
                                }
                                sb.Append("\n");
                            }
                            sb.Append(indent + "]");
                        }
                        break;
                    }
                case EnumType.Dictionary:
                    {
                        if (summarise)
                        {
                            sb.Append("Dictionary(size=" + Count.ToString() + ")");
                        }
                        else
                        {
                            sb.Append("{\n");
                            int count = Count;
                            foreach (VariantItem item in this)
                            {
                                sb.Append(indent + tab + item.Key + ": ");
                                if (item.Value.Is(EnumType.Primitive | EnumType.None))
                                {
                                    item.Value.ToString(false, noIndent, sb);
                                }
                                else
                                {
                                    sb.Append("\n");
                                    item.Value.ToString(false, indent + tab + tab, sb);
                                }
                                if (--count != 0)
                                {
                                    sb.Append(",");
                                }
                                sb.Append("\n");
                            }
                            sb.Append(indent + "}");
                        }
                        break;
                    }
                case EnumType.Bag:
                    {
                        if (summarise)
                        {
                            sb.Append("Bag(size=" + Count + ")");
                        }
                        else
                        {
                            sb.Append("[\n");
                            int count = Count;
                            foreach (VariantItem item in this)
                            {
                                sb.Append(indent + tab + "(" + item.Key + ", ");
                                if (item.Value.Is(EnumType.Primitive))
                                {
                                    item.Value.ToString(false, noIndent, sb);
                                }
                                else
                                {
                                    sb.Append("\n");
                                    item.Value.ToString(false, indent + tab, sb);
                                }
                                sb.Append(")");
                                if (--count != 0)
                                {
                                    sb.Append(",");
                                }
                                sb.Append("\n");
                            }
                            sb.Append(indent + "]");
                        }
                        break;
                    }
                case EnumType.TimeSeries:
                    {
                        if (summarise)
                        {
                            sb.Append("TimeSeries(size=" + Count + ")");
                        }
                        else
                        {
                            sb.Append("TimeSeries(\n");
                            int count = Count;
                            foreach (VariantItem item in this)
                            {
                                sb.Append(indent + tab + "(" + item.Time + ", ");
                                if (item.Value.Is(EnumType.Primitive))
                                {
                                    item.Value.ToString(false, noIndent, sb);
                                }
                                else
                                {
                                    sb.Append("\n");
                                    item.Value.ToString(false, indent + tab, sb);
                                }
                                sb.Append(")");
                                if (--count != 0)
                                {
                                    sb.Append(",");
                                }
                                sb.Append("\n");
                            }
                            sb.Append(indent + ")");
                        }
                        break;
                    }
                case EnumType.Buffer:
                    {
                        if (summarise)
                        {
                            sb.Append("Buffer(size=").Append(Count).Append(")");
                        }
                        else
                        {
                            sb.Append("Buffer(").Append(Encoding.ASCII.GetString(AsBuffer())).Append(')');
                        }
                        break;
                    }
                case EnumType.Tuple:
                    {
                        if (summarise)
                        {
                            sb.Append("Tuple(size=").Append(Count).Append(")");
                        }
                        else
                        {
                            sb.Append("(").AppendLine();
                            int count = Count;
                            foreach (VariantItem item in this)
                            {
                                item.Value.ToString(false, indent + tab, sb);
                                if (--count != 0)
                                {
                                    sb.Append(",");
                                }
                                sb.AppendLine();
                            }
                            sb.Append(indent).Append(")");
                        }
                        break;
                    }
                case EnumType.Exception:
                    {
                        VariantExceptionInfo x = AsException();

                        sb.Append(x.Class).Append("('").Append(x.Message).Append("')");

                        if (x.Source.Length > 0)
                        {
                            sb.Append(" in: ").Append(x.Source);
                        }

                        if (x.Stack.Length > 0)
                        {
                            sb.AppendLine().Append(x.Stack);
                        }
                        break;
                    }
                case EnumType.Object:
                    {
                        IVariantObject o = AsObject();

                        if (summarise)
                        {
                            sb.Append(o.Class).Append("(version=").Append(o.Version).Append(")");
                        }
                        else
                        {
                            sb.Append(o.Class).Append("(version=").Append(o.Version).Append(")(").AppendLine();
                            o.Deflate().ToString(false, indent + tab, sb);
                            sb.AppendLine().Append(indent).Append(")");
                        }
                        break;
                    }
                case EnumType.Array:
                    {
                        TypedArray a = AsArray();

                        if (summarise)
                        {
                            sb.Append("Array(size=").Append(a.Count).Append(", type=").Append(a.ElementType).Append(")");
                        }
                        else
                        {
                            sb.Append("Array(");

                            int lastIndex = a.Count - 1;
                            for (int i = 0; i < a.Count; ++i)
                            {
                                sb.Append(a[i].ToString()).Append(i != lastIndex ? "," : ")");
                            }
                        }
                        break;
                    }
                default:
                    sb.Append("UNKNOWN<").Append(Type).Append(">");
                    break;
            }
            return sb;
        }

        public override string ToString()
        {
            var sb = new StringBuilder();
            return ToString(false, "", sb).ToString();
        }

		public string ToString(string indent)
		{
			var sb = new StringBuilder();
			return ToString(false, indent, sb).ToString();
		}

        public new string ToString(bool summarise)
        {
            var sb = new StringBuilder();
            return ToString(summarise, "", sb).ToString();
        }

        public IEnumerator<VariantItem> GetEnumerator()
        {
            CheckType(EnumType.Collection | EnumType.Any, "GetEnumerator()");

            if (Value is IVariantCollection)
            {
                return ((IVariantCollection)Value).GetEnumerator();
            }
            else
            {
                if (Empty)
                {
                    return new DummyEnumerator();
                }
                else
                {
                    throw new VariantException("Attempt to enumerate Any variant with content");
                }
            }
        }

        System.Collections.IEnumerator System.Collections.IEnumerable.GetEnumerator()
        {
            return GetEnumerator();
        }

        // System::IConvertible
        public TypeCode GetTypeCode()
        {
            return TypeCode.Object;
        }
        public bool ToBoolean(IFormatProvider provider)
        {
            return As<bool>();
        }
        public byte ToByte(IFormatProvider provider)
        {
            throw new VariantException("Variant does not support ToByte");
        }
        public char ToChar(IFormatProvider provider)
        {
            throw new VariantException("Variant does not support ToChar");
        }
        public DateTime ToDateTime(IFormatProvider provider)
        {
            return As<DateTime>();
        }
        public decimal  ToDecimal(IFormatProvider provider)
        {
            throw new VariantException("Variant does not support ToDecimal");
        }
        public double ToDouble(IFormatProvider provider)
        {
            return As<Double>();
        }
        public short ToInt16(IFormatProvider provider)
        {
            throw new VariantException("Variant does not support ToInt16");
        }
        public int ToInt32(IFormatProvider provider)
        {
            return As<Int32>();
        }
        public long ToInt64(IFormatProvider provider)
        {
            return As<Int64>();
        }
        public sbyte  ToSByte(IFormatProvider provider)
        {
            throw new VariantException("Variant does not support ToSByte");
        }
        public float ToSingle(IFormatProvider provider)
        {
            return As<float>();
        }
        public String ToString(IFormatProvider provider)
        {
            return As<String>();
        }
        public Object ToType(Type conversionType, IFormatProvider provider)
        {
			if (conversionType == typeof(System.TimeSpan))
			{
				return As<System.TimeSpan>();
			}
			else
			{
				Type[] types = new Type[] { typeof(Variant) };

				MethodInfo conversionMethod = conversionType.GetMethod("op_Implicit", types);

				if (conversionMethod == null)
				{
					return null;
				}

				Object[] args = new Object[] { this };

				return conversionMethod.Invoke(null, args);
			}
        }
        public ushort ToUInt16(IFormatProvider provider)
        {
            throw new VariantException("Variant does not support ToUInt16");
        }
        public uint ToUInt32(IFormatProvider provider)
        {
            return As<UInt32>();
        }
        public ulong ToUInt64(IFormatProvider provider)
        {
            return As<UInt64>();
        }

        private static void SelectImpl(Variant input, string path, Variant result)
        {
            string head = "", tail = "";

            Match myMatch = Regex.Match(path, @"[^/]+");

            if (myMatch.Success)
            {
                head = myMatch.Value;
                tail = path.Substring(myMatch.Index + myMatch.Length);
            }
            else
            {
                result.Add(input);
                return;
            }

            string node = "", pred_key = "", pred_val = "";

            myMatch = Regex.Match(head, @"^(\*|\w+)(\[@?(\w+)=""?(\w+)""?\])?");
            if (myMatch.Success)
            {
                node = myMatch.Groups[1].Value;
                pred_key = myMatch.Groups[3].Value;
                pred_val = myMatch.Groups[4].Value;
            }
            else 
            {
                throw new VariantException("Select path has invalid syntax: " + head);
            }

            Variant nodes = new Variant(EnumType.List);
            if (node.Length==0 || node=="*")
            {
                foreach(VariantItem item in input)
                {
                    nodes.Add(item.Value);
                }
            }
            else if (input.Is(EnumType.Mapping))
            {
                foreach(VariantItem item in input.Range(node))
                {
                    nodes.Add(item.Value);
                }
            }

            foreach(VariantItem item in nodes)
            {
                if (pred_key.Length==0 || (item.Value.Is(EnumType.Mapping) && item.Value.ContainsKey(pred_key) && item.Value[pred_key].AnyCast().As<string>()==pred_val))
                {
                    SelectImpl(item.Value, tail, result);
                }
            }
        }

        public Variant Select(string path)
        {
            Variant result = new Variant(Variant.EnumType.List);
            SelectImpl(this, path, result);
            return result;
        }

        // System.CompareTo<Variant>
        public int CompareTo(Variant rhs)
        {
            if (Type!=rhs.Type)
            {
                return Type.CompareTo(rhs.Type);
            }

            return Value.CompareTo(rhs.Value);
        }

        public int CompareTo(object obj)
        {
            if (obj is Variant)
            {
                return CompareTo((Variant)obj);
            }

            throw new ArgumentException("Object is not a Variant");
        }

        // System.IEquatable<Variant>
        public bool Equals(Variant rhs)
        {
            if (ReferenceEquals(rhs, null)) return false;
            if (ReferenceEquals(rhs, this)) return true;

            // lazy, this could be implemented efficiently if required
            return CompareTo(rhs)==0;
        }

        public override bool Equals(object obj)
        {
            if (ReferenceEquals(obj, null)) return false;
            if (ReferenceEquals(obj, this)) return true;

            return Equals(obj as Variant);
        }

		public override int GetHashCode()
		{
			return  base.GetHashCode();
		}

        public static bool operator ==(Variant lhs, Variant rhs)
        {
            if (ReferenceEquals(lhs, null))
            {
                return ReferenceEquals(rhs, null);
            }

            return lhs.Equals(rhs);
        }

        public static bool operator !=(Variant lhs, Variant rhs)
        {
            return !(lhs == rhs);
        }

        // helpers
        public static EnumType StringToEnum(string value)
        {
            try
            {
                return (EnumType)Enum.Parse(typeof(EnumType), value, true);
            }
            catch
            {
                throw new VariantException(string.Format("Unable to convert {0} to valid variant enumeration", value));
            }
        }
        private void CheckType(EnumType type, string fn)
        {
            if (!Is(type))
            {
                throw new VariantException(string.Format("Attempt to call {0} on Variant of type {1}\n{2}", fn, Type, ToString()));
            }
        }

        public static readonly DateTime MinDateTime = new DateTime(1400, 1, 1);
        public static readonly DateTime MaxDateTime = new DateTime(9999, 12, 31, 23, 59, 59, 999);
    };

} // Protean
