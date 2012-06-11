using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace FrameWork.NetWork.Crypt.Crypto
{
    public sealed class BigIntegers
    {
        // Fields
        private const int MaxIterations = 0x3e8;

        // Methods
        private BigIntegers()
        {
        }

        public static byte[] AsUnsignedByteArray(BigInteger n)
        {
            return n.ToByteArrayUnsigned();
        }

        public static BigInteger CreateRandomInRange(BigInteger min, BigInteger max, SecureRandom random)
        {
            int num = min.CompareTo(max);
            if (num >= 0)
            {
                if (num > 0)
                {
                    throw new ArgumentException("'min' may not be greater than 'max'");
                }
                return min;
            }
            if (min.BitLength > (max.BitLength / 2))
            {
                return CreateRandomInRange(BigInteger.Zero, max.Subtract(min), random).Add(min);
            }
            for (int i = 0; i < 0x3e8; i++)
            {
                BigInteger integer = new BigInteger(max.BitLength, random);
                if ((integer.CompareTo(min) >= 0) && (integer.CompareTo(max) <= 0))
                {
                    return integer;
                }
            }
            return new BigInteger(max.Subtract(min).BitLength - 1, random).Add(min);
        }
    }


}
