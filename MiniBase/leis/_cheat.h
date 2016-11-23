#pragma once

#include "../main.h"

class cCheat
{
public:

	int iBhopCount;
	int iPacketCounter;

	bool bFreeLook;
	bool bFastRun;

	cCheat()
	{
		iBhopCount = 0;
		iPacketCounter = 0;
		bFreeLook = false;
		bFastRun = false;
	}

	void KnifeBotFunction( usercmd_s *cmd );
	void FastRunFunction( usercmd_s *cmd );
	
	void BhopFunction( usercmd_s *cmd );
	void AimBotFunction( usercmd_s *cmd );
	void FreeLookFunction( float frametime , usercmd_s *cmd );
	void GranadeTrailFunction( usercmd_s *cmd );
};

extern cCheat g_Cheat;