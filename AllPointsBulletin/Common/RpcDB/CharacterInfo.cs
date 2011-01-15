using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace Common
{
    [Serializable]
    public class CharacterInfo
    {
        private DBCharacter _Character;     // Informations contenues dans la DB
        private bool _Enter = false;                // Personnage en cours de connexion
        private UInt16 _InstanceId = 0;         // Instance sur laquel se trouve le personnage
        private bool _Online = false;               // True si le joueur est en jeu

        public CharacterInfo(DBCharacter Character)
        {
            _Character = Character;
        }

        public DBCharacter Character
        {
            get { return _Character; }
            set { _Character = value; }
        }

        public bool Enter
        {
            get { return _Enter; }
            set { _Enter = value; }
        }

        public UInt16 InstanceId
        {
            get { return _InstanceId; }
            set { _InstanceId = value; }
        }

        public bool Online
        {
            get { return _Online; }
            set { _Online = value; }
        }
    }
}
