using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;
using FrameWork.Logger;
using FrameWork.NetWork;

namespace WorldServer
{
    [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.F_SWITCH_ATTACK_MODE, "onSwitchAttackMode")]
    public class F_SWITCH_ATTACK_MODE : IPacketHandler
    {
        public void HandlePacket(BaseClient client, PacketIn packet)
        {
            GameClient cclient = client as GameClient;
            if (!cclient.HasPlayer())
                return;

            Player Plr = cclient.Plr;

            Object Target = Plr.CbtInterface.GetObjectTarget();
            if (Target == null)
            {
                Log.Error("F_SWITCH_ATTACK_MODE", "Target == null");
                return;
            }

            if (Plr.CbtInterface.Target.Type != GameData.TargetTypes.TARGETTYPES_TARGET_ENEMY)
            {
                Log.Error("F_SWITCH_ATTACK_MODE", "Invalide target !");
                return;
            }

            Plr.CbtInterface.Attacking = true;
        }
    }
}
