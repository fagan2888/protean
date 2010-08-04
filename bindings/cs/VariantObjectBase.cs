//  (C) Copyright Johan Ditmar, Karel Hruda, Paul O'Neill & Luke Stedman 2009.
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

using System;
using System.Collections.Generic;
using System.Text;

namespace protean {

    public abstract class VariantObjectBase : IVariantData
    {
        public abstract string ClassName { get; }

        public abstract int Version { get; }

        public abstract Variant Deflate();

        public abstract void Inflate(Variant param, int version);

        public VariantBase.EnumType Type
        {
            get { return VariantBase.EnumType.Object; }
        }
    };

} // protean
