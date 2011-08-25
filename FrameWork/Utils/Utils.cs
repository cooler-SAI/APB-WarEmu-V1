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
                    L.Add((T)Convert.ChangeType(Val, typeof(T)));

            return L;
        }
        static public string ConvertArrayToString<T>(T[] Value)
        {
            string Result = "";
            foreach (T val in Value)
                Result += (string)Convert.ChangeType(val, typeof(string)) + " ";

            return Result;
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
