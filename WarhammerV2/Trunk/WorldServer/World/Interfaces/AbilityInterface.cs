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


        }
    }
}
