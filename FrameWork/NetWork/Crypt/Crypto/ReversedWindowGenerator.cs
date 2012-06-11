using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace FrameWork.NetWork.Crypt.Crypto
{
    public class ReversedWindowGenerator : IRandomGenerator
    {
        // Fields
        private readonly IRandomGenerator generator;
        private byte[] window;
        private int windowCount;

        // Methods
        public ReversedWindowGenerator(IRandomGenerator generator, int windowSize)
        {
            if (generator == null)
            {
                throw new ArgumentNullException("generator");
            }
            if (windowSize < 2)
            {
                throw new ArgumentException("Window size must be at least 2", "windowSize");
            }
            this.generator = generator;
            this.window = new byte[windowSize];
        }

        public virtual void AddSeedMaterial(byte[] seed)
        {
            lock (this)
            {
                this.windowCount = 0;
                this.generator.AddSeedMaterial(seed);
            }
        }

        public virtual void AddSeedMaterial(long seed)
        {
            lock (this)
            {
                this.windowCount = 0;
                this.generator.AddSeedMaterial(seed);
            }
        }

        private void doNextBytes(byte[] bytes, int start, int len)
        {
            lock (this)
            {
                int num = 0;
                while (num < len)
                {
                    if (this.windowCount < 1)
                    {
                        this.generator.NextBytes(this.window, 0, this.window.Length);
                        this.windowCount = this.window.Length;
                    }
                    bytes[start + num++] = this.window[--this.windowCount];
                }
            }
        }

        public virtual void NextBytes(byte[] bytes)
        {
            this.doNextBytes(bytes, 0, bytes.Length);
        }

        public virtual void NextBytes(byte[] bytes, int start, int len)
        {
            this.doNextBytes(bytes, start, len);
        }
    }


}
