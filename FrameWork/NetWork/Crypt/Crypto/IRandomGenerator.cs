using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace FrameWork.NetWork.Crypt.Crypto
{
    public interface IRandomGenerator
    {
        // Methods
        void AddSeedMaterial(byte[] seed);
        void AddSeedMaterial(long seed);
        void NextBytes(byte[] bytes);
        void NextBytes(byte[] bytes, int start, int len);
    }
}
