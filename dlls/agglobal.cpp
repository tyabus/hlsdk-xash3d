//++ BulliT

#include "extdll.h"
#include "util.h"
#include "cbase.h"
#include "player.h"
#include "agglobal.h"

// TITLES FOR HALF-LIFE
// Position command $position x y
// x & y are from 0 to 1 to be screen resolution independent
// -1 means center in each dimension
// Effect command $effect <effect number>
// effect 0 is fade in/fade out
// effect 1 is flickery credits
// effect 2 is write out (training room)
// Text color r g b command $color
// fadein time fadeout time / hold time
// $fadein (message fade in time - per character in effect 2)
// $fadeout (message fade out time)
// $holdtime (stay on the screen for this long)
void AgSay(CBasePlayer* pPlayer, const AgString& sText, float* pfFloodProtected, float fHoldTime, float x, float y, int iChannel)
{

  hudtextparms_t hText;
  memset(&hText, 0, sizeof(hText));
  hText.channel = iChannel;
  // These X and Y coordinates are just above the health meter.
  hText.x = x;
  hText.y = y;

  hText.r1 = hText.g1 = hText.b1 = 180;
  hText.a1 = 0;

  hText.r2 = hText.g2 = hText.b2 = 0;
  hText.a2 = 0;

  hText.holdTime = fHoldTime - 0.30;

  hText.fadeinTime = 0.000;
  hText.fadeoutTime = 0.000;
  hText.fxTime = 0.25;

  if (pPlayer)
  {
    UTIL_HudMessage(pPlayer,hText, sText);
  }
  else
  {
    for (int i = 1; i <= gpGlobals->maxClients; i++)
    {
      CBaseEntity* pPlayerLoop = UTIL_PlayerByIndex(i);
      if (pPlayerLoop)
        UTIL_HudMessage(pPlayerLoop,hText, sText);
    }
  }
}
