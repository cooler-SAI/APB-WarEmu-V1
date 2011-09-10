using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Reflection;

using FrameWork;
using Common;

namespace WorldServer
{
    static public class AbilityMgr
    {
        static public Dictionary<UInt16, IAbilityTypeHandler> _AbilityTypes = new Dictionary<ushort, IAbilityTypeHandler>();

        static public void LoadAbilityType()
        {
            foreach (Assembly assembly in AppDomain.CurrentDomain.GetAssemblies())
            {
                foreach (Type type in assembly.GetTypes())
                {
                    // Pick up a class
                    if (type.IsClass != true)
                        continue;

                    if (type.IsSubclassOf(typeof(IAbilityTypeHandler)))
                        continue;
                }
            }
        }
    }
}
