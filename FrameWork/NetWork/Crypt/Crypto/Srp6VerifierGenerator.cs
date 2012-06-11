using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace FrameWork.NetWork.Crypt.Crypto
{
    public class Srp6VerifierGenerator
    {
        // Fields
        protected IDigest digest;
        protected BigInteger g;
        protected BigInteger N;

        // Methods
        public virtual BigInteger GenerateVerifier(byte[] salt, byte[] identity, byte[] password)
        {
            BigInteger exponent = Srp6Utilities.CalculateX(this.digest, this.N, salt, identity, password);
            return this.g.ModPow(exponent, this.N);
        }

        public virtual void Init(BigInteger N, BigInteger g, IDigest digest)
        {
            this.N = N;
            this.g = g;
            this.digest = digest;
        }
    }


}
