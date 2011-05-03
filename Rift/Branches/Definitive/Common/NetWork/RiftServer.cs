using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork;

namespace Common
{
    public class RiftServer  :TCPManager
    {
        public RiftServer()
            : base()
        {
            PacketOut.OpcodeInLen = true;
            PacketOut.OpcodeReverse = true;
            PacketOut.SizeInLen = false;
            PacketOut.Struct = PackStruct.SizeAndOpcode;
            PacketOut.SizeLen = 0;

            PacketProcessor.RegisterDefinitions();
        }

        protected override BaseClient GetNewClient()
        {
            return new RiftClient(this);
        }
    }
}
