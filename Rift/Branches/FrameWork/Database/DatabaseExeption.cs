using System;

namespace FrameWork
{
    public class DatabaseException : ApplicationException
    {
        // Exeption levé par la database
        public DatabaseException(Exception e)
            : base("", e)
        {
        }

        // Lèvre un exeption avec le message d'erreur
        public DatabaseException(string str, Exception e)
            : base(str, e)
        {
        }

        // Raisons de l'exeption
        public DatabaseException(string str)
            : base(str)
        {
        }
    }
}