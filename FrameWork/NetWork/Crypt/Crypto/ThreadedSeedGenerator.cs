using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

namespace FrameWork.NetWork.Crypt.Crypto
{
    public class ThreadedSeedGenerator
    {
        // Methods
        public byte[] GenerateSeed(int numBytes, bool fast)
        {
            return new SeedGenerator().GenerateSeed(numBytes, fast);
        }

        // Nested Types
        private class SeedGenerator
        {
            // Fields
            private volatile int counter;
            private volatile bool stop;

            // Methods
            public byte[] GenerateSeed(int numBytes, bool fast)
            {
                this.counter = 0;
                this.stop = false;
                byte[] buffer = new byte[numBytes];
                int counter = 0;
                int num2 = fast ? numBytes : (numBytes * 8);
                ThreadPool.QueueUserWorkItem(new WaitCallback(this.Run));
                for (int i = 0; i < num2; i++)
                {
                    while (this.counter == counter)
                    {
                        try
                        {
                            Thread.Sleep(1);
                        }
                        catch (Exception)
                        {
                        }
                    }
                    counter = this.counter;
                    if (fast)
                    {
                        buffer[i] = (byte)counter;
                    }
                    else
                    {
                        int index = i / 8;
                        buffer[index] = (byte)((buffer[index] << 1) | (counter & 1));
                    }
                }
                this.stop = true;
                return buffer;
            }

            private void Run(object ignored)
            {
                while (!this.stop)
                {
                    this.counter++;
                }
            }
        }
    }


}
