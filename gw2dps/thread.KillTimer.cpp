void threadKillTimer()
{
	int pAgentId = 0;
	int pHealth = 0;
	double pollingRate = 250; // ms

	timer::cpu_timer timer;

	while (true)
	{
		this_thread::interruption_point();

		if (logKillTimer && locked.valid && pAgentId == locked.id && locked.alive)
		{
			if (pHealth == 0)
				pHealth = locked.cHealth;

			if (locked.cHealth == locked.mHealth)
			{
				// 100% hp, reset
				pAgentId = locked.id;
				pHealth = locked.cHealth;
				bufferKillTimer = {};
				
				if(!timer.is_stopped())
					timer.stop();

				continue;
			}

			if (locked.cHealth == locked.mHealth && bufferKillTimer.dmg == 0)
			{
				Sleep(1); // no idea why but it only works with this being here
				continue;
			}

			if (timer.is_stopped())
			{
				bufferKillTimer = {};
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
						bufferKillTimer.samplesKnown += 1;
					}
					else
					{
						//bufferKillTimer.dmg += pHealth;
						bufferKillTimer.dmg += 0;
						bufferKillTimer.samplesKnown += 1;
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
			if (pHealth != 0) {
				
				{ // check if char is dead, if dead then tweak dps for accuracy
					bool isAlive = false;
					Agent ag;
					while (ag.BeNext() && !this_thread::interruption_requested())
					{
						if (pAgentId == ag.GetAgentId())
						{
							Character ch = ag.GetCharacter();
							if (ch.IsValid())
							{
								if (ch.IsAlive() && ch.GetCurrentHealth() > 0) {
									//DbgOut("Char valid; alive");
									isAlive = true;
								}
								else
								{
									//DbgOut("Char valid; dead");
								}
							}
							else
							{
								//DbgOut("Agent valid; unknown");
								isAlive = true;
							}
						}
					}

					
					// add the last polling sample to buffer
					if (!isAlive && bufferKillTimer.time > 0) {
						bufferKillTimer.dmg += pHealth;
						bufferKillTimer.dps = bufferKillTimer.dmg / bufferKillTimer.time;
					}
				}

				// reset
				pHealth = 0;
			}
				

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

		// go easy on the cpu
		if (loopLimiter)
			Sleep(1);
	}
}