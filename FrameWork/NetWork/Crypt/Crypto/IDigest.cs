using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace FrameWork.NetWork.Crypt.Crypto
{
    public interface IDigest
    {
        // Methods
        void BlockUpdate(byte[] input, int inOff, int length);
        int DoFinal(byte[] output, int outOff);
        int GetByteLength();
        int GetDigestSize();
        void Reset();
        void Update(byte input);

        // Properties
        string AlgorithmName { get; }
    }
}
