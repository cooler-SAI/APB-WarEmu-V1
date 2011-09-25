#ifndef STATSMGR_H
#define STATSMGR_H

class StatsMgr : public Singleton<StatsMgr>
{
	public:
		StatsMgr();
		void Update();

		string GetDayString() 
		{ 
			stringstream ss;
			ss << m_day.tm_year+1900 << "-";
			if(m_day.tm_mon+1 < 10)
				ss << "0";
			ss << m_day.tm_mon+1 << "-" << m_day.tm_mday;
			return ss.str();
		}


	private:
		uint32 m_starttime;
		uint32 m_connexionpic;
		uint32 m_latency;
		tm m_day;
};

#endif