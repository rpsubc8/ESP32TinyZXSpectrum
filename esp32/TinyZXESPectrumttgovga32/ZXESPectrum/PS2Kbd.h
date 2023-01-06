#ifndef _TECLADO_PS2KBD_H
 #define _TECLADO_PS2KBD_H

#include "gbConfig.h"
#include "hardware.h"
#include "keys.h"
#include <Arduino.h>

#define KB_INT_START attachInterrupt(digitalPinToInterrupt(KEYBOARD_CLK), kb_interruptHandler, FALLING)
#define KB_INT_STOP detachInterrupt(digitalPinToInterrupt(KEYBOARD_CLK))

extern byte lastcode;

void IRAM_ATTR kb_interruptHandler(void);
void kb_begin();
boolean isKeymapChanged();
boolean checkAndCleanKey(uint8_t scancode);
boolean checkKey(uint8_t scancode);

// inject key from wiimote, for not modifying OSD code
void emulateKeyChange(uint8_t scancode, uint8_t isdown);

#endif

