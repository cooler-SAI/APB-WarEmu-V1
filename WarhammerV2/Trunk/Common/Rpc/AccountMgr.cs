/*
 * Copyright (C) 2011 APS
 *	http://AllPrivateServer.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
 
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Security.Cryptography;

using FrameWork;

namespace Common
{
    public enum AuthResult
    {
        AUTH_SUCCESS = 0x00,
        AUTH_ACCT_EXPIRED = 0x07,
        AUTH_ACCT_BAD_USERNAME_PASSWORD = 0x09,
        AUTH_ACCT_TERMINATED = 0x0D,
        AUTH_ACCT_SUSPENDED = 0x0E
    };

    [Rpc(true, System.Runtime.Remoting.WellKnownObjectMode.Singleton,1)]
    public class AccountMgr : RpcObject
    {
        // Account Database
        static public MySQLObjectDatabase Database = null;

        #region Account

        // Account : Username,Account
        public Dictionary<string, Account> _Accounts = new Dictionary<string, Account>();

        public bool LoadAccount(string Username)
        {
            Username = Username.ToLower();
            Log.Info("LoadAccount", Username);
            try
            {
                lock (_Accounts)
                    if (_Accounts.ContainsKey(Username))
                        return true;

                Account Acct = Database.SelectObject<Account>("Username='" + Database.Escape(Username) + "'");

                if (Acct == null)
                {
                    Log.Error("LoadAccount", "Compte Introuvable : " + Username);
                    return false;
                }

                lock (_Accounts)
                    _Accounts.Add(Username, Acct);
            }
            catch (Exception e)
            {
                Log.Error("LoadAccount", e.ToString());
                return false;
            }

            return true;
        }
        public Account GetAccount(string Username)
        {
            Username = Username.ToLower();

            Log.Info("GetAccount", Username);

            if (!LoadAccount(Username))
            {
                Log.Error("GetAccount", "Compte Introuvable : " + Username);
                return null;
            }

            lock (_Accounts)
                return _Accounts[Username];
        }
        public bool CheckAccount(string Username, byte[] Password)
        {
            Username = Username.ToLower();

            Log.Info("CheckAccount", Username + " : " + Password);

            try
            {
                Account Acct = GetAccount(Username);

                if (Acct == null)
                {
                    Log.Error("CheckAccount", "Compte introuvable : " + Username);
                    return false;
                }

                SHA256Managed Sha = new SHA256Managed();
                string CP = Acct.Username + ":" + Acct.Password;
                Log.Error("Cp", "."+CP+".");

                byte[] Result = Sha.ComputeHash(UTF8Encoding.UTF8.GetBytes(CP));
                Log.Dump("Result", Result, 0, Result.Length);
                Log.Dump("Pass", Password, 0, Password.Length);

                if (Result.Length != Password.Length)
                {
                    Log.Error("CheckAccount", "Taille du pass invalide : " + Password.Length);
                    return false;
                }

                for (int i = 0; i < Result.Length; ++i)
                    if (Result[i] != Password[i])
                        return false;

            }
            catch (Exception e)
            {
                Log.Error("CheckAccount", e.ToString());
                return false;
            }

            return true;
        }
        public string GenerateToken(string Username)
        {
            Username = Username.ToLower();
            Log.Info("GenerateToken", Username);
            Account Acct = GetAccount(Username);

            if (Acct == null)
            {
                Log.Error("GenerateToken", "Compte introuvable : " + Username);
                return "ERREUR";
            }

            string Token = Guid.NewGuid().ToString();
            if (Token.Length <= 34)
            {
                for (int i = Token.Length; i < 34; ++i)
                    Token += "X";
            }
            else Token = Token.Substring(0, 34);

            Acct.Token = Token;
            
            Log.Debug("GenerateToken",Acct.Token);
            
            Database.SaveObject(Acct);

            return Acct.Token;
        }
        public AuthResult CheckToken(string Username, string Token)
        {
            Username = Username.ToLower();

            Account Acct = GetAccount(Username);
            if (Acct == null)
                return AuthResult.AUTH_ACCT_BAD_USERNAME_PASSWORD;

            if (Acct.Token != Token)
                return AuthResult.AUTH_ACCT_BAD_USERNAME_PASSWORD;

            return AuthResult.AUTH_SUCCESS;
        }

        #endregion

        #region Realm

        public Dictionary<byte, Realm> _Realms = new Dictionary<byte, Realm>();
        
        public void LoadRealms()
        {
            Realm[] Rms = Database.SelectAllObjects<Realm>().ToArray();
            foreach (Realm Rm in Rms)
                AddRealm(Rm);
        }
        public bool AddRealm(Realm Rm)
        {
            lock (_Realms)
            {
                if (_Realms.ContainsKey(Rm.RealmId))
                    return false;

                Log.Info("AddRealm", "New Realm : " + Rm.Name);

                _Realms.Add(Rm.RealmId, Rm);
            }

            return true;
        }
        public Realm GetRealm(byte RealmId)
        {
            Log.Info("GetRealm", "RealmId = " + RealmId);
            lock (_Realms)
                if (_Realms.ContainsKey(RealmId))
                    return _Realms[RealmId];

            return null;
        }
        public List<Realm> GetRealms()
        {
            List<Realm> Rms = new List<Realm>();
            Rms.AddRange(_Realms.Values);
            return Rms;
        }
        public Realm GetRealmByRpc(int RpcId)
        {
            lock (_Realms)
                return _Realms.Values.ToList().Find(info => info.Info != null && info.Info.RpcID == RpcId);
        }
        public bool UpdateRealm(RpcClientInfo Info, byte RealmId)
        {
            Realm Rm = GetRealm(RealmId);

            if (Rm != null)
            {
                Log.Success("Realm", "Realm (" + Rm.Name + ") online at " + Info.Ip+":"+Info.Port);
                Rm.Info = Info;
            }
            else
            {
                Log.Error("UpdateRealm", "Realm (" + RealmId + ") missing : Please complete the table 'realm'");
                return false;
            }

            return true;
        }
        public void UpdateRealm(byte RealmId, uint OnlinePlayers, uint OrderCount, uint DestructionCount)
        {
            Realm Rm = GetRealm(RealmId);

            if (Rm == null)
                return;

            Rm.OnlinePlayers = OnlinePlayers;
            Rm.OrderCount = OrderCount;
            Rm.DestructionCount = DestructionCount;
        }
        public byte[] BuildRealms(uint sequence)
        {
            try
            {
                PacketOut Out = new PacketOut((byte)0);
                Out.Position = 0;

                Out.WriteUInt32(sequence);
                Out.WriteUInt16(0);
                lock (_Realms)
                {
                    Log.Info("BuildRealm", "Sending " + _Realms.Count + " realm(s)");
                    Out.WriteUInt32((uint)_Realms.Count);

                    foreach (Realm Rm in _Realms.Values)
                    {
                        Out.WriteByte(Rm.RealmId);
                        Out.WriteByte((byte)(Rm.Info != null ? 1 : 0));
                        Out.WriteUInt32(1);
                        Out.WriteByte(Rm.RealmId);
                        Out.WriteByte(Rm.RealmId);
                        Out.WriteString("[" + Rm.Language + "] " + Rm.Name);
                        Out.WriteUInt32(19);
                        Out.WriteString("setting.allow_trials");
                        Out.WriteString(Rm.AllowTrials);
                        Out.WriteString("setting.charxferavailable");
                        Out.WriteString(Rm.CharfxerAvailable);
                        Out.WriteString("setting.language");
                        Out.WriteString(Rm.Language);
                        Out.WriteString("setting.legacy");
                        Out.WriteString(Rm.Legacy);
                        Out.WriteString("setting.manualbonus.realm.destruction");
                        Out.WriteString(Rm.BonusDestruction);
                        Out.WriteString("setting.manualbonus.realm.order");
                        Out.WriteString(Rm.BonusOrder);
                        Out.WriteString("setting.name");
                        Out.WriteString(Rm.Name);
                        Out.WriteString("setting.net.address");
                        Out.WriteString(Rm.Adresse);
                        Out.WriteString("setting.net.port");
                        Out.WriteString(Rm.Port.ToString());
                        Out.WriteString("setting.redirect");
                        Out.WriteString(Rm.Redirect);
                        Out.WriteString("setting.region");
                        Out.WriteString(Rm.Region);
                        Out.WriteString("setting.retired");
                        Out.WriteString(Rm.Retired);
                        Out.WriteString("status.queue.Destruction.waiting");
                        Out.WriteString(Rm.WaitingDestruction);
                        Out.WriteString("status.queue.Order.waiting");
                        Out.WriteString(Rm.WaitingOrder);
                        Out.WriteString("status.realm.destruction.density");
                        Out.WriteString(Rm.DensityDestruction);
                        Out.WriteString("status.realm.order.density");
                        Out.WriteString(Rm.DensityOrder);
                        Out.WriteString("status.servertype.openrvr");
                        Out.WriteString(Rm.OpenRvr);
                        Out.WriteString("status.servertype.rp");
                        Out.WriteString(Rm.Rp);
                        Out.WriteString("status.status");
                        Out.WriteString(Rm.Status);
                    }
                }
                Out.WriteUInt32(0);

                return Out.ToArray();
            }
            catch
            {
                return new byte[0];
            }
        }
        public override void  OnClientDisconnected(RpcClientInfo Info)
        {
            Realm Rm = GetRealmByRpc(Info.RpcID);
            if (Rm != null && Rm.Info.RpcID == Info.RpcID)
            {
                Log.Error("Realm", "Realm offline : " + Rm.Name);
                Rm.Info = null;
            }
        }

        #endregion
    }
}
