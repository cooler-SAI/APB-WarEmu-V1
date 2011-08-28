using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Reflection;

namespace FrameWork
{
    static public class Utils
    {
        static public List<T> ConvertStringToArray<T>(string Value)
        {
            string[] Values = Value.Split(' ');
            List<T> L = new List<T>();
            foreach (string Val in Values)
                if (Val.Length > 0)
                {
                    try
                    {
                        L.Add((T)Convert.ChangeType(Val, typeof(T)));
                    }
                    catch
                    {
                        L.Add((T)Convert.ChangeType("0",typeof(T)));
                    }
                }

            return L;
        }
        static public string ConvertArrayToString<T>(T[] Value)
        {
            string Result = "";
            foreach (T val in Value)
                Result += (string)Convert.ChangeType(val, typeof(string)) + " ";

            return Result;
        }

        static public string VectorToString(Vector3 Vec)
        {
            return PositionToString(Vec.X, Vec.Y, Vec.Z);
        }
        static public Vector3 StringToVector(string Str)
        {
            float[] Value = StringToPosition(Str);
            return new Vector3(Value[0], Value[1], Value[2]);
        }

        static public string PositionToString(float X, float Y, float Z)
        {
            return "" + X + ":" + Y + ":" + Z;
        }
        static public float[] StringToPosition(string Str)
        {
            float[] Result = new float[3] { 0, 0, 0 };
            string[] Value = Str.Split(':');
            for (int i = 0; i < Result.Length; ++i)
                if (Value.Length >= i)
                    Result[i] = float.Parse(Value[i]);
            return Result;
        }

        static public string ChannelsToString(Dictionary<byte, Color> Chans)
        {
            string Result = "";

            if (Chans.Count <= 0)
                return Result;

            foreach (KeyValuePair<byte, Color> Kp in Chans)
                Result += Kp.Key.ToString() + "," + ColorToString(Kp.Value) + "|";

            // 0 - 1 - 2    
            return Result.Remove(Result.Length - 1, 1);
        }
        static public Dictionary<byte, Color> StringToChannels(string Str)
        {
            // 1,18:20:25:45|2,457:154:651:0
            Dictionary<byte, Color> Chans = new Dictionary<byte, Color>();
            if (Str.Length <= 0)
                return Chans;

            foreach (string Kp in Str.Split('|'))
            {
                if (Kp.Length <= 0)
                    continue;

                byte ChannelId = byte.Parse(Kp.Split(',')[0]);
                Color Col = StringToColor(Kp.Split(',')[1]);

                if (!Chans.ContainsKey(ChannelId))
                    Chans.Add(ChannelId, Col);
            }
            return Chans;
        }

        static public string UintArrayToString(uint[] Values)
        {
            string Result = "";

            if (Values.Length <= 0)
                return Result;

            foreach (int Val in Values)
                Result += Val.ToString() + ":";

            return Result.Remove(Result.Length - 1, 1);
        }
        static public uint[] StringToUintArray(string Str)
        {
            if (Str.Length <= 0)
                return new uint[0];

            string[] Values = Str.Split(':');
            uint[] Result = new uint[Values.Length];

            for (int i = 0; i < Values.Length; ++i)
                if (Values[i].Length > 0)
                    Result[i] = uint.Parse(Values[i]);

            return Result;
        }

        static public string EquipementToString(Dictionary<byte, uint> Chans)
        {
            string Result = "";

            if (Chans.Count <= 0)
                return Result;

            foreach (KeyValuePair<byte, uint> Kp in Chans)
                Result += Kp.Key.ToString() + "," + Kp.Value.ToString() + "|";

            return Result.Remove(Result.Length - 1, 1);
        }
        static public Dictionary<byte, uint> StringToEquipement(string Str)
        {
            // 1,134|2,18
            Dictionary<byte, uint> Chans = new Dictionary<byte, uint>();
            if (Str.Length <= 0)
                return Chans;

            foreach (string Kp in Str.Split('|'))
            {
                if (Kp.Length <= 0)
                    continue;

                byte ChannelId = byte.Parse(Kp.Split(',')[0]);
                uint ItemId = uint.Parse(Kp.Split(',')[1]);

                if (!Chans.ContainsKey(ChannelId))
                    Chans.Add(ChannelId, ItemId);
            }
            return Chans;
        }

        static public string ColorToString(Color Col)
        {
            return Col.R + ":" + Col.G + ":" + Col.B + ":" + Col.A;
        }
        static public Color StringToColor(string Str)
        {
            string[] Bytes = Str.Split(':');
            Color Col = new Color();
            Col.R = float.Parse(Bytes[0]);
            Col.G = float.Parse(Bytes[1]);
            Col.B = float.Parse(Bytes[2]);
            Col.A = float.Parse(Bytes[3]);
            return Col;

        }
        public static String AssemblyVersion
        {
            get
            {
                return System.Reflection.Assembly.GetExecutingAssembly().GetName().Version.ToString();
            }
        }
    }
}
