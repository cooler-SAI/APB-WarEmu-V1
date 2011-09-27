using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork;
using Common;

namespace MapServer
{
    [ISerializableAttribute((long)Opcodes.WorldServerPositionUpdate)]
    public class WorldServerPositionUpdate : ISerializablePacket
    {
        [Raw8Bit(0)]
        public long GUID;

        [ListBit(1)]
        public List<float> Position;

        [Unsigned7Bit(2)]
        public long UnkMovementInfo1 = 10569;

        [Unsigned7Bit(3)]
        public long UnkMovementInfo2 = 32767;

        [ListBit(4)]
        public List<float> Orientation;

        [BoolBit(12)]
        public bool UnkMovementInfo3;

        public override void OnRead(RiftClient From)
        {
            if (From.Acct == null || From.Character == null)
                return;

            From.Character.Info.Position = Position;
            From.Character.Info.Dirty = true;
        }
    }
}