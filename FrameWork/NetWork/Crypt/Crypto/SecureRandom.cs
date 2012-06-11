using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Globalization;

namespace FrameWork.NetWork.Crypt.Crypto
{
    public class SecureRandom : Random
    {
        // Fields
        private static readonly double DoubleScale = Math.Pow(2.0, 64.0);
        protected IRandomGenerator generator;
        private static readonly SecureRandom[] master = new SecureRandom[1];
        private static readonly IRandomGenerator sha1Generator = new DigestRandomGenerator(new Sha1Digest());
        private static readonly IRandomGenerator sha256Generator = new DigestRandomGenerator(new Sha256Digest());

        // Methods
        public SecureRandom()
            : this(sha1Generator)
        {
            this.SetSeed(GetSeed(8));
        }

        public SecureRandom(IRandomGenerator generator)
            : base(0)
        {
            this.generator = generator;
        }

        public SecureRandom(byte[] inSeed)
            : this(sha1Generator)
        {
            this.SetSeed(inSeed);
        }

        public virtual byte[] GenerateSeed(int length)
        {
            this.SetSeed(DateTime.Now.Ticks);
            byte[] buffer = new byte[length];
            this.NextBytes(buffer);
            return buffer;
        }

        public static SecureRandom GetInstance(string algorithm)
        {
            IRandomGenerator generator = null;
            string str = algorithm.ToUpper(CultureInfo.InvariantCulture);
            if (str != null)
            {
                if (!(str == "SHA1PRNG"))
                {
                    if (str == "SHA256PRNG")
                    {
                        generator = sha256Generator;
                    }
                }
                else
                {
                    generator = sha1Generator;
                }
            }
            if (generator == null)
            {
                throw new ArgumentException("Unrecognised PRNG algorithm: " + algorithm, "algorithm");
            }
            return new SecureRandom(generator);
        }

        public static byte[] GetSeed(int length)
        {
            return Master.GenerateSeed(length);
        }

        public override int Next()
        {
            int num;
            do
            {
                num = this.NextInt() & 0x7fffffff;
            }
            while (num == 0x7fffffff);
            return num;
        }

        public override int Next(int maxValue)
        {
            int num3;
            int num4;
            if (maxValue < 2)
            {
                if (maxValue < 0)
                {
                    throw new ArgumentOutOfRangeException("maxValue < 0");
                }
                return 0;
            }
            if ((maxValue & -maxValue) == maxValue)
            {
                int num = this.NextInt() & 0x7fffffff;
                long num2 = (maxValue * num) >> 0x1f;
                return (int)num2;
            }
            do
            {
                num3 = this.NextInt() & 0x7fffffff;
                num4 = num3 % maxValue;
            }
            while (((num3 - num4) + (maxValue - 1)) < 0);
            return num4;
        }

        public override int Next(int minValue, int maxValue)
        {
            int num2;
            if (maxValue <= minValue)
            {
                if (maxValue != minValue)
                {
                    throw new ArgumentException("maxValue cannot be less than minValue");
                }
                return minValue;
            }
            int num = maxValue - minValue;
            if (num > 0)
            {
                return (minValue + this.Next(num));
            }
            do
            {
                num2 = this.NextInt();
            }
            while ((num2 < minValue) || (num2 >= maxValue));
            return num2;
        }

        public override void NextBytes(byte[] buffer)
        {
            this.generator.NextBytes(buffer);
        }

        public virtual void NextBytes(byte[] buffer, int start, int length)
        {
            this.generator.NextBytes(buffer, start, length);
        }

        public override double NextDouble()
        {
            return (Convert.ToDouble((ulong)this.NextLong()) / DoubleScale);
        }

        public virtual int NextInt()
        {
            byte[] buffer = new byte[4];
            this.NextBytes(buffer);
            int num = 0;
            for (int i = 0; i < 4; i++)
            {
                num = (num << 8) + (buffer[i] & 0xff);
            }
            return num;
        }

        public virtual long NextLong()
        {
            return (long)((((ulong)this.NextInt()) << 0x20) | ((ulong)this.NextInt()));
        }

        public virtual void SetSeed(byte[] inSeed)
        {
            this.generator.AddSeedMaterial(inSeed);
        }

        public virtual void SetSeed(long seed)
        {
            this.generator.AddSeedMaterial(seed);
        }

        // Properties
        private static SecureRandom Master
        {
            get
            {
                if (master[0] == null)
                {
                    IRandomGenerator generator = new ReversedWindowGenerator(sha256Generator, 0x20);
                    SecureRandom random = master[0] = new SecureRandom(generator);
                    random.SetSeed(DateTime.Now.Ticks);
                    random.SetSeed(new ThreadedSeedGenerator().GenerateSeed(0x18, true));
                    random.GenerateSeed(1 + random.Next(0x20));
                }
                return master[0];
            }
        }
    }
}
