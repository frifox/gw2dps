void threadCrits() {
    int pAgentId = 0;
    float pHealth = 0;
    bool logFileInit = false;
    bool logFileEmpty = false;

    while (true)
    {
        this_thread::interruption_point();

        if (!logCritsToFile && logFileInit)
            logFileInit = false;

        if (logCrits && locked.valid)
        {
            if (locked.id == pAgentId)
            {
                float cHealth = locked.cHealth;

                if (cHealth == pHealth)
                    continue;
                else
                {
                    if (cHealth < pHealth && cHealth != 0)
                    {
                        int hit = int(pHealth - cHealth);

                        int limitLow = int(floor(logCritsSample * (900.f / 1100.f)));
                        int limitHigh = int(ceil(logCritsSample * (1100.f / 900.f)));

                        if (hit < limitLow)
                            logCritsGlances++;
                        else if (limitHigh < hit)
                            logCritsCrits++;
                        else
                            logCritsNormals++;

                    }
                    pHealth = cHealth;
                }
            }
            else
            {
                pAgentId = locked.id;
                pHealth = locked.cHealth;

                logCritsGlances = 0;
                logCritsNormals = 0;
                logCritsCrits = 0;
            }
        }
        else
        {
            pHealth = 0;
            pAgentId = 0;

            logCritsGlances = 0;
            logCritsNormals = 0;
            logCritsCrits = 0;

            if (logFileInit)
                logFileInit = false;

            if (!logCrits)
                Sleep(100); // Thread not needed, sleep
        }

        // go easy on the cpu
        if (loopLimiter)
            Sleep(1);
    }
}
