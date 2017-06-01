#ifndef __PRINTER_H
#define __PRINTER_H

#include "stm32f10x.h"

void Printer_Init(void);
void check_PrinterState(void);
int ReceivePrinterData(int seconds,u8 *data);
void printSampleData(float xy[256][2] , int num);
void cal_result(float xy[256][2],float result_Pin4[2] , float result_Pin3[2]);
#endif

