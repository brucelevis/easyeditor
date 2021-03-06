#include "Subject.h"
#include "Observer.h"

namespace ee
{

void Subject::Register(Observer* observer)
{
	m_observers.insert(observer);
}

void Subject::UnRegister(Observer* observer)
{
	std::set<Observer*>::iterator itr 
		= m_observers.find(observer);
	if (itr != m_observers.end()) {
		m_observers.erase(itr);
	}
}

void Subject::Notify(void* ud, Observer* except)
{
	std::set<Observer*>::iterator itr = m_observers.begin();
	for ( ; itr != m_observers.end(); ++itr) {
		if (*itr == except) {
			continue;
		}
		(*itr)->Notify(m_id, ud);
	}
}

}