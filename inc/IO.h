#ifndef IO_H
#define IO_H

bool button1;
bool button2;

void IO_Init(void);

void PortF_Init(void);

void toggle_red(void);
void toggle_green(void);
void toggle_blue(void);

#endif
