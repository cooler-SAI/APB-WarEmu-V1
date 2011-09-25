using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Reflection;

using FrameWork;

namespace Common
{
    public class Raw8BitAttribute : ISerializableFieldAttribute
    {
        public Raw8BitAttribute(int Index)
            : base(Index)
        {

        }

        public override Type GetSerializableType()
        {
            return typeof(Raw8BitBitField);
        }
    }

    [Serializable]
    public class Raw8BitBitField : ISerializableField
    {
        public override void Deserialize(ref PacketInStream Data)
        {
            val = Data.Read(8);
        }

        public override bool Serialize(ref PacketOutStream Data, bool Force)
        {
            if (!Force && (val == null || val.ToString() == "0"))
                return false;

            if (val is byte[])
                Data.Write((byte[])val);
            else if (val is long)
                Data.WriteInt64R((long)val);
            else if (val is UInt64)
                Data.WriteUInt64R((UInt64)val);
            else
                return false;

            return true;
        }

        public override void ApplyToFieldInfo(FieldInfo Info, ISerializablePacket Packet, Type Field)
        {
            if (Field.Equals(typeof(byte[])))
                Info.SetValue(Packet, (byte[])val);
            else if (Field.Equals(typeof(long)))
            {
                //Array.Reverse((val as byte[]));
                Info.SetValue(Packet, BitConverter.ToInt64((byte[])val, 0));
            }
        }
    }
}
