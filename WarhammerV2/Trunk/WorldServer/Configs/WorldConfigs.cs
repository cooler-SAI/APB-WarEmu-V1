using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using FrameWork.Config;
using FrameWork.Database;
using FrameWork.RpcV3;

namespace WorldServer
{
    [ConfigAttributes("Configs/World.xml")]
    public class WorldConfigs : Config
    {
        public DatabaseInfo CharacterDatabase = new DatabaseInfo();
        public DatabaseInfo WorldDatabase = new DatabaseInfo();

        public RpcClientInfo RpcAccountClient = new RpcClientInfo("127.0.0.1", "Pass", 2100);

        public byte RealmId = 1;

        public int GlobalLootRate = 1;
        public int CommonLootRate = 1;
        public int UncommonLootRate = 1;
        public int RareLootRate = 1;
        public int VeryRareLootRate = 1;
        public int ArtifactLootRate = 1;

        public int GoldRate = 1;
        public int XpRate = 1;
    }
}
