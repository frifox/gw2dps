#include "globals.h"

void threadHits() {
    int pAgentId = 0;
    float pHealth = 0;
    bool logFileInit = false;
    bool logFileEmpty = false;

    int dmg = 0;
    timer::cpu_timer timer;
    timer.stop();

    while (true)
    {
        this_thread::interruption_point();

        if (!logHitsToFile && logFileInit)
            logFileInit = false;

        if (logHits && locked.valid)
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
                        bufferHits.push_front(hit);
                        threadHitsCounter++;

                        // LOG TO FILE start //
                        if (logHitsToFile)
                        {
                            ofstream file;

                            file.precision(6);
                            file << fixed;

                            if (!logFileInit){
                                file.open(logHitsFile);
                                file.close();
                                logFileInit = true;
                                logFileEmpty = true;
                            }

                            file.open(logHitsFile, ios::app);
                            if (file.is_open())
                            {
                                // log as "time \t hit"
                                bool logDamageOverTime = false;

                                if (logFileEmpty)
                                {
                                    if (logDamageOverTime) {
                                        timer.start();
                                        dmg += hit;
                                        file << "0.000000" << "\t" << dmg;
                                        logFileEmpty = false;
                                    }
                                    else
                                    {
                                        file << hit; logFileEmpty = false;
                                    }
                                }
                                else
                                {
                                    if (logDamageOverTime) {
                                        timer::cpu_times elapsed = timer.elapsed();
                                        double elapsedTime = elapsed.wall / 1e9;
                                        dmg += hit;

                                        file << endl << elapsedTime << "\t" << dmg;
                                    }
                                    else
                                    {
                                        file << endl << hit;
                                    }

                                }

                                //if (file.bad())
                                    //DbgOut("Failed writing log file (hits)\n");
                            }
                            //else DbgOut("Failed opening log file (hits)\n");
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

                    dmg = 0;
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

                dmg = 0;
            }

            if (logFileInit)
                logFileInit = false;

            if (!logHits)
                this_thread::sleep_for(chrono::milliseconds(100)); // Thread not needed, sleep
        }

        // go easy on the cpu
        if (loopLimiter)
            this_thread::sleep_for(chrono::milliseconds(1));
    }
}
