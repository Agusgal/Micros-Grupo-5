#ifndef PIN_ENTRY_H_
#define PIN_ENTRY_H_

#include <os.h>

void pin_accept_number(void);
void pin_down_number(void);
void pin_up_number(void);

void msg_pin_short(void);
void msg_fail_pin(void);
void msg_ok_pin(void);

void msg_pin_3_times(void);

void msg_fail_encoder(void);

void msg_ok_encoder(void);

void init_pin(void);

void update_floor_information();

void assignQueuePointer(OS_Q* msgq);


#endif /* PIN_ENTRY_H_ */

