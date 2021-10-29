#ifndef __CMD_Thread_H__
#define __CMD_Thread_H__

#define IN_CCMRAM(adr) ((void*)adr > (void*)0x10000000 && (void*)adr < (void*)0x10010000)
void Debug(char* cmd);
void Data_Adjust(char* argv);
#endif
