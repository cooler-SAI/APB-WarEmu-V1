using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;
using FrameWork;

namespace WorldServer
{

    public class GmCommandHandler
    {
        public delegate bool GmComHandler(Player Plr,ref List<string> Values);
        public GmCommandHandler(string Name, GmComHandler Handler, List<GmCommandHandler> Handlers, int GmLevel, int ValuCount,string Description)
        {
            this.Name = Name;
            this.Handler = Handler;
            this.Handlers = Handlers;
            this.GmLevel = GmLevel;
            this.ValuCount = ValuCount;
            this.Description = Description;
        }

        public string Name;
        public GmComHandler Handler;
        public List<GmCommandHandler> Handlers;
        public int GmLevel;
        public int ValuCount;
        public string Description;
    }

    static public class GmCommand
    {

        static public List<GmCommandHandler> ModifyCommands = new List<GmCommandHandler>()
        {
            new GmCommandHandler("level",ModifyLevel, null, 0, 1, "Change the level of selected Player"),
            new GmCommandHandler("speed",ModifySpeed, null, 0, 1, "Change the speed of selected Player"),
            new GmCommandHandler("renown",ModifyRenown, null, 0, 1, "Change the speed of selected Player"),
        };

        static public List<GmCommandHandler> AddCommands = new List<GmCommandHandler>()
        {
            new GmCommandHandler("xp",AddXp, null, 0, 1, "Add xp to player"),
            new GmCommandHandler("item",AddItem, null, 0, 1, "Add item to player"),
            new GmCommandHandler("money",AddMoney, null, 0, 1, "Add money to player"),
            new GmCommandHandler("tok",AddTok, null, 0, 1, "Add tok to player"),
        };

        static public List<GmCommandHandler> ChapterCommands = new List<GmCommandHandler>()
        {
            new GmCommandHandler("save",ChapterSave, null, 0, 1, "Save chapter position"),
            new GmCommandHandler("explore",ChapterTokExplore, null, 0, 2, "Set tok explore"),
            new GmCommandHandler("tokentry",ChapterTok, null, 0, 2, "Set tok entry"),
        };

        static public List<GmCommandHandler> BaseCommand = new List<GmCommandHandler>()
        {
            new GmCommandHandler("modify",null, ModifyCommands, 3, 0, "All command for modify player info"),
            new GmCommandHandler("add",null, AddCommands, 3, 0, "All commands for add somethink to player"),
            new GmCommandHandler("info",Info, null, 3, 0, "Info of selected target"),
            new GmCommandHandler("save",Save, null, 3, 0, "Save target"),
            new GmCommandHandler("gps",Gps, null, 3, 0, "Print your position"),
            new GmCommandHandler("kill",Kill, null, 3, 0, "Kill target"),
            new GmCommandHandler("move",Move, null, 3, 0, "move target to my position"),
            new GmCommandHandler("chapter",null, ChapterCommands, 3, 0, "All Chapter commands"),
        };

        static public bool HandleCommand(Player Plr, string Text)
        {
            if (Plr._Client._Account.GmLevel <= 0 || Text.Length <= 0)
                return true;

            if (Text[0] != '.')
                return true;

            Text = Text.Remove(0, 1);
            List<string> Values = Text.Split(' ').ToList();
            Values.RemoveAll(str => str.Length <= 0);

            return !DecodeCommand(Plr,ref Values, BaseCommand,null);
        }

