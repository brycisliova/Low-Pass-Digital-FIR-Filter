#include <at89c5131.h>
#include "stdio.h"	
#include "serial.h"

void int_to_string(unsigned int val,unsigned char *temp_str_data);
	
float fir_coeffs[8] = {0.00875474, 0.04794887, 0.16402439, 0.27927199, 0.27927199,
       0.16402439, 0.04794887, 0.00875474};//for 2kHz cutoff: 
//The first element(index = 0) of this array contains the coefficient of the oldest sample 
void main(void)
{
	unsigned char data_recv[5] = {0,0,0,0,'\0'};
	int sample_val;
	int sample_val_doub;
	unsigned char data_send[5] = {0,0,0,0,'\0'};
	//y = output, to be transmitted for analysis, y is a 4 digit number XXXX in mV
	float y_accurate = 0;
	int y = 0; 
  int signal[8] = {0,0,0,0,0,0,0,0}; //this array contains the oldest sample at index 0
	int i = 1;
	int j = 0;
	
	uart_init();

	while(1)
	{ 
		data_recv[0] = receive_char(); 		
		data_recv[1] = receive_char();
		data_recv[2] = receive_char(); 
		data_recv[3] = receive_char();
		data_recv[4] = '\0';
		
		sample_val = (data_recv[0] - 48)*1000  + (data_recv[1]-48)*100 + (data_recv[2]-48)*10 + (data_recv[3]-48);//4 digit mV
		
		  while(i <= 7){
   		  signal[i-1] = signal[i]; 
			  i = i + 1;
	   	}
		  i = 1;
		  signal[7]	= sample_val;
			
			y_accurate = 0;
		  while(j <= 7){
		    y_accurate = y_accurate + fir_coeffs[j]*signal[j];
		    j = j + 1;
		  }
			j = 0;
			
			y = (unsigned int) (y_accurate);
			
       int_to_string(y, data_send);
			 transmit_string(data_send);
			 transmit_string("\r\n");
 	}
}

void int_to_string(unsigned int val,unsigned char *temp_str_data)
{	
	  temp_str_data[0]=48+(val%10000/1000);
	  temp_str_data[1]=48+((val%1000)/100);
	  temp_str_data[2]=48+((val%100)/10);
	  temp_str_data[3]=48+(val%10);
 }