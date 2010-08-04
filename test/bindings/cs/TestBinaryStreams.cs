﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace protean.test
{
    using NUnit.Framework;
    using protean;

    [TestFixture]
    public class TestBinaryStreams
    {
        [Test]
        public void TestPrimitives()
        {
            Int32 argInt32 = -1;
            UInt32 argUInt32 = 0xffffffff;
            Int64 argInt64 = -1;
            UInt64 argUInt64 = 0xffffffffffffffff;
            bool argBoolean = true;
            double argDouble = double.MaxValue;
            string argString = "test string";

            Variant v1 = new Variant(Variant.EnumType.Dictionary);
            v1.Add("Int32", new Variant(argInt32));
            v1.Add("UInt32", new Variant(argUInt32));
            v1.Add("Int64", new Variant(argInt64));
            v1.Add("UInt64", new Variant(argUInt64));
            v1.Add("Boolean", new Variant(argBoolean));
            v1.Add("Double", new Variant(argDouble));
            v1.Add("String", new Variant(argString));
            v1.Add("None", new Variant(Variant.EnumType.None));

            byte[] bytesCompressed = BinaryWriter.ToBytes(v1, BinaryMode.Compress);
            byte[] bytesUncompressed = BinaryWriter.ToBytes(v1);

            Variant v2 = BinaryReader.FromBytes(bytesCompressed);

            Assert.AreEqual(v2.Type, Variant.EnumType.Dictionary);
            Assert.AreEqual(v2["String"].Type, Variant.EnumType.String);
            Assert.AreEqual(v2["Int32"].Type, Variant.EnumType.Int32);
            Assert.AreEqual(v2["UInt32"].Type, Variant.EnumType.UInt32);
            Assert.AreEqual(v2["Int64"].Type, Variant.EnumType.Int64);
            Assert.AreEqual(v2["UInt64"].Type, Variant.EnumType.UInt64);
            Assert.AreEqual(v2["Boolean"].Type, Variant.EnumType.Boolean);
            Assert.AreEqual(v2["Double"].Type, Variant.EnumType.Double);
            Assert.AreEqual(v2["None"].Type, Variant.EnumType.None);

            Assert.AreEqual(v2["String"].As<string>(), argString);
            Assert.AreEqual(v2["Int32"].As<Int32>(), argInt32);
            Assert.AreEqual(v2["UInt32"].As<UInt32>(), argUInt32);
            Assert.AreEqual(v2["Int64"].As<Int64>(), argInt64);
            Assert.AreEqual(v2["UInt64"].As<UInt64>(), argUInt64);
            Assert.AreEqual(v2["Boolean"].As<Boolean>(), argBoolean);
            Assert.AreEqual(v2["Double"].As<Double>(), argDouble);

            Variant v3 = BinaryReader.FromBytes(bytesUncompressed);

            Assert.IsTrue(v2.Equals(v3));
        }

        [Test]
        // Test compatibility with CLR variant
        public void TestCompatibility()
        {
            Int32 argInt32 = -1;
            UInt32 argUInt32 = 0xffffffff;
            Int64 argInt64 = -1;
            UInt64 argUInt64 = 0xffffffffffffffff;
            bool argBoolean = true;
            double argDouble = double.MaxValue;
            string argString = "test string";

            Variant v1 = new Variant(Variant.EnumType.Dictionary);
            v1.Add("Int32", new Variant(argInt32));
            v1.Add("UInt32", new Variant(argUInt32));
            v1.Add("Int64", new Variant(argInt64));
            v1.Add("UInt64", new Variant(argUInt64));
            v1.Add("Boolean", new Variant(argBoolean));
            v1.Add("Double", new Variant(argDouble));
            v1.Add("String", new Variant(argString));
            v1.Add("None", new Variant(Variant.EnumType.None));

            byte[] bytesCompressed = BinaryWriter.ToBytes(v1, BinaryMode.Compress);
            byte[] bytesUncompressed = BinaryWriter.ToBytes(v1);

            protean.clr.Variant v2 = protean.clr.BinaryReader.FromBytes(bytesCompressed);

            Assert.AreEqual(v2.Type, protean.clr.Variant.EnumType.Dictionary);
            Assert.AreEqual(v2["String"].Type, protean.clr.Variant.EnumType.String);
            Assert.AreEqual(v2["Int32"].Type, protean.clr.Variant.EnumType.Int32);
            Assert.AreEqual(v2["UInt32"].Type, protean.clr.Variant.EnumType.UInt32);
            Assert.AreEqual(v2["Int64"].Type, protean.clr.Variant.EnumType.Int64);
            Assert.AreEqual(v2["UInt64"].Type, protean.clr.Variant.EnumType.UInt64);
            Assert.AreEqual(v2["Boolean"].Type, protean.clr.Variant.EnumType.Boolean);
            Assert.AreEqual(v2["Double"].Type, protean.clr.Variant.EnumType.Double);
            Assert.AreEqual(v2["None"].Type, protean.clr.Variant.EnumType.None);

            Assert.AreEqual(v2["String"].AsString(), argString);
            Assert.AreEqual(v2["Int32"].AsInt32(), argInt32);
            Assert.AreEqual(v2["UInt32"].AsUInt32(), argUInt32);
            Assert.AreEqual(v2["Int64"].AsInt64(), argInt64);
            Assert.AreEqual(v2["UInt64"].AsUInt64(), argUInt64);
            Assert.AreEqual(v2["Boolean"].AsBoolean(), argBoolean);
            Assert.AreEqual(v2["Double"].AsDouble(), argDouble);

            protean.clr.Variant v3 = protean.clr.BinaryReader.FromBytes(bytesUncompressed);

            Assert.IsTrue(v2.Equals(v3));
        }

        [Test]
        public void TestPerformanceCS()
        {
            Int32 argInt32 = -1;
            UInt32 argUInt32 = 0xffffffff;
            Int64 argInt64 = -1;
            UInt64 argUInt64 = 0xffffffffffffffff;
            bool argBoolean = true;
            double argDouble = double.MaxValue;
            string argString = "test string";

            Variant v1 = new Variant(Variant.EnumType.Dictionary);
            v1.Add("Int32", new Variant(argInt32));
            v1.Add("UInt32", new Variant(argUInt32));
            v1.Add("Int64", new Variant(argInt64));
            v1.Add("UInt64", new Variant(argUInt64));
            v1.Add("Boolean", new Variant(argBoolean));
            v1.Add("Double", new Variant(argDouble));
            v1.Add("String", new Variant(argString));
            v1.Add("None", new Variant(Variant.EnumType.None));

            byte[] bytesCompressed = BinaryWriter.ToBytes(v1, BinaryMode.Compress);
            byte[] bytesDefault = BinaryWriter.ToBytes(v1);

            const int numIterations = 50000;

            DateTime t1 = DateTime.Now;

            // C# serialisation, compressed
            for (int i = 0; i < numIterations; ++i)
            {
                byte[] bytes = BinaryWriter.ToBytes(v1, BinaryMode.Compress);
                Assert.DoesNotThrow(delegate { BinaryReader.FromBytes(bytes); });
            }

            DateTime t2 = DateTime.Now;

            // C# serialisation, default
            for (int i = 0; i < numIterations; ++i)
            {
                byte[] bytes = BinaryWriter.ToBytes(v1);
                Assert.DoesNotThrow(delegate { BinaryReader.FromBytes(bytes); });
            }

            DateTime t3 = DateTime.Now;

            System.Console.WriteLine("C# serialisation timings: {0} (compressed), {1} (default)", t2 - t1, t3 - t2);
        }

        [Test]
        public void TestPerformanceCLR()
        {
            Int32 argInt32 = -1;
            UInt32 argUInt32 = 0xffffffff;
            Int64 argInt64 = -1;
            UInt64 argUInt64 = 0xffffffffffffffff;
            bool argBoolean = true;
            double argDouble = double.MaxValue;
            string argString = "test string";

            protean.clr.Variant v1 = new protean.clr.Variant(protean.clr.Variant.EnumType.Dictionary);
            v1.Add("Int32", new protean.clr.Variant(argInt32));
            v1.Add("UInt32", new protean.clr.Variant(argUInt32));
            v1.Add("Int64", new protean.clr.Variant(argInt64));
            v1.Add("UInt64", new protean.clr.Variant(argUInt64));
            v1.Add("Boolean", new protean.clr.Variant(argBoolean));
            v1.Add("Double", new protean.clr.Variant(argDouble));
            v1.Add("String", new protean.clr.Variant(argString));
            v1.Add("None", new protean.clr.Variant(protean.clr.Variant.EnumType.None));

            byte[] bytesCompressed = protean.clr.BinaryWriter.ToBytes(v1, protean.clr.BinaryMode.Compress);
            byte[] bytesDefault = protean.clr.BinaryWriter.ToBytes(v1);

            const int numIterations = 50000;

            DateTime t1 = DateTime.Now;

            // C# serialisation, compressed
            for (int i = 0; i < numIterations; ++i)
            {
                byte[] bytes = protean.clr.BinaryWriter.ToBytes(v1, protean.clr.BinaryMode.Compress);
                Assert.DoesNotThrow(delegate { BinaryReader.FromBytes(bytes); });
            }

            DateTime t2 = DateTime.Now;

            // C# serialisation, default
            for (int i = 0; i < numIterations; ++i)
            {
                byte[] bytes = protean.clr.BinaryWriter.ToBytes(v1);
                Assert.DoesNotThrow(delegate { BinaryReader.FromBytes(bytes); });
            }

            DateTime t3 = DateTime.Now;

            System.Console.WriteLine("CLR serialisation timings: {0} (compressed), {1} (default)", t2 - t1, t3 - t2);
        }

        class TestObject1 : VariantObjectBase
        {
            public TestObject1(string value)
            {
                m_value = value;
            }

            public TestObject1() :
                this("default")
            { }

            public override string Class { get { return "TestObject1"; } }

            public override int Version { get { return 1; } }

            public override Variant Deflate()
            {
                Variant result = new Variant(Variant.EnumType.Dictionary);
                result.Add("value", new Variant(m_value));
                return result;
            }

            public override void Inflate(Variant param, int version)
            {
                m_value = param["value"].As<string>();
            }

            private string m_value;
        }

        [Test]
        public void TestObject()
        {
            TestObject1 o1 = new TestObject1("some value");

            Variant v1 = new Variant(o1);

            byte[] bytes = BinaryWriter.ToBytes(v1);
            
            Variant v2 = BinaryReader.FromBytes(bytes);

            Assert.AreEqual(v2.Type, Variant.EnumType.Object);

            TestObject1 o2 = v2.AsObject<TestObject1>();

            Assert.AreEqual(o1.Class, o2.Class);
            Assert.AreEqual(o1.Version, o2.Version);
            Assert.IsTrue(o1.Deflate().Equals(o2.Deflate()));
        }
    }
}
