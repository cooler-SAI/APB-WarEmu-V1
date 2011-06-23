using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading;

using Common;
using FrameWork;

namespace WorldServer
{
    public class LoaderMgr
    {
        public delegate void LoadFunction();
        public delegate void MultiLoadFunction(int ThreadCount,int Id);
        static public int LoaderCount = 0;

        static public void InitLoad(LoadFunction Func)
        {
            new LoaderMgr(Func);
            Thread.Sleep(50);
        }
        static public void InitMultiLoad(MultiLoadFunction Func,int Count)
        {
            for (int i = 0; i < Count; ++i)
                new LoaderMgr(Func, Count,i);
        }

        static public void InitMultiLoad()
        {

        }

        static public void Wait()
        {
            while (LoaderCount > 0)
                Thread.Sleep(50);
        }

        private LoadFunction _Function;
        public LoaderMgr(LoadFunction Function)
        {
            _Function = Function;
            ThreadStart Start = new ThreadStart(Load);
            Thread LoadThread = new Thread(Start);
            LoadThread.Start();
        }
        public void Load()
        {
            System.Threading.Interlocked.Increment(ref LoaderCount);
            try
            {
                if (_Function != null)
                {
                    Log.Debug("Load", "Chargement de : " + _Function.Method.Name);
                    _Function.Invoke();
                }
            }
            catch (Exception e)
            {
                Log.Error(_Function.Method.Name, e.ToString());
            }
            finally
            {
                System.Threading.Interlocked.Decrement(ref LoaderCount);
            }
        }

        private MultiLoadFunction _MultiFunction;
        private int Id;
        private int Count;
        public LoaderMgr(MultiLoadFunction Function,int Count,int Id)
        {
            _MultiFunction = Function;
            this.Count = Count;
            this.Id = Id;
            ThreadStart Start = new ThreadStart(MultiLoad);
            Thread LoadThread = new Thread(Start);
            LoadThread.Start();
        }

        public void MultiLoad()
        {
            System.Threading.Interlocked.Increment(ref LoaderCount);
            try
            {
                if (_MultiFunction != null)
                {
                    Log.Debug("Load", "Chargement de : " + _MultiFunction.Method.Name +", Id="+Id);
                    _MultiFunction.Invoke(Count,Id);
                }
            }
            catch (Exception e)
            {
                Log.Error(_MultiFunction.Method.Name, e.ToString());
            }
            finally
            {
                System.Threading.Interlocked.Decrement(ref LoaderCount);
            }
        }
    }
}
