/*
 * homework4.h
 *
 *  Created on: 4/2/2020
 *      Author: Nelson Hurley
 */

#ifndef HOMEWORK4_H_
#define HOMEWORK4_H_

// This function initializes the board by turning off the Watchdog Timer.
void initBoard();
bool charFSM(char rChar);
typedef enum {SX, S2, S25, S253, S2534} passcode_state_t;
extern uint8_t UARTGetChar(uint32_t moduleInstance);
bool UARTCanSend(uint32_t moduleInstance);
// TODO: Define any constants that are local to homework.c using #define


#endif /* HOMEWORK4_H_ */
