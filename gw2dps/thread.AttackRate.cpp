void threadAttackRate() {
	int pAgentId = 0;
	float pHealth = 0;
	bool logFileInit = false;
	bool logFileEmpty = false;

	timer::cpu_timer timer;
	timer.stop();

	int hits = 0;
	while (true)
	{
		this_thread::interruption_point();

		if (!logAttackRateToFile && logFileInit)
			logFileInit = false;

		if (logAttackRate && locked.valid && locked.id == pAgentId)
		{
			if (timer.is_stopped())
				timer.start();

			float cHealth = locked.cHealth;
			float mHealth = locked.mHealth;
			if (pHealth == 0)
				pHealth = cHealth;

			if (cHealth == pHealth)
				continue;
			else
			{
				pHealth = cHealth;
				if (cHealth > pHealth)
					continue;

				hits++;
				if (hits == 1)
					timer.start();

				if (hits > AttackRateChainHits)
				{
					timer.stop();
					timer::cpu_times elapsed = timer.elapsed();
					double sample = elapsed.wall / 1e9;
					bufferAttackRate.push_front(sample);

					timer.start();
					hits = 1;

					threadAttackRateCounter++;

					// LOG TO FILE start //
					if (logAttackRateToFile)
					{
						ofstream file;
						if (!logFileInit){
							file.open(logAttackRateFile);
							file.close();
							logFileInit = true;
							logFileEmpty = true;
						}

						file.open(logAttackRateFile, ios::app);
						if (file.is_open())
						{
							if (logFileEmpty)
							{
								file << sample; logFileEmpty = false;
							}
							else
							{
								file << endl << sample;
							}


							//if (file.bad())
								//DbgOut("Failed writing log file (attackRate)\n");
						}
						//else DbgOut("Failed opening log file (attackRate)\n");
						file.close();
					}
				}
			}
		}
		else
		{
			if (!timer.is_stopped())
			{
				timer.stop();
				pHealth = 0;
				hits = 0;
			}

			if (!bufferAttackRate.empty())
				bufferAttackRate.clear();

			if (threadAttackRateCounter>0)
				threadAttackRateCounter = 0;

			if (locked.valid && locked.id != pAgentId)
				pAgentId = locked.id;

			if (logFileInit)
				logFileInit = false;

			if (!logAttackRate)
				Sleep(100); // Thread not needed, sleep
		}
		
		// go easy on the cpu
		if (loopLimiter)
			Sleep(1);
	}
}
