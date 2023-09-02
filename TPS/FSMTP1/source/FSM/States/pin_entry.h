#ifndef PIN_ENTRY_H_
#define PIN_ENTRY_H_

void pin_accept_number(void);
void pin_down_number(void);
void pin_up_number(void);

void msg_pin_short(void);
void msg_fail_pin(void);
void msg_ok_pin(void);

void init_pin(void);

#endif /* PIN_ENTRY_H_ */

