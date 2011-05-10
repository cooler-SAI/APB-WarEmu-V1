using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork;

namespace Common
{
    [Rpc(false, System.Runtime.Remoting.WellKnownObjectMode.Singleton, 1)]
    public class CharacterMgr : RpcObject
    {
        static public MySQLObjectDatabase CharactersDB;

        #region Characters


        #endregion
    }
}
