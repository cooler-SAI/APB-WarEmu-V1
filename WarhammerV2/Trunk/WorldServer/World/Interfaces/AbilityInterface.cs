using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;
using FrameWork;

namespace WorldServer
{
    public class AbilityInterface : BaseInterface
    {
        public List<Ability_Info> Abilities = new List<Ability_Info>();

        public AbilityInterface(Object Owner)
            : base(Owner)
        {

        }

        public override bool Load()
        {
            UpdateAbilities();
            return base.Load();
        }

        public void UpdateAbilities()
        {
            if (HasPlayer())
                Abilities = AbilityMgr.GetCareerAbility(GetPlayer()._Info.CareerLine, GetPlayer().Level);
        }

        public void SendAbilities()
        {
            if (!HasPlayer())
                return;

            Log.Info("AbilityInterface", "Sending " + Abilities.Count + " Abilities");

            PacketOut Out = new PacketOut((byte)Opcodes.F_CHARACTER_INFO);
            Out.WriteByte(1); // Action
            Out.WriteByte((byte)Abilities.Count);
            Out.WriteUInt16(0x300);

            foreach (Ability_Info Info in Abilities)
            {
                Out.WriteUInt16(Info.Entry);
                Out.WriteByte(Info.Level);
            }

            GetPlayer().SendPacket(Out);

            PacketOut AutoAttack = new PacketOut((byte)Opcodes.F_CHARACTER_INFO);
            AutoAttack.WriteByte(1); // Action
            AutoAttack.WriteByte(1); // Count
            AutoAttack.WriteUInt16(0x300);
            AutoAttack.WriteUInt16(245);
            AutoAttack.WriteByte(1);
            GetPlayer().SendPacket(AutoAttack);

        }


        public bool IsCasting()
        {
            return false;
        }
    }
}
