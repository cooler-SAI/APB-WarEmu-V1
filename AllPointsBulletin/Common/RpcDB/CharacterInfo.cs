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
