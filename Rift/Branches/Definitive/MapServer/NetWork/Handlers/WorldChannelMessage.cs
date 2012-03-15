using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork;
using Common;

namespace MapServer
{
    [ISerializableAttribute((long)Opcodes.WorldChatMessage)]
    public class WorldChatMessage : ISerializablePacket
    {
        [ArrayBit(0)]
        public string ChannelName;

        [ArrayBit(2)]
        public string PlayerName;

        [ArrayBit(3)]
        public string Message;

        [Unsigned7Bit(7)]
        public long Unk1;

        [Unsigned7Bit(9)]
        public long Unk2;

        public static long CurrentMapId = 676;

        public override void OnRead(RiftClient From)
        {
            if (From.Character == null)
                return;

            PlayerName = From.Character.CharacterName;

            foreach (RiftClient Client in From.Server.Clients)
            {
                if (Client != null)
                    Client.SendSerialized(this);
            }
        }
    }
}
