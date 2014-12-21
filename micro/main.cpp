#include "mbed.h"
#include "bluesmirfhid.hpp"
#include "temperature.hpp"
//#include "relay.hpp"
#include "typedefinitions.hpp"

BluesmirfHID bt(PC_10, PC_11);


// A0: RTD
// A1: thermocouple
OvenTemperature oven(A0, A1);
DigitalOut relay(D2);

DigitalOut led(LED1);



Serial pc(SERIAL_TX, SERIAL_RX);



void printMessage(Serial * s, Message * m)
{
    s->printf(m->content);
};


int main()
{
    int desiredTemperature = 0;
    //float ovenTemperature = 0.0;

    Status s = IDLE;

    Message m;
    Message m_out;

    relay = 0;
    led = 0;

    for(;;) {
        switch (s) {
            case IDLE:
                // Enables Bluetooth and waits for
                // incoming connection

                while (!bt.messagePending())
                    wait_ms(250);

                m = bt.getMessage();

                if (m.content[0] == PC_CONNECTED) {
                    s = WAIT_FOR_START;
                    led = 1;
                }
                break;
            case WAIT_FOR_START:
                while (!bt.messagePending())
                    wait_ms(250);

                m = bt.getMessage();

                if (m.content[0] == PC_START) {
                    s = DATA_EXCHANGE;
                    led = 0;
                }
                break;
            case DATA_EXCHANGE:
                // 1) Sends the current oven temperature
                // 2) receives the desired temperature
                // 3) controls the relay, accordingly

                led = !led;

                // 1)
                //ovenTemperature = oven.temperature();
                //ovenTemperature++;
                //unsigned int OT = ovenTemperature;
                unsigned int OT;
                OT = oven.temperature();

                sprintf(m_out.content, "%c,%003d", OVEN_TEMP, OT);
                bt.sendMessage(m_out);

                // 2)
                unsigned int safetyCounter = 0;
                while (!bt.messagePending()) {
                    wait_ms(250);
                    // If the PC is no more active,
                    // then lock the system
                    if (safetyCounter++ > 1) {
                        relay = 0; //relay.off();
                        for (;;) {
                            led = !led;
                            wait_ms(250);
                        }
                    }
                }

                m = bt.getMessage();

                printMessage(&pc, &m);

                // 3)
                if (m.content[0] == PC_STOP) {
                    relay = 0; //relay.off();
                    led = 0;
                    s = WAIT_FOR_START;
                    break;
                } else if (m.content[0] == PC_TEMP) {
                    
                    desiredTemperature = (m.content[2] - '0') * 100;
                    desiredTemperature += (m.content[3] - '0') * 10;
                    desiredTemperature += (m.content[4] - '0');
                    
                    printf("Desired: %d", desiredTemperature);
                    printf("ovenTemperature: %d", OT);
                    
                    if (OT < desiredTemperature)
                        relay = 1; //relay.on();
                    else
                        relay = 0; //relay.off();
                    break;
                }
        }

        wait_ms(2000);
    }
}
