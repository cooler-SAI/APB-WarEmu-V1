using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace FrameWork.NetWork.Crypt.Crypto
{
    public abstract class GeneralDigest : IDigest
    {
        // Fields
        private const int BYTE_LENGTH = 0x40;
        private long byteCount;
        private byte[] xBuf;
        private int xBufOff;

        // Methods
        internal GeneralDigest()
        {
            this.xBuf = new byte[4];
        }

        internal GeneralDigest(GeneralDigest t)
        {
            this.xBuf = new byte[t.xBuf.Length];
            Array.Copy(t.xBuf, 0, this.xBuf, 0, t.xBuf.Length);
            this.xBufOff = t.xBufOff;
            this.byteCount = t.byteCount;
        }

        public void BlockUpdate(byte[] input, int inOff, int length)
        {
            while ((this.xBufOff != 0) && (length > 0))
            {
                this.Update(input[inOff]);
                inOff++;
                length--;
            }
            while (length > this.xBuf.Length)
            {
                this.ProcessWord(input, inOff);
                inOff += this.xBuf.Length;
                length -= this.xBuf.Length;
                this.byteCount += this.xBuf.Length;
            }
            while (length > 0)
            {
                this.Update(input[inOff]);
                inOff++;
                length--;
            }
        }

        public abstract int DoFinal(byte[] output, int outOff);
        public void Finish()
        {
            long bitLength = this.byteCount << 3;
            this.Update(0x80);
            while (this.xBufOff != 0)
            {
                this.Update(0);
            }
            this.ProcessLength(bitLength);
            this.ProcessBlock();
        }

        public int GetByteLength()
        {
            return 0x40;
        }

        public abstract int GetDigestSize();
        internal abstract void ProcessBlock();
        internal abstract void ProcessLength(long bitLength);
        internal abstract void ProcessWord(byte[] input, int inOff);
        public virtual void Reset()
        {
            this.byteCount = 0L;
            this.xBufOff = 0;
            Array.Clear(this.xBuf, 0, this.xBuf.Length);
        }

        public void Update(byte input)
        {
            this.xBuf[this.xBufOff++] = input;
            if (this.xBufOff == this.xBuf.Length)
            {
                this.ProcessWord(this.xBuf, 0);
                this.xBufOff = 0;
            }
            this.byteCount += 1L;
        }

        // Properties
        public abstract string AlgorithmName { get; }
    }

 

}
