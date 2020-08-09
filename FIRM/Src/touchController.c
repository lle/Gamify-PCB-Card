#include "touchController.h"

#define TSC_THRESHOLD 0.80f

#define TOUCH_GROUP_ID1 			1 								//"2" = TSC_GROUP1
#define TOUCH_SAMPLE_CAP_G1 	TSC_GROUP1_IO1 		//change this value and it will be applied across all this code page
#define TOUCH_CAP_1						TSC_GROUP1_IO2
#define TOUCH_CAP_2						TSC_GROUP1_IO3
#define TOUCH_CAP_3						TSC_GROUP1_IO4

#define TOUCH_GROUP_ID2 			2 								//"1" = TSC_GROUP2
#define TOUCH_SAMPLE_CAP_G2 	TSC_GROUP2_IO1 		//change this value and it will be applied across all this code page
#define TOUCH_CAP_4						TSC_GROUP2_IO2
#define TOUCH_CAP_5						TSC_GROUP2_IO3
#define TOUCH_CAP_6						TSC_GROUP2_IO4

#define NUM_OF_TSC 6

#define CAP_1 0	//set some ID for clarity of reading, make it human-readable
#define CAP_2 1 
#define CAP_3 2 
#define CAP_4 3 
#define CAP_5 4 
#define CAP_6 5 

uint32_t CapSenseCounter[NUM_OF_TSC] 	= 	{0,0,0,0 , 0,0};

uint32_t CapSenseThreshold[NUM_OF_TSC] = 	{0,0,0,0 , 0,0};

uint8_t CapSense_Selector = 0;	//set starting point to CAP1

TSC_IOConfigTypeDef touchSenseConfig;	



/***********************************[NEW FUNCTION]************************************/
//	*		Section Name:	Private Function
//	*		Description:	Hidden function for the operation running in the background  
//	*									
/************************************************************************************/
void CapSense_CalibrationStart(TSC_HandleTypeDef* htsc)
{
	touchSenseConfig.SamplingIOs = TOUCH_SAMPLE_CAP_G1; 	//sense cap is on group			
	touchSenseConfig.ChannelIOs = TOUCH_CAP_1; 				//electrode IO					
	HAL_TSC_IOConfig(htsc, &touchSenseConfig);						//Apply config
	HAL_TSC_Start_IT(htsc); //Start reading TSC, when the reading is completed, "HAL_TSC_ConvCpltCallback()" will be triggered, look there for the next steps
	CapSense_Selector = CAP_1;
}

