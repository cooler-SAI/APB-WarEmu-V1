using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Common;

namespace WorldServer
{
    public class MailData
    {
        public Character_mail Mail;
        public List<Item> Items;

        public MailData(Character_mail Mail)
        {
            this.Mail = Mail;
            Items = null;
        }

        public void LoadItems(Player Plr)
        {
            if (Items != null)
                return;

            Items = new List<Item>();
            if (Mail.Items != null)
            {
                foreach (UInt32 Id in Mail.Items)
                {
                    Item Itm = new Item(Plr);
                    Itm.Load(CharMgr.GetItem(Id));
                    Items.Add(Itm);
                }
            }
        }
    }

    public class MailsInterface : BaseInterface
    {
        public UInt32 CharacterID;
        public List<MailData> Mails = new List<MailData>();

        public override bool Load()
        {
            foreach (MailData Mail in Mails)
                Mail.LoadItems(_Owner.GetPlayer());

            return true;
        }

        public MailData GetMail(UInt32 MailID)
        {
            lock (Mails)
            {
                foreach (MailData Mail in Mails)
                    if (Mail.Mail.Guid == MailID)
                        return Mail;
            }

            return null;
        }

        public void AddMail(Character_mail Mail)
        {
            lock (Mails)
            {
                Mails.Add(new MailData(Mail));
            }
        }

        public void RemoveMail(MailData Mail)
        {
            lock (Mails)
            {
                Mails.Remove(Mail);
            }
        }
    }
}
