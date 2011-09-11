using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork;
using Common;

namespace WorldServer
{
    [AttributeUsage(AttributeTargets.Class)]
    public class IAbilityTypeAttribute : Attribute
    {
        public UInt16 AbilityType;
        public string TypeDescription;

        public IAbilityTypeAttribute(UInt16 AbilityType, string TypeDescription)
        {
            this.AbilityType = AbilityType;
            this.TypeDescription = TypeDescription;
        }
    }

    public abstract class IAbilityTypeHandler
    {
        public abstract void Start(Object Attacker, Object Victim,Ability_Info Info);

        public abstract void Update();

        public abstract void Stop();
    }
}
