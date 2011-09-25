using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Reflection;

namespace Common
{
    public class Unsigned7BitAttribute : ISerializableFieldAttribute
    {
        public Unsigned7BitAttribute(int Index)
            : base(Index)
        {

        }

        public override Type GetSerializableType()
        {
            return typeof(Unsigned7BitField);
        }
    }

    [Serializable]
    public class Unsigned7BitField : ISerializableField
    {
        public override void Deserialize(ref PacketInStream Data)
        {
            val = Data.ReadEncoded7Bit();
        }

        public override bool Serialize(ref PacketOutStream Data, bool Force)
        {
            if (!Force && (val == null || val.ToString() == "0"))
                return false;

            Data.WriteEncoded7Bit((long)(Convert.ChangeType(val,typeof(long))));
            return true;
        }

        public override void ApplyToFieldInfo(FieldInfo Info, ISerializablePacket Packet, Type Field)
        {
            Info.SetValue(Packet, Convert.ChangeType(val,Info.FieldType));
        }
    }
}
