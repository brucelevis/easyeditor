#ifndef _DRAG2D_OBSERVER_H_
#define _DRAG2D_OBSERVER_H_

#include <vector>

namespace d2d
{

class Subject;

class Observer
{
public:
	Observer();
	virtual ~Observer();

	void RegistSubject(Subject* sbj);

	void RegistSubjects();
	void UnRegistSubjects();

	void Notify(int sj_id, void* ud);

	void EnableObserve(bool enable);

protected:
	virtual void OnNotify(int sj_id, void* ud) = 0;

private:
	std::vector<Subject*> m_subjects;

	bool m_observe_enable;

}; // Observer

}

#endif // _DRAG2D_OBSERVER_H_