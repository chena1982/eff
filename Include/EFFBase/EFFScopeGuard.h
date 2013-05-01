/******************************************************************************
	created:	2013-5-1   16:51
	file path:	d:\EFF\Include\EFFBase\EFFScopeGuard.h
	author:		ChenA
	
	purpose:	
******************************************************************************/
#ifndef __EFFScopeGuard_2013_5_1__
#define __EFFScopeGuard_2013_5_1__

EFFBASE_BEGIN


class EFFBASE_API EFFScopeGuard
{
public:
    explicit EFFScopeGuard(std::function<effVOID ()> onExitScope_);
	~EFFScopeGuard();

	effVOID Dismiss();
private:
    std::function<effVOID ()> onExitScope;
    effBOOL dismissed;

private: // noncopyable
    EFFScopeGuard(const EFFScopeGuard &);
    EFFScopeGuard & operator = (const EFFScopeGuard &);
};


#define SCOPEGUARD_LINENAME_CAT(name, line) name##line
#define SCOPEGUARD_LINENAME(name, line) SCOPEGUARD_LINENAME_CAT(name, line)
#define ON_SCOPE_EXIT(callback) EFFScopeGuard SCOPEGUARD_LINENAME(EXIT, __LINE__)(callback);

EFFBASE_END

#endif