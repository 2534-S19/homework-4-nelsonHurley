#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "homework4.h"

int main(void)
{
    int i =0;
    char rChar;
    char *response = "\n\n\r2534 is the best course in the curriculum!\r\n\n";

    // TODO: Declare the variables that main uses to interact with your state machine.
    bool finished;

    // Stops the Watchdog timer.
    initBoard();
    // TODO: Declare a UART config struct as defined in uart.h.
    //       To begin, configure the UART for 9600 baud, 8-bit payload (LSB first), no parity, 1 stop bit.
    eUSCI_UART_ConfigV1 uartConfig =
    {
             EUSCI_A_UART_CLOCKSOURCE_SMCLK,               // SMCLK Clock Source = 3MHz
             19,                                           // UCBR = 19
             8,                                            // UCBRF = 8
             0x55,                                         // UCBRS = 0x55
             EUSCI_A_UART_NO_PARITY,                       // No Parity
             EUSCI_A_UART_LSB_FIRST,                       // LSB First
             EUSCI_A_UART_ONE_STOP_BIT,                    // One stop bit
             EUSCI_A_UART_MODE,                            // UART mode
             EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION,// Oversampling
             EUSCI_A_UART_8_BIT_LEN                        // payload = 8
        };

    // TODO: Make sure Tx AND Rx pins of EUSCI_A0 work for UART and not as regular GPIO pins.

    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1, GPIO_PIN2, GPIO_PRIMARY_MODULE_FUNCTION);
    GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_P1, GPIO_PIN3, GPIO_PRIMARY_MODULE_FUNCTION);


    // TODO: Initialize EUSCI_A0
    UART_initModule(EUSCI_A0_BASE, &uartConfig);

    // TODO: Enable EUSCI_A0
    UART_enableModule(EUSCI_A0_BASE);

    while(1)
    {
        // TODO: Check the receive interrupt flag to see if a received character is available.
        //       Return 0xFF if no character is available.
        rChar = UARTGetChar(EUSCI_A0_BASE);


        // TODO: If an actual character was received, echo the character to the terminal AND use it to update the FSM.
        //       Check the transmit interrupt flag prior to transmitting the character.
        if(rChar!=0xff)
        {

            if(UARTCanSend(EUSCI_A0_BASE))
            {
                UART_transmitData(EUSCI_A0_BASE, rChar);
                finished = charFSM(rChar);
            }
        }


        // TODO: If the FSM indicates a successful string entry, transmit the response string.
        //       Check the transmit interrupt flag prior to transmitting each character and moving on to the next one.
        //       Make sure to reset the success variable after transmission.
        if(finished)
        {
            for(i=0;response[i]!='\0';i++)
            {
                if(UARTCanSend(EUSCI_A0_BASE))
                {
                    UART_transmitData(EUSCI_A0_BASE, response[i]);
                }
            }
            finished = false;
        }


    }
}

void initBoard()
{
    WDT_A_hold(WDT_A_BASE);
}
//gets the character if its ready to recieve
uint8_t UARTGetChar(uint32_t moduleInstance)
{
    uint8_t check = 0xFF;
    static int i =0;
    if(UART_getInterruptStatus(moduleInstance, EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG) == 1)
    {
        check = UART_receiveData(moduleInstance);
        i++;
    }
    return check;
}
//wait till its ready to transmit
bool UARTCanSend(uint32_t moduleInstance)
{
    bool check = false;
    while(UART_getInterruptStatus(moduleInstance,EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG)==0)
    {

    }
    check = true;
    return check;
}
// TODO: FSM for detecting character sequence.
bool charFSM(char rChar)
{
    bool finished = false;

    static passcode_state_t currentState = SX;


    switch (currentState) {
        case SX:
            if (rChar == '2')
                currentState = S2;
            break;

        case S2:
             if (rChar == '5')
                currentState = S25;
             else
                currentState = SX;
             break;

        case S25:
             if (rChar == '3')
                currentState = S253;
             else if(rChar == '2')
                 currentState = S2;
             else
                currentState = SX;
             break;

        case S253:
             if (rChar == '4')
             {
                currentState = SX;
                finished = true;
             }
             else if(rChar == '2')
                 currentState = S2;
             else
                 currentState = SX;
             break;

        default:
                currentState = SX;
             break;
        }


    return finished;
}
