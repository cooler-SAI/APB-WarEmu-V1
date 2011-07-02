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
        public RpcClientConfig RpcCharacter = new RpcClientConfig("127.0.0.1","127.0.0.1",6800);
        public RpcServerConfig RpcMapServer = new RpcServerConfig("127.0.0.1",6499,6500);

        public Realm RealmInfo = new Realm();

        public DatabaseInfo CharactersDB = new DatabaseInfo();
        public DatabaseInfo WorldDB = new DatabaseInfo();
        public LogInfo LogLevel = new LogInfo();
    }
}