        static public bool DecodeCommand(Player Plr,ref List<string> Values, List<GmCommandHandler> Handlers,List<GmCommandHandler> BaseHandlers)
        {
            string Command = GetString(ref Values);

            Log.Success("DecodeCommand", "Command = " + Command);

            GmCommandHandler Handler = Handlers.Find(com => com != null && com.Name.StartsWith(Command));

            if(Handler == null) // Si la commande n'existe pas , on affiche la liste des commandes
            {
                List<GmCommandHandler> Base = Handlers == null ? BaseHandlers : Handlers;
                PrintCommands(Plr,Base);
                return true;
            }
            else // Le Handler Existe
            {
                if(Handler.GmLevel > Plr.GmLevel) // GmLevel insuffisant
                {

                    return false;
                }

                if(Handler.ValuCount > Values.Count) // Nombre d'arguments insuffisant
                {

                    return true;
                }

                if(Handler.Handlers != null && Handler.Handlers.Count > 0)
                {
                    return DecodeCommand(Plr,ref Values,Handler.Handlers,Handlers);
                }

                if(Handler.Handler != null)
                {
                    return Handler.Handler.Invoke(Plr,ref Values);
                }
            }

            return false;
        }

        static public void PrintCommands(Player Plr, List<GmCommandHandler> Handler)
        {
            string Str = "";
            foreach (GmCommandHandler Com in Handler)
                Str += Com.Name + ", ";

            Plr.SendMessage(0, "", Str, SystemData.ChatLogFilters.CHATLOGFILTERS_SHOUT);
        }

        static public string GetTotalString(ref List<string> Values)
        {
            string Str = "";
            foreach (string str in Values)
                Str += str + " ";

            return Str;
        }
        static public string GetString(ref List<string> Values)
        {
            if(Values.Count <= 0)
                return "0";

            string str = Values[0];
            Values.RemoveAt(0);

            return str;
        }
        static public int GetInt(ref List<string> Values)
        {
            return int.Parse(GetString(ref Values));
        }
        static public Player GetTargetOrMe(Player Plr)
        {
            Player Target = null;

            Target = Plr.CbtInterface.GetPlayerTarget();
            if (Target == null)
                Target = Plr;

            return Target;
        }
        static public Object GetObjectTarget(Player Obj)
        {
            Object Target = Obj.CbtInterface.GetObjectTarget();
            if (Target == null)
                Target = Obj;

            return Target;
        }

        #region Commands

        #region Modify

        static public bool ModifyLevel(Player Plr,ref List<string> Values)
        {
            int Level = GetInt(ref Values);
            Plr = GetTargetOrMe(Plr);
            Plr.SetLevel((byte)Level);

            return true;
        }
        static public bool ModifySpeed(Player Plr, ref List<string> Values)
        {
            int Speed = GetInt(ref Values);
            Plr = GetTargetOrMe(Plr);
            Plr.Speed = (UInt16)Speed;
            return true;
        }

        static public bool ModifyRenown(Player Plr, ref List<string> Values)
        {
            int RenownLevel = GetInt(ref Values);
            Plr = GetTargetOrMe(Plr);
            Plr.SetRenownLevel((byte)RenownLevel);
            return true;
        }

        #endregion

        #region Add

        static public bool AddXp(Player Plr, ref List<string> Values)
        {
            int Xp = GetInt(ref Values);
            Plr = GetTargetOrMe(Plr);
            Plr.AddXp((uint)Xp);

            return true;
        }
        static public bool AddMoney(Player Plr, ref List<string> Values)
        {
            int Money = GetInt(ref Values);
            Plr = GetTargetOrMe(Plr);
            Plr.AddMoney((uint)Money);
            return true;
        }

        static public bool AddItem(Player Plr, ref List<string> Values)
        {
            int ItemId = GetInt(ref Values);
            int Count = 1;
            if (Values.Count > 0)
                Count = GetInt(ref Values);

            Plr = GetTargetOrMe(Plr);
            if (Plr.ItmInterface.CreateItem((uint)ItemId, (ushort)Count) == ItemError.RESULT_OK)
                return true;

            return false;
        }

        static public bool AddTok(Player Plr, ref List<string> Values)
        {
            int TokEntry = GetInt(ref Values);

            Tok_Info Info = WorldMgr.GetTok((UInt32)TokEntry);
            if (Info == null)
                return false;

            Plr = GetTargetOrMe(Plr);
            Plr.TokInterface.AddTok(Info.Entry);

            return false;
        }

