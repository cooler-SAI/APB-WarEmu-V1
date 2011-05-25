using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Security.Cryptography;

using Common;
using FrameWork;

namespace CharacterServer
{
    [ConsoleHandler("caccount", 2, "<Username,Password>")]
    public class CreateAccount : IConsoleHandler
    {
        public bool HandleCommand(string command, List<string> args)
        {
            Account Acct = Program.AcctMgr.GetAccountByUsername(args[0]);
            if (Acct != null)
            {
                Log.Error("CreateAccount", "Username '" + args[0] + "' Already exist.");
                return false;
            }

            Log.Debug("CreateAccount", "1");

            Acct = new Account();
            Acct.Username = args[0].ToUpper();
            Acct.Sha_Password = MakePassword(args[1]);
            Acct.SessionKey = "";
            Acct.Email = "";

            Log.Debug("CreateAccount", "2");

            if (AccountMgr.AccountDB.AddObject(Acct))
                Log.Success("CreateAccount", "New Account : '" + Acct.Username + "'-'" + Acct.Sha_Password + "'");
            else
                Log.Error("CreateAccount", "Can not create account : " + Acct.Username);

            Log.Debug("CreateAccount", "3");

            return true;
        }

        static public string MakePassword(string Password)
        {
            string Sha_Password = "";

            SHA1CryptoServiceProvider Crypto = new SHA1CryptoServiceProvider();
            Crypto.Initialize();
            byte[] Result = Crypto.ComputeHash(Encoding.UTF8.GetBytes(Password), 0, Encoding.UTF8.GetByteCount(Password));
            Sha_Password = BitConverter.ToString(Result).Replace("-", string.Empty).ToLower();

            return Sha_Password;
        }


    }
}
