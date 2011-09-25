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

public enum Opcodes : byte
{
    CL_CHECK    =   0x01,   // Le client envoit sa version pour vérification (version du launcher, verif fichier de start)
    LCR_CHECK   =   0x02,   // Le Serveur renvoi la réponse et déconnecte si besoin est.

    CL_START    =   0x03,   // Le client demande l'autorisation de démarrer.(Username + pass sha256)
    LCR_START   =   0x04,   // Le Serveur renvoi l'autorisation ou non de démarrer le client, le client se lancera avec les info du server

    CL_CREATE   =   0x05,   // Le client demande de créer un compte (Username + Pass + IP);
    LCR_CREATE  =   0x06,   // Réponse du serveur a la demande du client( reponse en string du message + bool d'erreur)

    CL_INFO     =   0x07,   // Le client demande les informations sur les royaumes
    LCR_INFO    =   0x08,   // Réponse du serveur sur les royaumes
};

public enum CheckResult : byte
{
    LAUNCHER_OK =       0x00,   // Le Launcher est ok
    LAUNCHER_VERSION =  0x01,   // Mauvaise version du launcher
    LAUNCHER_FILE    =  0x02,   // Fichier manquant dans le launcher
};