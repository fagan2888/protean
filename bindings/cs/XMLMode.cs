//  (C) Copyright Johan Ditmar, Karel Hruda, Paul O'Neill & Luke Stedman 2009.
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

namespace protean
{
    public enum XMLMode
    {
        None                = 0x00000000,
        Preserve            = 0x00000001,
        NoHeader            = 0x00000002,
        Indent              = 0x00000004,
        StripSpace          = 0x00000008,
        Default             = None
    }

    public abstract class XMLConst 
    {
        public const string Default = "Variant";
        public const string Text = "__text__";
        public const string Comment = "__comment__";
        public const string Instruction = "__instruction__";
        public const string Attributes = "__attributes__";
        public const string Target = "__target__";
        public const string Data = "__data__";
    };

} // protean
