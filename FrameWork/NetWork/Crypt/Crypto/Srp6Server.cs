using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace FrameWork.NetWork.Crypt.Crypto
{
    public class Srp6Server
    {
        // Fields
        protected BigInteger A;
        protected IDigest digest;
        protected BigInteger g;
        protected BigInteger N;
        protected BigInteger privB;
        protected BigInteger pubB;
        protected SecureRandom random;
        protected BigInteger S;
        protected BigInteger u;
        protected BigInteger v;

        // Methods
        private BigInteger CalculateS()
        {
            return this.v.ModPow(this.u, this.N).Multiply(this.A).Mod(this.N).ModPow(this.privB, this.N);
        }

        public virtual BigInteger CalculateSecret(BigInteger clientA)
        {
            this.A = Srp6Utilities.ValidatePublicValue(this.N, clientA);
            this.u = Srp6Utilities.CalculateU(this.digest, this.N, this.A, this.pubB);
            this.S = this.CalculateS();
            return this.S;
        }

        public virtual BigInteger GenerateServerCredentials()
        {
            BigInteger integer = Srp6Utilities.CalculateK(this.digest, this.N, this.g);
            this.privB = this.SelectPrivateValue();
            this.pubB = integer.Multiply(this.v).Mod(this.N).Add(this.g.ModPow(this.privB, this.N)).Mod(this.N);
            return this.pubB;
        }

        public virtual void Init(BigInteger N, BigInteger g, BigInteger v, IDigest digest, SecureRandom random)
        {
            this.N = N;
            this.g = g;
            this.v = v;
            this.random = random;
            this.digest = digest;
        }

        protected virtual BigInteger SelectPrivateValue()
        {
            return Srp6Utilities.GeneratePrivateValue(this.digest, this.N, this.g, this.random);
        }
    }


}
