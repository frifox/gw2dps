#include "stdafx.h"
#include "gw2dps.h"

void threadHits() {
	int pAgentId = 0;
	int pHealth = 0;
	bool logFileInit = false;
	bool logFileEmpty = false;

	while (true)
	{
		if (!logHitsToFile && logFileInit)
			logFileInit = false;

		if (logHits && locked.valid)
		{
			if (locked.id == pAgentId)
			{
				int cHealth = locked.cHealth;

				if (cHealth == pHealth)
					continue;
				else
				{
					if (cHealth < pHealth && cHealth != 0)
					{
						int hit = pHealth - cHealth;
						bufferHits.push_front(hit);
						threadHitsCounter++;

						// LOG TO FILE start //
						if (logHitsToFile)
						{
							ofstream file;
							if (!logFileInit){
								file.open(logHitsFile);
								file.close();
								logFileInit = true;
								logFileEmpty = true;
							}

							file.open(logHitsFile, ios::app);
							if (file.is_open())
							{
								if (logFileEmpty)
								{
									file << hit; logFileEmpty = false;
								}
								else
								{
									file << endl << hit;
								}


								if (file.bad())
									DbgOut("Failed writing log file (hits)\n");
							}
							else DbgOut("Failed opening log file (hits)\n");
							file.close();
						}

						// LOG TO FILE end //
					}
					pHealth = cHealth;
				}
			}
			else
			{
				pAgentId = locked.id;
				pHealth = locked.cHealth;

				if (!bufferHits.empty())
				{
					bufferHits.clear();
					threadHitsCounter = 0;
				}
			}
		}
		else
		{
			pHealth = 0;
			pAgentId = 0;

			if (!bufferHits.empty())
			{
				bufferHits.clear();
				threadHitsCounter = 0;
			}

			if (logFileInit)
				logFileInit = false;

			if (!logHits)
				Sleep(100); // Thread not needed, sleep
		}
	}
}
