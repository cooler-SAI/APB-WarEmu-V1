#ifndef MAILINTERFACE_H
#define MAILINTERFACE_H

#define MAIL_PRICE 30

class Player;
class Unit;
class Player;
class Buffer;

struct ItemProto;

struct CharacterMail
{
	uint32 num;		// Num�ro dans la liste

	uint32 guid;	// Num�ro de la lettre

	uint32 receiver;// character_id du receiver
	uint32 sender;	// de l'envoyeur

	string receivername;  // Nom de l'envoyeur
	string sendername;  // Nom de l'envoyeur

	string title;	// Titre du message
	string content;	// Texte du message
	uint32 money;	// Argent envoy� ou demand�
	bool cr;		// false = envoy� , true = demand�
	bool opened;	// true si d�ja ouvert
	vector<Item*> m_items; // Liste des items envoy�

	void Save();	// Sauvegarde le mail
	void Delete();	// Supprime le mail
};

class MailInterface
{
public:
	MailInterface(Player * Plr) : m_player(Plr) {}

	void Load();

	// Mail Build
	void BuildMail(Buffer &m_readBuffer);
	void BuildPreMail(Buffer *b,CharacterMail * Mail);

	// Mailbox
	void sendMailCounts();
	void sendMailBox(Unit * sender=NULL);
	void sendResult(MailResult Result);

	// Mail Mod
	void AddMail(CharacterMail * Mail);		// Ajoute un mail
	void sendMail(CharacterMail * Mail);	// Envoi le contenu du mail
	void sendMailUpdate(CharacterMail * Mail);

	CharacterMail * GetMail(uint32 guid);
	CharacterMail * GetMail(uint8 num);

	void RemoveMail(uint32 num)
	{
		M_mails.Acquire();
		for(vector<CharacterMail*>::iterator itr=m_mails.begin();itr!=m_mails.end();++itr)
			if( (*itr) && (*itr)->num == num)
			{
				m_mails.erase(itr);
				(*itr)->Delete();
				break;
			}
		M_mails.Release();
		sendResult(TEXT_MAIL_RESULT12);
	}

	void RemoveMail(CharacterMail * Mail)
	{
		M_mails.Acquire();
		for(vector<CharacterMail*>::iterator itr=m_mails.begin();itr!=m_mails.end();++itr)
			if( (*itr) && (*itr) == Mail)
			{
				m_mails.erase(itr);
				Mail->Delete();
				break;
			}
		M_mails.Release();
		sendResult(TEXT_MAIL_RESULT12);
	}

private:
	Player * m_player;				// Owner de la class

	Mutex M_mails;
	vector<CharacterMail*> m_mails;	// Liste des mails re�us

	uint32 m_nextsend;	// 5 secondes entre chaque envoi
};


#endif