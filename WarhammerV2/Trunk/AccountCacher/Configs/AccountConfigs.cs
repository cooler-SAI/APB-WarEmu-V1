using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork.Config;
using FrameWork.Database;
using FrameWork.RpcV3;

namespace AccountCacher
{
    [ConfigAttributes("Configs/Account.xml")]
    public class AccountConfigs : Config
    {
        public DatabaseInfo AccountDatabase = new DatabaseInfo();
        public RpcServerInfo RpcServer = new RpcServerInfo("Pass", 2100);
    }
}
