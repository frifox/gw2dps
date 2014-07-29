#include "stdafx.h"
#include "gw2dps.h"

void threadDps() {
	int pAgentId = 0;
	int pHealth = 0;
	double pollingRate = dpsPollingRate; // 250 ms

	timer::cpu_timer timer;
	while (true)
	{
		if (logDps && locked.valid && locked.id == pAgentId) {
			if (timer.is_stopped())
			{
				timer.start();
				bufferDps.push_front(0);
				pHealth = locked.cHealth;
				continue;
			}

			timer::cpu_times elapsed = timer.elapsed();
			double elapsedMs = elapsed.wall / 1e6;
			if (elapsedMs > pollingRate)
			{
				timer.start();
				int cHealth = locked.cHealth;

				if (pHealth == 0)
					pHealth = cHealth;

				int dmg = pHealth - cHealth;
				pHealth = cHealth;

				if (!dpsAllowNegative && dmg < 0)
					dmg = 0;

				bufferDps.push_front(dmg);
			}
		}
		else
		{
			if (!timer.is_stopped())
			{
				timer.stop();
				pHealth = 0;
			}

			if (!bufferDps.empty())
				bufferDps.clear();

			if (locked.valid && locked.id != pAgentId)
				pAgentId = locked.id;

			if (!logDps)
				Sleep(100); // Thread not needed, sleep
		}

		// go easy on the cpu
		if (loopLimiter)
			Sleep(1);
	}
}
