#include <stdint.h>

#define _hz_200 *(volatile long *)0x4ba

int32_t timerGetPrec()
 {
     return  _hz_200 * 5;
 }


// uint32_t getMicroseconds( )
// {
//     uint64_t timer200hz; 
//     uint32_t data; 
// resync:
	
//     timer200hz = ((volatile uint32_t)0x4BA);
	
//     data = ((volatile uint8_t)0xFFFFFA23);
	
// 	if ( ((volatile uint32_t)0x4BA) != timer200hz )
// 	{
// 		goto resync;
// 	}
	
// 	timer200hz*=5000;				// convert to microseconds
// 	timer200hz+=(uint64_t)(((192-data)*6666)>>8); //26;			// convert data to microseconds
// 	return timer200hz/10;
// }