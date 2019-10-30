//#include <stdio.h>
//
////FIR filter coeficients
//#define FIR0 ((float)0.001456221097893)
//#define FIR1 ((float)0.002411708629448)
//#define FIR2 ((float)0.002910975063111)
//#define FIR3 ((float)0.002411708629448)
//#define FIR4 ((float)0.001456221097893)
//
//
////buffer for raw signal
//#define N_RAW ((int)5)
//int raw[N_RAW];
//
////buffer for smooth signal
//#define N_SMOOTH((int)50)
//int smooth[N_SMOOTH];
//
////buffer for signal's derivative
//#define N_GRADIENT((int)3)
//int gradient[N_GRADIENT];
//
//void shift_raw(void);
//int get_data(void);
//void shift_smooth(void);
//float filter(void);
//float gradient();
//
//int main(void){
//	//setup
//	unsigned int i=0;
//	for(i=0;i<N_RAW;i++){
//		raw[i]=0;
//	}
//	for(i=0;i<N_SMOOTH;i++){
//		smooth[i]=0;
//	}
//	for(i=0;i<N_GRADIENT;i++){
//		gradient[i]=0;
//	}
//
//	//loop
//	while(1){
//		//get raw signal
//		shift_raw();
//		raw[0]=get_data();
//
//		//filter raw signal
//		shift_smooth();
//		smooth[0]=filter();
//
//		//obtain signal's derivative
//		shift_gradient();
//		gradient[0]=gradient();//remember the derivative is shifted by 1 sample
//
//		//check for derivative peak(rising edge)
//		flag_peak=1;
//		for(i=1;i<(N_GRADIENT);i++){
//			if(gradient[i]>gradient[0]){
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
////shifts raw signal to make space for new data
//void shift_raw(void){
//	unsigned int i=0;
//	for(i=1;i<N_RAW;i++){
//		raw[i]=raw[i-1];
//	}
//}
//
////shifts smooth signal to make space for new data
//void shift_smooth(void){
//	unsigned int i=0;
//	for(i=1;i<N_SMOOTH;i++){
//		smooth[i]=smooth[i-1];
//	}
//}
//
////shifts signal's derivative to make space for new data
//void shift_gradient(void){
//	unsigned int i=0;
//	for(i=1;i<N_GRADIENT;i++){
//		gradient[i]=gradient[i-1];
//	}
//}
//
////FIR Lowpass filter
//float filter(void){
//	return raw[0]*FIR0+raw[1]*FIR1+raw[2]*FIR2+raw[3]*FIR3+raw[4]*FIR4;
//}
//
////gradient function
//float gradient(void){
//	return (smooth[0]-smooth[3]);// /2;
//}
//
