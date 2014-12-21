#include "bluesmirfhid.hpp"

#ifdef DEBUGGING_BT
Serial pc(SERIAL_TX, SERIAL_RX);
#endif

Serial * bluesmirfHID;

Message btMessageBuffer;
unsigned int btMessageBufferDisplacement = 0;

bool pendingBtMessage;

void btMessageDriver()
{
    while (bluesmirfHID->readable()) {
        if (pendingBtMessage)
            return;
        
        char c = btMessageBuffer.content[btMessageBufferDisplacement] = bluesmirfHID->getc();
        btMessageBufferDisplacement++;
    
        if (c == '\n') {
            btMessageBuffer.content[btMessageBufferDisplacement] = '\0';
    
#ifdef DEBUGGING_BT
            pc.printf("%s\n", btMessageBuffer.content);
#endif
    
            btMessageBufferDisplacement = 0;
            pendingBtMessage = true;
        }
    }
}