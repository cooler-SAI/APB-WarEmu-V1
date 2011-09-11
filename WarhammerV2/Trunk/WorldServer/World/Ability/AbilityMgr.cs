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
        static public MySQLObjectDatabase Database;

        static public Dictionary<UInt16, IAbilityTypeHandler> _AbilityTypes = new Dictionary<ushort, IAbilityTypeHandler>();

        static private void LoadAbilityType()
        {
            _AbilityTypes = new Dictionary<ushort, IAbilityTypeHandler>();

            Log.Debug("AbilityMgr", "Loading Ability Type Handlers...");

            foreach (Assembly assembly in AppDomain.CurrentDomain.GetAssemblies())
            {
                foreach (Type type in assembly.GetTypes())
                {
                    // Pick up a class
                    if (type.IsClass != true)
                        continue;

                    if (!type.IsSubclassOf(typeof(IAbilityTypeHandler)))
                        continue;

                    IAbilityTypeAttribute[] attrib = type.GetCustomAttributes(typeof(IAbilityTypeAttribute), true) as IAbilityTypeAttribute[];
                    if (attrib.Length <= 0)
                        continue;

                    Log.Info("AbilityMgr", "Registering Ability Type : " + attrib[0].TypeDescription +"("+attrib[0].AbilityType+")");
                    _AbilityTypes.Add(attrib[0].AbilityType, (IAbilityTypeHandler)Activator.CreateInstance(type));
                }
            }
        }

        static public bool HasAbilityHandler(UInt16 AbilityType)
        {
            return _AbilityTypes.ContainsKey(AbilityType);
        }

        #region Ability_Info

        static public Dictionary<UInt16, Ability_Info> _AbilityInfos = new Dictionary<ushort, Ability_Info>();

        static public void LoadAbilityInfo()
        {
            LoadAbilityType();

            _AbilityInfos = new Dictionary<ushort, Ability_Info>();

            Log.Debug("AbilityMgr", "Loading Ability Info...");

            IList<Ability_Info> Infos = Database.SelectAllObjects<Ability_Info>();
            int Error = 0;

            foreach (Ability_Info Info in Infos)
            {
                if (!HasAbilityHandler(Info.AbilityType))
                {
                    Log.Debug("AbilityMgr", "Ability Type of : " + Info.Entry + " do not exist : " + Info.AbilityType);
                    ++Error;
                    continue;
                }

                _AbilityInfos.Add(Info.Entry, Info);
            }

            if (Error > 0)
                Log.Error("AbilityMgr", "[" + Error + "] Ability Error");

            Log.Success("AbilityMgr", "Loaded " + _AbilityInfos.Count + " Ability Info");
        }

        #endregion
    }
}
