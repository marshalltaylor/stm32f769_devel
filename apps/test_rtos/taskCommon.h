#ifndef OSTASKS_H
#define OSTASKS_H

#ifdef __cplusplus
extern "C" {
#endif

#define TEST_EVENT_0	( 1 << 0 )
#define TEST_EVENT_1	( 1 << 1 )

extern EventGroupHandle_t xTestEventGroup;

void taskConsoleStart(void * argument);
void taskTestStart(void * argument);

	
#ifdef __cplusplus
}
#endif

#endif
