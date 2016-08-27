#include "globals.h"

void threadDps() {
    int pAgentId = 0;
    float pHealth = 0;
    double pollingRate = 250; // ms

    timer::cpu_timer timer;
    while (true)
    {
        this_thread::interruption_point();

        if ((logDps || dpsGrapher) && locked.valid && locked.id == pAgentId) {
            if (timer.is_stopped())
            {
                timer.start();

                // reset party dps
                bufferDps.push_front(0);
                pHealth = locked.cHealth;

                // reset self dps
                bufferSelfDps.push_front(0);
                dpsGraph.Push(0);
                selfDmg = Dmg();

                continue;
            }

            timer::cpu_times elapsed = timer.elapsed();
            double elapsedMs = elapsed.wall / 1e6;
            if (elapsedMs > pollingRate)
            {
                timer.start();

                // party dps
                float cHealth = locked.cHealth;
                if (pHealth == 0)
                    pHealth = cHealth;

                float partyDps = pHealth - cHealth;
                if (!dpsAllowNegative && partyDps < 0)
                    partyDps = 0;

                pHealth = cHealth;

                // self dps
                float selfDps = selfDmg.total - selfDmg.snapshot; // probably should leave as float...
                selfDmg.snapshot = selfDmg.total;

                // push dps values to the buffer
                bufferDps.push_front(partyDps);
                bufferSelfDps.push_front(selfDps);
                dpsGraph.Push(selfDps);
            }
        }
        else
        {
            if (!timer.is_stopped())
            {
                timer.stop();
                pHealth = 0;
            }

            if (!bufferDps.empty()) {
                bufferDps.clear();
            }

            if (!bufferSelfDps.empty()) {
                bufferSelfDps.clear();
                dpsGraph.Clear();
            }

            if (locked.valid && locked.id != pAgentId)
                pAgentId = locked.id;

            if (!logDps)
                this_thread::sleep_for(chrono::milliseconds(100)); // Thread not needed, sleep
        }

        // go easy on the cpu
        if (loopLimiter)
            this_thread::sleep_for(chrono::milliseconds(25));
    }
}
