using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;

namespace FrameWork.NetWork.Crypt.Crypto
{
    public sealed class Hex
    {
        // Fields
        private static readonly HexEncoder encoder = new HexEncoder();

        // Methods
        private Hex()
        {
        }

        public static byte[] Decode(string data)
        {
            MemoryStream outStream = new MemoryStream((data.Length + 1) / 2);
            encoder.DecodeString(data, outStream);
            return outStream.ToArray();
        }

        public static byte[] Decode(byte[] data)
        {
            MemoryStream outStream = new MemoryStream((data.Length + 1) / 2);
            encoder.Decode(data, 0, data.Length, outStream);
            return outStream.ToArray();
        }

        public static int Decode(string data, Stream outStream)
        {
            return encoder.DecodeString(data, outStream);
        }

        public static byte[] Encode(byte[] data)
        {
            return Encode(data, 0, data.Length);
        }

        public static int Encode(byte[] data, Stream outStream)
        {
            return encoder.Encode(data, 0, data.Length, outStream);
        }

        public static byte[] Encode(byte[] data, int off, int length)
        {
            MemoryStream outStream = new MemoryStream(length * 2);
            encoder.Encode(data, off, length, outStream);
            return outStream.ToArray();
        }

        public static int Encode(byte[] data, int off, int length, Stream outStream)
        {
            return encoder.Encode(data, off, length, outStream);
        }
    }


}
