using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace WorldServer
{
    [AttributeUsage(AttributeTargets.Class)]
    public class IAbilityTypeAttribute : Attribute
    {
        public UInt16 AbilityType;

        public IAbilityTypeAttribute(UInt16 AbilityType)
        {
            this.AbilityType = AbilityType;
        }
    }

    public interface IAbilityTypeHandler
    {
        void Execute(Object Attacker, Object Victim);
    }
}
