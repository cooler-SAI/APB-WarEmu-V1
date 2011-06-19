using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;
using FrameWork;

namespace CharacterServer
{
    [ISerializableAttribute((long)Opcodes.LobbyCharacterCreationCacheRequest)]
    public class LobbyCharacterCreationCacheRequest : ISerializablePacket
    {
        public override void OnRead(RiftClient From)
        {
            Log.Success("CreationCache", "Requesting Creation cache request : " + From.Rm.RpcInfo.Description());

            if (From.Acct == null || From.Rm == null)
                return;

            CacheTemplate[] Tmps = From.Rm.GetObject<WorldMgr>().GetTemplates();
            foreach (CacheTemplate Tmp in Tmps)
                From.SendSerialized(WorldMgr.BuildCache(Tmp.CacheID, Tmp.CacheType, Tmp));

            CacheData[] Dts = From.Rm.GetObject<WorldMgr>().GetDatas();
            foreach (CacheData Tmp in Dts)
                From.SendSerialized(WorldMgr.BuildCache(Tmp.CacheID, Tmp.CacheType, Tmp));

            ISerializablePacket Packet = new ISerializablePacket();
            Packet.Opcode = (long)Opcodes.LobbyCharacterCreationCacheResponse;
            From.SendSerialized(Packet);
        }
    }
}
