using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork;

namespace RealmServer
{
    [aConfigAttributes("Configs/RealmServer.xml")]
    public class RealmConfig : aConfig
    {
        public string LocalRpcIP = "127.0.0.1";
        public string RpcServerIp = "127.0.0.1";
        public int RpcServerPort = 6800;

        public Realm RealmInfo = new Realm();

        public DatabaseInfo CharactersDB = new DatabaseInfo();
        public DatabaseInfo WorldDB = new DatabaseInfo();
        public LogInfo LogLevel = new LogInfo();
    }
}
