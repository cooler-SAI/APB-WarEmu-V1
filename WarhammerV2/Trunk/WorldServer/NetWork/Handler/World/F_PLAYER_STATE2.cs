using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;
using FrameWork.Logger;
using FrameWork.NetWork;

namespace WorldServer
{

    public enum MovementFlag : byte
    {        
        RECULE = 84,            // 001010100
        AVANCE = 192,           // 011000000
        IMMOBILE = 254,         // 111111100

        SAUTE = 4,
        GDBOUGE = 8,            // 000001000
        GAUCHE = 32,            // 000010000
        DROITE = 64,            // 000100000

        HORS_COMBAT = 31,   // 000011111
        COMBAT = 95,        // 001011111
    }

    [PacketHandlerAttribute(PacketHandlerType.TCP, (int)Opcodes.F_PLAYER_STATE2, "onPlayerState2")]
    public class F_PLAYER_STATE2 : IPacketHandler
    {
        public void HandlePacket(BaseClient client, PacketIn packet)
        {
            GameClient cclient = client as GameClient;

            if (cclient.Plr == null || !cclient.Plr.IsInWorld())
                return;

            Player Plr = cclient.Plr;

            try
            {
                long Pos = packet.Position;

                PacketOut Out = new PacketOut((byte)Opcodes.F_PLAYER_STATE2);
                Out.Write(packet.ToArray(), (int)packet.Position, (int)packet.Size);
                Out.WriteByte(0);
                Plr.DispatchPacket(Out, false);

                packet.Position = Pos;
            }
            catch (Exception e)
            {
                Log.Error("F_PLAYER_STATE2", e.ToString());
            }

            if (packet.Size < 18)
            {
                Plr.IsMoving = false;
                return;
            }

            UInt16 Key = packet.GetUint16();

            byte MoveByte = packet.GetUint8();
            byte UnkByte = packet.GetUint8();
            byte CombatByte = packet.GetUint8();
            byte RotateByte = packet.GetUint8();

            UInt16 Heading = packet.GetUint16R();
            UInt16 X = packet.GetUint16R();
            UInt16 Y = packet.GetUint16R();
            byte Unk1 = packet.GetUint8();
            UInt16 Z = packet.GetUint16R();
            byte Unk2 = packet.GetUint8();

            Heading /= 8;
            X /= 2;
            Y /= 2;
            Z /= 2;

            if (CombatByte >= 50 && CombatByte < 0x92 || CombatByte == 0xDF)
            {
                if (Plr.LastCX != 0 && Plr.LastCY != 0)
                {
                    if (Plr.LastCX > 12288 && X < 4096)
                        Plr.SetOffset((ushort)(Plr.XOffset + 1), Plr.YOffset);
                    else if (X > 12288 && Plr.LastCX < 4096)
                        Plr.SetOffset((ushort)(Plr.XOffset - 1), Plr.YOffset);

                    if (Plr.LastCY > 24576 && Y < 8192)
                        Plr.SetOffset(Plr.XOffset, (ushort)(Plr.YOffset + 1));
                    else if (Y > 24576 && Plr.LastCY < 8192)
                        Plr.SetOffset(Plr.XOffset, (ushort)(Plr.YOffset - 1));
                }

                Plr.LastCX = X;
                Plr.LastCY = Y;

                X = Plr.Zone.CalculCombat(X, Plr.XOffset, true);
                Y = Plr.Zone.CalculCombat(Y, Plr.YOffset, false);
                Heading /= 2;
                Z += 32768;
                Z /= 4;
            }
            else
            {
                if (Plr.LastX != 0 && Plr.LastY != 0)
                {
                    if (Plr.LastX > 24576 && X < 4096)
                        Plr.SetOffset((ushort)(Plr.XOffset + 1), Plr.YOffset);
                    else if (Plr.LastX < 4096 && X > 24576)
                        Plr.SetOffset((ushort)(Plr.XOffset - 1), Plr.YOffset);

                    if (Plr.LastY > 24576 && Y < 4096)
                        Plr.SetOffset(Plr.XOffset, (ushort)(Plr.YOffset + 1));
                    else if (Plr.LastY < 4096 && Y > 24576)
                        Plr.SetOffset(Plr.XOffset, (ushort)(Plr.YOffset - 1));

                }

                Plr.LastX = X;
                Plr.LastY = Y;

                X = Plr.Zone.CalculPin(X, Plr.XOffset, true);
                Y = Plr.Zone.CalculPin(Y, Plr.YOffset, false);
            }
            cclient.Plr.SetPosition(X, Y, Z, Heading);
        }
    }
}
