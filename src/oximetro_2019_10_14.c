//#include <stdio.h>
//
//#define N	5
//
////FIR filter coeficients
//#define FIR0 ((float)0.001456221097893)
//#define FIR1 ((float)0.002411708629448)
//#define FIR2 ((float)0.002910975063111)
//#define FIR3 ((float)0.002411708629448)
//#define FIR4 ((float)0.001456221097893)
//
//const float h[N] = {
//		0.001456221097893,
//		0.002411708629448,
//		0.002910975063111,
//		0.002411708629448,
//		0.001456221097893,
//};
//
////buffer for raw signal
//#define N_RAW ((uint8_t)5)
//volatile uint16_t raw[N_RAW] = {0};
//
////buffer for smooth signal
//#define N_SMOOTH	((uint8_t)50)
//volatile uint16_t smooth[N_SMOOTH] = {0};
//
////buffer for signal's derivative
//#define N_GRADIENT	((uint8_t)3)
//volatile uint16_t gradient_buff[N_GRADIENT] = {0};
//
//int get_data(void);
//float filter(void);
//float gradient(void);
//void shiftBuffer(uint16_t *buffer, uint8_t length);
//
//int main(void){
//	//loop
//	while(1){
//		//get raw signal
//		shiftBuffer(raw, N_RAW);
//		raw[0]=get_data();
//
//		//filter raw signal
//		shiftBuffer(smooth, N_SMOOTH);
//		smooth[0]=filter();
//
//		//obtain signal's derivative
//		shiftBuffer(gradient_buff, N_GRADIENT);
//		gradient_buff[0]=gradient();//remember the derivative is shifted by 1 sample
//
//		//check for derivative peak(rising edge)
//		flag_peak=1;
//		for(i=1;i<(N_GRADIENT);i++){
//			if(gradient_buff[i]>gradient_buff[0]){
//				flag_peak=0;
//				break;
//			}
//		}
//
//		if(flag_peak){
//			flag_peak=0;
//			//start search for next relative maximum (on new data)
//			max=smooth[1];
//			flag_search_max=1;
//			counter_max=0;
//			//search for previous relative minimum (on old data)
//			min=smooth[1];
//			for(i=2;i<(MIN_WINDOW+2);i++){
//					if(smooth[i]<min){
//						min=smooth[i];
//						//break;
//					}
//				}
//			}
//
//			if(flag_search_max){
//				if(counter_max>=(MAX_WINDOW-1)){
//					flag_search_max=0;
//					flag_ready=1;
//				}
//				if(smooth[0]>max){
//					max=smooth[0];
//				}//else{flag_search_max=0;flag_ready=1;}
//			}
//
//		}
//
//	}
//	return 0;
//}
//
//void shiftBuffer(uint16_t *buffer, uint8_t length){
//	uint8_t i;
//	for(i = 1; i < length; i++){
//		buffer[i] = buffer[i-1];
//	}
//}
//
////FIR Lowpass filter
//float filter(void){
//	return (raw[0] * FIR0) + (raw[1] * FIR1) + (raw[2] * FIR2) + (raw[3] * FIR3) + (raw[4] * FIR4);
//}
//
//float filter (uint16_t* x, float* h, uint8_t length)
//{
//	uint8_t n = 0;
//	float y = 0;
//
//	for(n = 0; n < length; n++)
//		y += (float)x[n] * h[n];
//
//	return y;
//}
//
////gradient function
//float gradient(void){
//	return (smooth[0] - smooth[3])/* / 2 */;	/*TODO ¿No seria dividido 3? */
//}