void HAL_TSC_ConvCpltCallback(TSC_HandleTypeDef* htsc)
{	
	//Read TSC value after a conversion is completed
	if(CapSense_Selector == CAP_1)
	{
		//************************ TSC Read value from previous captouch: CAP_1
		CapSenseCounter[0] = HAL_TSC_GroupGetValue(htsc, TOUCH_GROUP_ID1 - 1);	
		
		if(CapSenseThreshold[0] == 0)
			CapSenseThreshold[0] = (uint32_t)( (float)CapSenseCounter[0] * TSC_THRESHOLD ) ; //if the threshold has not been set yet, set it now 
		
		//************************ Setup for next read CAP_2 **********************************
		touchSenseConfig.SamplingIOs = TOUCH_SAMPLE_CAP_G1; 	//sense cap is on group		
		touchSenseConfig.ChannelIOs = TOUCH_CAP_2; 						//electrode IO							
		HAL_TSC_IOConfig(htsc, &touchSenseConfig);						//Apply config
		CapSense_Selector = CAP_2; 	//This will select select CAP_2 as the next reading
		HAL_TSC_Start_IT(htsc); 		//Start conversion, come back at next "ConvCpltCallback()"
	}
	
	
	
	else if(CapSense_Selector == CAP_2)
	{
		//TSC Read value from previous captouch: CAP_2
		CapSenseCounter[1] = HAL_TSC_GroupGetValue(htsc, TOUCH_GROUP_ID1 - 1);	
	
		if(CapSenseThreshold[1] == 0)
			CapSenseThreshold[1] = (uint32_t)(CapSenseCounter[1] * TSC_THRESHOLD ); //if the threshold has not been set yet, set it now
		
		//************************ Setup for next read next TSC **********************************
		touchSenseConfig.SamplingIOs = TOUCH_SAMPLE_CAP_G1; 	//sense cap is on group		
		touchSenseConfig.ChannelIOs = TOUCH_CAP_3; 						//electrode IO							
		HAL_TSC_IOConfig(htsc, &touchSenseConfig);						//Apply config
		CapSense_Selector = CAP_3; 	//This will select select CAP_2 as the next reading
		HAL_TSC_Start_IT(htsc); 		//Start conversion, come back at next "ConvCpltCallback()"
	}
	
	
	
	else if(CapSense_Selector == CAP_3)
	{
		//TSC Read value from previous captouch: CAP_3
		CapSenseCounter[2] = HAL_TSC_GroupGetValue(htsc, TOUCH_GROUP_ID1 - 1);	
		
		
		if(CapSenseThreshold[2] == 0)
			CapSenseThreshold[2] = (uint32_t)(CapSenseCounter[2] * TSC_THRESHOLD ); //if the threshold has not been set yet, set it now
		
		//************************ Setup for next read next TSC **********************************
		touchSenseConfig.SamplingIOs = TOUCH_SAMPLE_CAP_G2; 	//sense cap is on group		
		touchSenseConfig.ChannelIOs = TOUCH_CAP_4; 				//electrode IO							
		HAL_TSC_IOConfig(htsc, &touchSenseConfig);						//Apply config
		CapSense_Selector = CAP_4; 			//This will select CAP_1 as the next reading
		HAL_TSC_Start_IT(htsc); 		//Start conversion, come back at next "ConvCpltCallback()"
	}
	
	
	
	else if(CapSense_Selector == CAP_4)
	{
		//TSC Read value from previous captouch: CAP_4
		CapSenseCounter[3] = HAL_TSC_GroupGetValue(htsc, TOUCH_GROUP_ID2 - 1);	
		
		
		if(CapSenseThreshold[3] == 0)
			CapSenseThreshold[3] = (uint32_t)(CapSenseCounter[3] * TSC_THRESHOLD ); //if the threshold has not been set yet, set it now
		
		//************************ Setup for next read next TSC **********************************
		touchSenseConfig.SamplingIOs = TOUCH_SAMPLE_CAP_G2; 	//sense cap is on group		
		touchSenseConfig.ChannelIOs = TOUCH_CAP_5; 				//electrode IO							
		HAL_TSC_IOConfig(htsc, &touchSenseConfig);						//Apply config
		CapSense_Selector = CAP_5; 			//This will select CAP_1 as the next reading
		HAL_TSC_Start_IT(htsc); 		//Start conversion, come back at next "ConvCpltCallback()"
	}
	
	
	
	else if(CapSense_Selector == CAP_5)
	{
		//TSC Read value from previous captouch: CAP_5
		CapSenseCounter[4] = HAL_TSC_GroupGetValue(htsc, TOUCH_GROUP_ID2 - 1);	

		if(CapSenseThreshold[4] == 0)
			CapSenseThreshold[4] = (uint32_t)(CapSenseCounter[4] * TSC_THRESHOLD ); //if the threshold has not been set yet, set it now
		
		//************************ Setup for next read next TSC **********************************
		touchSenseConfig.SamplingIOs = TOUCH_SAMPLE_CAP_G2; 	//sense cap is on group		
		touchSenseConfig.ChannelIOs = TOUCH_CAP_6; 				//electrode IO							
		HAL_TSC_IOConfig(htsc, &touchSenseConfig);						//Apply config
		CapSense_Selector = CAP_6; 			//This will select CAP_1 as the next reading
		HAL_TSC_Start_IT(htsc); 		//Start conversion, come back at next "ConvCpltCallback()"
	}
	
	
	
	else if(CapSense_Selector == CAP_6)
	{
		//TSC Read value from previous captouch: CAP_6
		CapSenseCounter[5] = HAL_TSC_GroupGetValue(htsc, TOUCH_GROUP_ID2 - 1);	
		
		
		if(CapSenseThreshold[5] == 0)
			CapSenseThreshold[5] = (uint32_t)(CapSenseCounter[5] * TSC_THRESHOLD ); //if the threshold has not been set yet, set it now
		
		//************************ Setup for next read next TSC **********************************
		touchSenseConfig.SamplingIOs = TOUCH_SAMPLE_CAP_G1; 	//sense cap is on group		
		touchSenseConfig.ChannelIOs = TOUCH_CAP_1; 				//electrode IO							
		HAL_TSC_IOConfig(htsc, &touchSenseConfig);						//Apply config
		CapSense_Selector = CAP_1; 			//This will select CAP_1 as the next reading
		HAL_TSC_Start_IT(htsc); 		//Start conversion, come back at next "ConvCpltCallback()"
	}
	
}


/***********************************[NEW FUNCTION]************************************/
//	*		Section Name:	Public Function
//	*		Description:	Functions available to initialize and fetch data from TSC  
//	*									
/************************************************************************************/
void tsc_capsense_init()
{
	HAL_TSC_IODischarge(&htsc, ENABLE);
	HAL_Delay(1);
	CapSense_CalibrationStart(&htsc);
}



//OUTPUT CAP SENSOR STATE
uint8_t tsc_get_capsense(uint8_t sensor_id)
{
	if(sensor_id < NUM_OF_TSC)
	{
		if( tsc_get_capsense_raw(sensor_id) < CapSenseThreshold[sensor_id])
		{
			return TOUCH_DETECTED;
		}
		else
		{
			return TOUCH_NONE;
		}
	}
	else
	{
		return 255; //OUT OF BOUND
	}
}



//OUTPUT RAW READING
uint32_t tsc_get_capsense_raw(uint8_t sensor_id)
{
	if(sensor_id < NUM_OF_TSC)
	{
		return CapSenseCounter[sensor_id];
	}
	else
	{
		return 99999; //OUT of BOUND
	}
}


//OUTPUT RAW THRESHOLD
uint32_t tsc_get_capsense_threshold_raw(uint8_t sensor_id)
{
	if(sensor_id < NUM_OF_TSC)
	{
		return CapSenseThreshold[sensor_id];
	}
	else
	{
		return 99999; //OUT of BOUND
	}
}
