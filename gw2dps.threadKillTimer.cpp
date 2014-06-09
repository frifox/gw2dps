#include "stdafx.h"
#include "gw2dps.h"

void threadKillTimer()
{
	int pAgentId = 0;
	int pHealth = 0;
	double pollingRate = 250; // ms

	timer::cpu_timer timer;

	while (true)
	{
		if (logKillTimer && locked.valid && pAgentId == locked.id && locked.alive)
		{
			if (pHealth == 0)
				pHealth = locked.cHealth;

			if (locked.cHealth == locked.mHealth && bufferKillTimer.dmg == 0)
			{
				Sleep(1); // no idea why but it only works with this being here
				continue;
			}

			if (timer.is_stopped())
			{
				timer.start();
				continue;
			}

			timer::cpu_times elapsed = timer.elapsed();
			double elapsedTime = elapsed.wall / 1e6;
			double elapsedSample = elapsedTime - ((bufferKillTimer.samplesKnown + bufferKillTimer.samplesUnknown) * pollingRate);

			if (elapsedSample > pollingRate)
			{
				int cHealth = locked.cHealth;
				int dmg = pHealth - cHealth;
				
				if (dmg >= 0)
				{
					if (cHealth != 0)
					{
						bufferKillTimer.dmg += dmg;
						bufferKillTimer.samplesKnown += 1;
					}
					else
					{
						if (locked.mHealth != 78870) // stop timer if not indestructible golem
							timer.stop();
					}
				}
				else
				{
					if (locked.mHealth == 78870)
					{
						bufferKillTimer.samplesUnknown += 1;
					}
					else if (dpsAllowNegative)
					{
						bufferKillTimer.dmg += dmg;
						DbgOut("One\n");
					}
					else
					{
						bufferKillTimer.dmg = pHealth;
					}
				}

				double dpKs; // damage per known sample
				dpKs = double(bufferKillTimer.dmg) / double(bufferKillTimer.samplesKnown);

				double dfUs; // damage from unknown sample
				dfUs = dpKs * double(bufferKillTimer.samplesUnknown);
				
				double dps; // total damage per second
				dps = (double(bufferKillTimer.dmg) + dfUs) / double(bufferKillTimer.samplesKnown + bufferKillTimer.samplesUnknown);
				dps = dps * (1000 / pollingRate);

				bufferKillTimer.dps = dpKs*4;

				pHealth = cHealth;
			}

			bufferKillTimer.time = elapsedTime / 1000;
		}
		else
		{
			if (pHealth != 0)
				pHealth = 0;

			if (!timer.is_stopped())
			{
				timer.stop();
			}

			if (logKillTimer && locked.valid && locked.id != pAgentId)
			{
				pAgentId = locked.id;
				bufferKillTimer = {};
			}
			else if (!logKillTimer)
			{
				pAgentId = locked.id;
				bufferKillTimer = {};
			}
			
			Sleep(100);
		}
	}
}