//++ BulliT

#if !defined(_AG_GLOBAL_H_)
#define _AG_GLOBAL_H_

#pragma warning(disable:4786)
#pragma warning(disable:4530)

typedef string_t AgString;

void AgSay(CBasePlayer* pPlayer, const AgString& sText, float* pfFloodProtected = NULL, float fHoldTime = 3.5, float x = -1, float y = -1, int iChannel = 5);

#endif // !defined(_AG_GLOBAL_H_)

//-- Martin Webrant
