//  (C) Copyright Johan Ditmar, Karel Hruda, Paul O'Neill & Luke Stedman 2009.
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).

using System;
using System.Collections.Generic;
using System.Text;

namespace protean {

    public class BinaryReader
    {
        public BinaryReader(System.IO.Stream stream)
        {
            m_stream = stream;
        }
    
        public Variant Read()
        {
            ReadHeader();
            return ReadVariant();
        }

        public static Variant FromBytes(byte[] bytes)
        {
            using (System.IO.MemoryStream ms = new System.IO.MemoryStream(bytes))
            {
                BinaryReader reader = new BinaryReader(ms);
                return reader.Read();
            }
        }

        private void ReadHeader()
        {
            byte[] bytes = new byte[4];
            m_stream.Read(bytes, 0, 4);

            UInt32 magicNumber = System.BitConverter.ToUInt32(bytes, 0);
            if (magicNumber != BinaryConstants.PROTEAN_MAGIC)
            {
                throw new VariantException("Invalid magic number");
            }

            // ignore version info for now
            m_stream.Read(bytes, 0, 4);

            m_stream.Read(bytes, 0, 4);
            m_mode = (BinaryMode)System.BitConverter.ToUInt32(bytes, 0);

            if ((m_mode & BinaryMode.Compress) != 0)
            {
                if ((m_mode & BinaryMode.ZlibHeader) != 0)
                {
                    throw new VariantException("Binary data appears to contain ZLIB header which is currently not supported in the protean.NET BinaryReader");
                }

                m_filter = new System.IO.Compression.DeflateStream(m_stream, System.IO.Compression.CompressionMode.Decompress, true);
            }
            else
            {
                m_filter = m_stream;
            }
        }

        private Variant ReadVariant()
        {
            Variant.EnumType type = (Variant.EnumType)ReadUInt32();

            switch (type)
            {
                case Variant.EnumType.None:
                    return new Variant(Variant.EnumType.None);
                case Variant.EnumType.String:
                    return new Variant(ReadString());
                case Variant.EnumType.Double:
                    return new Variant(ReadDouble());
                case Variant.EnumType.Float:
                    return new Variant(ReadFloat());
                case Variant.EnumType.Int32:
                    return new Variant(ReadInt32());
                case Variant.EnumType.UInt32:
                    return new Variant(ReadUInt32());
                case Variant.EnumType.Int64:
                    return new Variant(ReadInt64());
                case Variant.EnumType.UInt64:
                    return new Variant(ReadUInt64());
                case Variant.EnumType.Boolean:
                    return new Variant(ReadBoolean());
                case Variant.EnumType.Time:
                    if ((m_mode & BinaryMode.DateTimeAsTicks) == 0)
                    {
                        throw new VariantException("Binary data has DateTimeAsTicks mode disabled which is not supported in the protean.NET BinaryReader");
                    }
                    return new Variant(ReadTime());
                case Variant.EnumType.DateTime:
                    if ((m_mode & BinaryMode.DateTimeAsTicks) == 0)
                    {
                        throw new VariantException("Binary data has DateTimeAsTicks mode disabled which is not supported in the protean.NET BinaryReader");
                    }
                    return new Variant(ReadDateTime());
                case Variant.EnumType.Tuple:
                {
                    int length = ReadInt32();

                    Variant result = new Variant(Variant.EnumType.Tuple, length);
                    for (int i = 0; i < length; ++i)
                    {
                        result[i] = ReadVariant();
                    }
                    return result;
                }
                case Variant.EnumType.List:
                {
                    int length = ReadInt32();

                    Variant result = new Variant(type, length);
                    for (int i=0; i<length; ++i)
                    {
                        result.Add(ReadVariant());
                    }
                    return result;
                }
                case Variant.EnumType.Dictionary:
                case Variant.EnumType.Bag:
                {
                    int length = ReadInt32();

                    Variant result = new Variant(type);
                    for (int i=0; i<length; ++i)
                    {
                        String key = ReadString();
                        Variant value = ReadVariant();

                        result.Add(key, value);
                    }
                    return result;
                }
                case Variant.EnumType.TimeSeries:
                {
                    Variant result = new Variant(type);

                    int length = ReadInt32();

                    for (int i=0; i<length; ++i)
                    {
                        DateTime time = ReadDateTime();
                        Variant value = ReadVariant();

                        result.Add(time, value);
                    }

                    return result;
                }
                case Variant.EnumType.Object:
                {
                    string className = ReadString();
                    int version = ReadInt32();
                    Variant param = ReadVariant();

                    VariantObjectProxy proxy = new VariantObjectProxy(className);
                    proxy.Inflate(param, version);

                    return new Variant(proxy);
                }
                case Variant.EnumType.Exception:
                {
                    string xtype = ReadString();
                    string xmessage = ReadString();
                    string xsource = ReadString();
                    string xstack = ReadString();

                    return new Variant(new VariantExceptionInfo(xtype, xmessage, xsource, xstack));
                }
                case Variant.EnumType.Buffer:
                {
                    int length = ReadInt32();
                    return new Variant(ReadBytes(length, true));
                }
                default:
                    throw new VariantException("Case exhaustion: " + type.ToString());
            }
        }

        private byte[] ReadBytes(int length, bool readPadding)
        {
            byte[] bytes = ReadBytes(length);

            if (readPadding)
            {
                int residual = (4 - (length % 4)) % 4;
                for (int i = 0; i < residual; ++i)
                {
                    m_filter.ReadByte();
                }
            }

            return bytes;
        }
        private byte[] ReadBytes(int length)
        {
            byte[] bytes = new byte[length];
            m_filter.Read(bytes, 0, length);
            return bytes;
        }
        private bool ReadBoolean()
        {
            return ReadInt32()!=0;
        }

        private string ReadString()
        {
            Int32 length = ReadInt32();
            byte[] bytes = ReadBytes(length, true);

            return System.Text.Encoding.ASCII.GetString(bytes, 0, length);
        }
        private float ReadFloat()
        {
            return System.BitConverter.ToSingle(ReadBytes(sizeof(float)), 0);
        }
        private double ReadDouble()
        {
            return System.BitConverter.Int64BitsToDouble(ReadInt64());
        }
        private Int32 ReadInt32()
        {
            return System.BitConverter.ToInt32(ReadBytes(sizeof(Int32)), 0);
        }
        private UInt32 ReadUInt32()
        {
            return System.BitConverter.ToUInt32(ReadBytes(sizeof(UInt32)), 0);
        }
        private Int64 ReadInt64()
        {
            return System.BitConverter.ToInt64(ReadBytes(sizeof(Int64)), 0);
        }
        private UInt64 ReadUInt64()
        {
            return System.BitConverter.ToUInt64(ReadBytes(sizeof(UInt64)), 0);
        }
        private TimeSpan ReadTime()
        {
            Int64 total_millis = ReadInt64();

            return new TimeSpan(total_millis * 10000);
        }
        private DateTime ReadDateTime()
        {
            Int64 total_millis = ReadInt64();

            return Variant.MinDateTime + new TimeSpan(total_millis * 10000);
        }

        private System.IO.Stream m_stream;
        private System.IO.Stream m_filter;
        private BinaryMode m_mode;
    }

} // protean
