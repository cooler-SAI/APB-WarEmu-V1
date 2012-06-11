using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.IO;
using System.Security.Cryptography;

namespace FrameWork.NetWork.Crypt.Crypto
{
    public class Srp6Utilities
    {
        // Methods
        public static BigInteger CalculateK(IDigest digest, BigInteger N, BigInteger g)
        {
            return HashPaddedPair(digest, N, N, g);
        }

        public static BigInteger CalculateU(IDigest digest, BigInteger N, BigInteger A, BigInteger B)
        {
            return HashPaddedPair(digest, N, A, B);
        }

        public static BigInteger CalculateX(IDigest digest, BigInteger N, byte[] salt, byte[] identity, byte[] password)
        {
            byte[] output = new byte[digest.GetDigestSize()];
            digest.BlockUpdate(identity, 0, identity.Length);
            digest.Update(0x3a);
            digest.BlockUpdate(password, 0, password.Length);
            digest.DoFinal(output, 0);
            digest.BlockUpdate(salt, 0, salt.Length);
            digest.BlockUpdate(output, 0, output.Length);
            digest.DoFinal(output, 0);
            return new BigInteger(1, output).Mod(N);
        }

        public static BigInteger GeneratePrivateValue(IDigest digest, BigInteger N, BigInteger g, SecureRandom random)
        {
            int num = Math.Min(0x100, N.BitLength / 2);
            BigInteger min = BigInteger.One.ShiftLeft(num - 1);
            BigInteger max = N.Subtract(BigInteger.One);
            return BigIntegers.CreateRandomInRange(min, max, random);
        }

        private static byte[] GetPadded(BigInteger n, int length)
        {
            byte[] sourceArray = BigIntegers.AsUnsignedByteArray(n);
            if (sourceArray.Length < length)
            {
                byte[] destinationArray = new byte[length];
                Array.Copy(sourceArray, 0, destinationArray, length - sourceArray.Length, sourceArray.Length);
                sourceArray = destinationArray;
            }
            return sourceArray;
        }

        private static BigInteger HashPaddedPair(IDigest digest, BigInteger N, BigInteger n1, BigInteger n2)
        {
            int length = (N.BitLength + 7) / 8;
            byte[] padded = GetPadded(n1, length);
            byte[] input = GetPadded(n2, length);
            digest.BlockUpdate(padded, 0, padded.Length);
            digest.BlockUpdate(input, 0, input.Length);
            byte[] output = new byte[digest.GetDigestSize()];
            digest.DoFinal(output, 0);
            return new BigInteger(1, output).Mod(N);
        }

        public static BigInteger ValidatePublicValue(BigInteger N, BigInteger val)
        {
            val = val.Mod(N);
            if (val.Equals(BigInteger.Zero))
            {
                throw new Exception("Invalid public value: 0");
            }
            return val;
        }
    }

 

}
