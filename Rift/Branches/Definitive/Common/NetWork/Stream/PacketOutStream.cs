using System;
using System.IO;
using System.IO.Compression;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork;

namespace Common
{
    public class PacketOutStream : PacketOut
    {
        public PacketOutStream()
            : base()
        {

        }

        public void WriteEncoded7Bit(long Value)
        {
            while (true)
            {
                byte ActuValue = (byte)(Value & 0x7F);
                Value = Value >> 7;

                if (Value != 0)
                    ActuValue += 0x80;

                WriteByte(ActuValue);

                if (Value == 0)
                    break;
            }
        }

        public static void Encode2Parameters(out long pValue, int pParameter1, int pParameter2)
        {
            if (pParameter1 > 0x07)
            {
                pParameter1 -= 8;
                pParameter1 <<= 3;
                pParameter1 |= 0x07;
            }
            pValue = pParameter2;
            if (pParameter1 <= 0x07)
            {
                pValue <<= 3;
                pValue |= (byte)(pParameter1 & 0x07);
                return;
            }
            pValue <<= 6;
            pValue |= (byte)(pParameter1 & 0x3F);
        }
        public static bool Decode2Parameters(long pValue, out int pParameter1, out int pParameter2)
        {
            pParameter1 = (int)(pValue & 0x07);
            pParameter2 = (int)(pValue >> 3);
            if (pParameter1 < 0x07) return true;
            pParameter1 = pParameter2 & 0x07;
            pParameter2 = (int)(pValue >> 6);
            if (pParameter1 < 0x07)
            {
                pParameter1 += 0x08;
                return true;
            }
            return false;
        }
        public static void Encode3Parameters(out long pValue, int pParameter1, int pParameter2, int pParameter3)
        {
            if (pParameter1 > 0x07)
            {
                pParameter1 -= 8;
                pParameter1 <<= 3;
                pParameter1 |= 0x07;
            }
            if (pParameter2 > 0x07)
            {
                pParameter2 -= 8;
                pParameter2 <<= 3;
                pParameter2 |= 0x07;
            }
            pValue = pParameter3;
            if (pParameter2 <= 0x07)
            {
                pValue <<= 3;
                pValue |= (byte)(pParameter2 & 0x07);
            }
            else
            {
                pValue <<= 6;
                pValue |= (byte)(pParameter2 & 0x3F);
            }
            if (pParameter1 <= 0x07)
            {
                pValue <<= 3;
                pValue |= (byte)(pParameter1 & 0x07);
            }
            else
            {
                pValue <<= 6;
                pValue |= (byte)(pParameter1 & 0x3F);
            }
        }
        public static bool Decode3Parameters(long pValue, out int pParameter1, out int pParameter2, out int pParameter3)
        {
            pParameter1 = 0;
            pParameter2 = 0;
            pParameter3 = 0;
            return Decode2Parameters(pValue, out pParameter1, out pParameter2) &&
                   Decode2Parameters(pParameter2, out pParameter2, out pParameter3);
        }
    }
}
