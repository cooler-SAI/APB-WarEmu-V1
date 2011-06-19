using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Reflection;

namespace Common
{
    public class BoolBitAttribute : ISerializableFieldAttribute
    {
        public BoolBitAttribute(int Index)
            : base(Index)
        {

        }

        public override Type GetSerializableType()
        {
            return typeof(BoolBitField);
        }
    }

    [Serializable]
    public class BoolBitField : ISerializableField
    {
        public override void Deserialize(ref PacketInStream Data)
        {
            if (PacketType == EPacketFieldType.True)
                val = (bool)true;
            else
                val = (bool)false;
        }

        public override bool Serialize(ref PacketOutStream Data)
        {

            return true;
        }

        public override void ApplyToFieldInfo(FieldInfo Info, ISerializablePacket Packet, Type Field)
        {
            if (Field.Equals(typeof(bool)))
                Info.SetValue(Packet, val);
            else
                Info.SetValue(Packet, ((bool)val) == true ? 1 : 0);
                
        }
    }
}