using System;
using System.Text;

namespace Protean {

    public class VariantExceptionInfo : IVariantData
    {
        public VariantExceptionInfo(string type, string message, string source, string stack)
        {
            Class = type;
            Message = message;
            Source = source;
            Stack = stack;
        }

        public VariantExceptionInfo(string type, string message) :
            this(type, message, "", "")
        { }

        public VariantExceptionInfo(VariantExceptionInfo rhs) :
            this(rhs.Class, rhs.Message, rhs.Source, rhs.Stack)
        { }

        public VariantExceptionInfo(Exception e) :
            this(e.GetType().ToString(), e.Message, e.Source, e.StackTrace)
        { }

        public VariantBase.EnumType _Type
        {
            get { return VariantBase.EnumType.Exception; }
        }

        public int CompareTo(IVariantData rhs)
        {
            VariantExceptionInfo rhsEx = (VariantExceptionInfo)rhs;
            if (Class != rhsEx.Class)
            {
                return Class.CompareTo(rhsEx.Class);
            }
            if (Message != rhsEx.Message)
            {
                return Message.CompareTo(rhsEx.Message);
            }
            if (Source != rhsEx.Source)
            {
                return Source.CompareTo(rhsEx.Source);
            }
            return Stack.CompareTo(rhsEx.Stack);
        }

        public override string ToString()
        {
            StringBuilder sb = new StringBuilder(Class + '(' + Message + ')');

            if (!string.IsNullOrEmpty(Source))
            {
                sb.Append(" in: " + Source);
            }

            if (!string.IsNullOrEmpty(Stack))
            {
                sb.Append("\n" + Stack);
            }

            return sb.ToString();
        }

        public override int GetHashCode()
        {
            unchecked
            {
                var hashCode = Class != null ? Class.GetHashCode() : 0;
                hashCode = hashCode * 397 ^ (Message?.GetHashCode() ?? 0);
                hashCode = hashCode * 397 ^ (Source?.GetHashCode() ?? 0);
                hashCode = hashCode * 397 ^ (Stack?.GetHashCode() ?? 0);
                return hashCode;
            }
        }

        public string Class { get; set; }
        public string Message { get; set; }
        public string Source { get; set; }
        public string Stack { get; set; }
    }

} // Protean
