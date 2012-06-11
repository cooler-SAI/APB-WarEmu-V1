using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace FrameWork.NetWork.Crypt.Crypto
{
    public class Srp6Client
    {
        // Fields
        protected BigInteger B;
        protected IDigest digest;
        protected BigInteger g;
        protected BigInteger N;
        protected BigInteger privA;
        protected BigInteger pubA;
        protected SecureRandom random;
        protected BigInteger S;
        protected BigInteger u;
        protected BigInteger x;

        // Methods
        private BigInteger CalculateS()
        {
            BigInteger val = Srp6Utilities.CalculateK(this.digest, this.N, this.g);
            BigInteger exponent = this.u.Multiply(this.x).Add(this.privA);
            BigInteger n = this.g.ModPow(this.x, this.N).Multiply(val).Mod(this.N);
            return this.B.Subtract(n).Mod(this.N).ModPow(exponent, this.N);
        }

        public virtual BigInteger CalculateSecret(BigInteger serverB)
        {
            this.B = Srp6Utilities.ValidatePublicValue(this.N, serverB);
            this.u = Srp6Utilities.CalculateU(this.digest, this.N, this.pubA, this.B);
            this.S = this.CalculateS();
            return this.S;
        }

        public virtual BigInteger GenerateClientCredentials(byte[] salt, byte[] identity, byte[] password)
        {
            this.x = Srp6Utilities.CalculateX(this.digest, this.N, salt, identity, password);
            this.privA = this.SelectPrivateValue();
            this.pubA = this.g.ModPow(this.privA, this.N);
            return this.pubA;
        }

        public virtual void Init(BigInteger N, BigInteger g, IDigest digest, SecureRandom random)
        {
            this.N = N;
            this.g = g;
            this.digest = digest;
            this.random = random;
        }

        protected virtual BigInteger SelectPrivateValue()
        {
            return Srp6Utilities.GeneratePrivateValue(this.digest, this.N, this.g, this.random);
        }
    }


}
