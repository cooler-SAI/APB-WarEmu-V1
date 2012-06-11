using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace FrameWork.NetWork.Crypt.Crypto
{
    public class DigestRandomGenerator : IRandomGenerator
    {
        // Fields
        private const long CYCLE_COUNT = 10L;
        private IDigest digest;
        private byte[] seed;
        private long seedCounter;
        private byte[] state;
        private long stateCounter;

        // Methods
        public DigestRandomGenerator(IDigest digest)
        {
            this.digest = digest;
            this.seed = new byte[digest.GetDigestSize()];
            this.seedCounter = 1L;
            this.state = new byte[digest.GetDigestSize()];
            this.stateCounter = 1L;
        }

        public void AddSeedMaterial(byte[] inSeed)
        {
            lock (this)
            {
                this.DigestUpdate(inSeed);
                this.DigestUpdate(this.seed);
                this.DigestDoFinal(this.seed);
            }
        }

        public void AddSeedMaterial(long rSeed)
        {
            lock (this)
            {
                this.DigestAddCounter(rSeed);
                this.DigestUpdate(this.seed);
                this.DigestDoFinal(this.seed);
            }
        }

        private void CycleSeed()
        {
            long num;
            this.DigestUpdate(this.seed);
            this.seedCounter = (num = this.seedCounter) + 1L;
            this.DigestAddCounter(num);
            this.DigestDoFinal(this.seed);
        }

        private void DigestAddCounter(long seedVal)
        {
            ulong num = (ulong)seedVal;
            for (int i = 0; i != 8; i++)
            {
                this.digest.Update((byte)num);
                num = num >> 8;
            }
        }

        private void DigestDoFinal(byte[] result)
        {
            this.digest.DoFinal(result, 0);
        }

        private void DigestUpdate(byte[] inSeed)
        {
            this.digest.BlockUpdate(inSeed, 0, inSeed.Length);
        }

        private void GenerateState()
        {
            long num;
            this.stateCounter = (num = this.stateCounter) + 1L;
            this.DigestAddCounter(num);
            this.DigestUpdate(this.state);
            this.DigestUpdate(this.seed);
            this.DigestDoFinal(this.state);
            if ((this.stateCounter % 10L) == 0L)
            {
                this.CycleSeed();
            }
        }

        public void NextBytes(byte[] bytes)
        {
            this.NextBytes(bytes, 0, bytes.Length);
        }

        public void NextBytes(byte[] bytes, int start, int len)
        {
            lock (this)
            {
                int num = 0;
                this.GenerateState();
                int num2 = start + len;
                for (int i = start; i < num2; i++)
                {
                    if (num == this.state.Length)
                    {
                        this.GenerateState();
                        num = 0;
                    }
                    bytes[i] = this.state[num++];
                }
            }
        }
    }


}
