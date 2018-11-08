#ifndef MAIN_H
#define	MAIN_H

enum {R, A, V, D, I};

unsigned long millis(void);
void setMotores(signed char izq, signed char der);
void initIO(void);
void initPWM(void);
void initRest(void);
bit btn(char n);
void led(char n, char v);
bit ir(char n);
bit cny(void);

#endif
