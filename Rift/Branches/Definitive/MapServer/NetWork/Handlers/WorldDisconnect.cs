using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;
using FrameWork;

namespace MapServer
{
    [ISerializableAttribute((long)Opcodes.WorldDisconnect)]
    public class WorldDisconnect : ISerializablePacket
    {
        public override void OnRead(RiftClient From)
        {
            if (From.Character != null)
            {
                Log.Success("WorldDisconnect", "Character disconnectig : " + From.Character.CharacterName);
                From.Disconnect();
            }
        }
    }
}
