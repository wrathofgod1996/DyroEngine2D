#ifndef _SYSTEMMANAGER_H
#define _SYSTEMMANAGER_H

#include "Helpers/Manager.h"

#ifndef _VECTOR_
	#include <vector>
#endif

#ifndef _SYSTEM_H
	#include "Core\System\System.h"
#endif
#ifndef _SYSTEMTYPE_H
	#include "Defines/Types/SystemType.h"
#endif

class SystemFactory;

class SystemManager : public Manager<System>
{
public:
	SystemManager();
	virtual ~SystemManager();

	virtual bool initialize();
	virtual bool shutdown();

	System* getSystem(SystemType type);
	void getSystems(std::vector<System*>& systems);

private:
	SystemFactory* factory;
};

#endif //_SYSTEMMANAGER