        #endregion

        #region Info

        static public bool Info(Player Plr, ref List<string> Values)
        {
            Object Other = GetObjectTarget(Plr);
            Plr.SendMessage(0, "", Other.ToString(), SystemData.ChatLogFilters.CHATLOGFILTERS_EMOTE);
            return true;
        }

        #endregion

        #region Save

        static public bool Save(Player Plr, ref List<string> Values)
        {
            Player Other = GetTargetOrMe(Plr);
            Plr.Save();
            return true;
        }

        #endregion

        #region Gps

        static public bool Gps(Player Plr, ref List<string> Values)
        {
            Plr.CalcWorldPositions();
            Object Obj = Plr.CbtInterface.GetObjectTarget();

            string Pos = "Px="+Plr.X+",Py="+Plr.Y+",Pz="+Plr.Z;
            Pos+=",Wx="+Plr._Value.WorldX +",Wy="+Plr._Value.WorldY;
            Pos += ",Ox=" + Plr.XOffset + ",Oy=" + Plr.YOffset;
            Pos += ",Wh=" + Plr._Value.WorldO + ",Ph=" + Plr.Heading;
            if (Obj != null)
                Pos += ",Dist=" + Plr.GetDistanceTo(Obj);
            Plr.SendMessage(0, "", Pos, SystemData.ChatLogFilters.CHATLOGFILTERS_EMOTE);
            return true;
        }

        #endregion

        #region Kill

        static public bool Kill(Player Plr, ref List<string> Values)
        {
            Unit Target = Plr.CbtInterface.GetUnitTarget();
            if (Target == null)
                return false;

            Target.SetDeath(Plr);

            return true;
        }

        #endregion

        #region Movement

        static public bool Move(Player Plr, ref List<string> Values)
        {
            Object Target = Plr.CbtInterface.GetObjectTarget();
            if (Target == null || !Target.IsCreature())
                return false;

            Target.GetCreature().MvtInterface.WalkTo(Plr, 100);
            return true;

        }

        #endregion

        #region Toks

        static public bool ChapterSave(Player Plr, ref List<string> Values)
        {
            int Entry = GetInt(ref Values);

            Chapter_Info Info = WorldMgr.GetChapter((UInt32)Entry);
            if (Info == null)
                return false;

            Info.PinX = (ushort)Plr.X;
            Info.PinY = (ushort)Plr.Y;

            Plr.SendMessage(Plr, "Saved [" + Info.Name + "] to '" + Plr.X + "','" + Plr.Y + "'", SystemData.ChatLogFilters.CHATLOGFILTERS_SAY);

            Info.Dirty = true;
            WorldMgr.Database.SaveObject(Info);

            return true;
        }

        static public bool ChapterTokExplore(Player Plr, ref List<string> Values)
        {
            int Entry = GetInt(ref Values);
            int TokExplore = GetInt(ref Values);

            Chapter_Info Chapter = WorldMgr.GetChapter((UInt32)Entry);
            Tok_Info Tok = WorldMgr.GetTok((UInt32)Entry);

            if (Tok == null || Chapter == null)
                return false;

            Chapter.TokExploreEntry = (UInt32)TokExplore;
            Chapter.Dirty = true;
            WorldMgr.Database.SaveObject(Chapter);

            return true;
        }

        static public bool ChapterTok(Player Plr, ref List<string> Values)
        {
            int Entry = GetInt(ref Values);
            int TokEntry = GetInt(ref Values);

            Chapter_Info Chapter = WorldMgr.GetChapter((UInt32)Entry);
            Tok_Info Tok = WorldMgr.GetTok((UInt32)Entry);

            if (Tok == null || Chapter == null)
                return false;

            Chapter.TokEntry = (UInt32)TokEntry;
            Chapter.Dirty = true;
            WorldMgr.Database.SaveObject(Chapter);

            return true;
        }

        #endregion

        #endregion
    }
}
