#ifndef ABILITYTARGET_H
#define ABILITYTARGET_H

class Unit;
class Player;
class Creature;
struct AbilityInfo;

class AbilityTarget
{

public:
	AbilityTarget()
	{ 
		m_count=0;
	}

	void AddTarget(Unit * unit) 
	{ 
		Log.Debug("Target","AddTarget %s",unit->GetName().c_str());
		M_targets.Acquire();
		m_targetslist.push_back(unit);
		M_targets.Release(); 
	}

	bool HasTarget(Unit * unit)
	{
		M_targets.Acquire();
		for(std::vector<Unit*>::iterator itr=m_targetslist.begin();itr!=m_targetslist.end();++itr)
		{
			if( (*itr) == unit )
			{
				M_targets.Release();
				return true;
			}
		}
		M_targets.Release();
		return false;
	}
	void RemoveTarget(Unit * unit) 
	{ 
		M_targets.Acquire(); 
		for(vector<Unit*>::iterator i = m_targetslist.begin(); i != m_targetslist.end(); ++i )
		{
			if((*i) == unit)
			{
				m_targetslist.erase(i);
				break;
			}
		}
		M_targets.Release(); 
	};
	void ClearTargets() 
	{ 
		M_targets.Acquire(); 
		m_targetslist.clear(); 
		M_targets.Release(); 
		m_count=0;
	}

	// Comme sa , plus qu'a faire un while(GetNextTarget())
	bool NextTarget()
	{
		M_targets.Acquire();
		if(m_count >= m_targetslist.size())
		{
			M_targets.Release();
			m_count=0;
			return false;
		}

		++m_count;
		M_targets.Release();
		return true;
	}

	void ClearCount() { m_count=0; }

	Unit * GetTarget()
	{
		Unit * target=NULL;

		M_targets.Acquire();
		if(m_count<m_targetslist.size())
			target=m_targetslist[m_count];
		M_targets.Release();

		return target;
	}

	bool HasTargets() { return (m_targetslist.size() > 0); }


	Mutex M_targets;
	size_t m_count;
	vector<Unit*> m_targetslist;

};

#endif