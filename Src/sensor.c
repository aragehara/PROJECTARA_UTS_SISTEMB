/*
 * sensor.c
 *
 *  Created on: Nov 2, 2019
 *      Author: aragehara
 */
#include "main.h"
#include "stm32f1xx_hal.h"
#include "i2c-lcd.h"
#include "device.h"
#include "sensor.h"

unsigned int ms=0,dd=0,mm=0;
char buffer[17],buffs[13];
//char buffer1[15];
unsigned int runstop=0;

unsigned int lapA=0,lapB=0,lapC=0,totlap=5;
unsigned int ms1,dd1,mm1,mslast1=0,ddlast1=0,mmlast1=0;
unsigned int ms2,dd2,mm2,mslast2=0,ddlast2=0,mmlast2=0;
unsigned int ms3,dd3,mm3,mslast3=0,ddlast3=0,mmlast3=0;

unsigned char bouncing	=0xFF;
unsigned char bouncing2	=0xFF;
unsigned char bouncing3	=0xFF;
unsigned char bouncing4	=0xFF;
unsigned char bouncing5	=0xFF;

uint32_t Timeout_loop 	= 0;
uint32_t Timeout_value 	= 400;
uint32_t Timeout_loop2 	= 0;
uint32_t Timeout_value2 = 400;
uint32_t Timeout_loop3 	= 0;
uint32_t Timeout_value3 = 400;
uint32_t Timeout_loop4 	= 0;
uint32_t Timeout_value4 = 400;
uint32_t Timeout_loop5 	= 0;
uint32_t Timeout_value5 = 400;

void tombolrunstop(void)
{
	if(HAL_GPIO_ReadPin(Button_RunStop_GPIO_Port,Button_RunStop_Pin)== GPIO_PIN_RESET && (Timeout_loop++<=Timeout_value)){
		bouncing=(bouncing<<1);
	}

	else if(HAL_GPIO_ReadPin(Button_RunStop_GPIO_Port,Button_RunStop_Pin)== GPIO_PIN_RESET && (Timeout_loop++>Timeout_value)){
		if(Timeout_loop>=Timeout_value+50){
			Timeout_loop=Timeout_value+1;
			HAL_GPIO_TogglePin(User_LED_GPIO_Port,User_LED_Pin);
			HAL_UART_Transmit(&huart1,(uint8_t*)"\n\rButton_RunStop : Error",24,10);
		}
	}

	else if(HAL_GPIO_ReadPin(Button_RunStop_GPIO_Port,Button_RunStop_Pin)== GPIO_PIN_SET && Timeout_loop<=Timeout_value){
		bouncing = (bouncing<<1)|1;
	}
	else{
		Timeout_loop=0;
		bouncing=0xFF;
	}

	if (bouncing==0x01){
		if(runstop==0){
			HAL_TIM_Base_Start_IT(&htim2);
			runstop=1;
		}
		else{
			HAL_TIM_Base_Stop_IT(&htim2);
			runstop=0;
		}
	}
}

void tombolreset(void)
{
	if(HAL_GPIO_ReadPin(Button_Reset_GPIO_Port,Button_Reset_Pin)== GPIO_PIN_RESET && (Timeout_loop2++<=Timeout_value2)){
		bouncing2=(bouncing2<<1);

	}

	else if(HAL_GPIO_ReadPin(Button_Reset_GPIO_Port,Button_Reset_Pin)== GPIO_PIN_RESET && (Timeout_loop2++>Timeout_value2)){
		if(Timeout_loop2>=Timeout_value2+50){
			Timeout_loop2=Timeout_value2+1;
			HAL_GPIO_TogglePin(User_LED_GPIO_Port,User_LED_Pin);
			HAL_UART_Transmit(&huart1,(uint8_t*)"\n\rButton_Reset : Error",22,10);
		}
	}

	else if(HAL_GPIO_ReadPin(Button_Reset_GPIO_Port,Button_Reset_Pin)== GPIO_PIN_SET && Timeout_loop2<=Timeout_value2){
		bouncing2 = (bouncing2<<1)|1;
	}

	else{
		Timeout_loop2=0;
		bouncing2=0xFF;
	}

	if (bouncing2==0x01){

		HAL_TIM_Base_Stop_IT(&htim2);

		ms=0;	dd=0;	mm=0;	runstop=0;
		ms1=0;	dd1=0;	mm1=0;
		ms2=0;	dd2=0;	mm2=0;
		ms3=0;	dd3=0;	mm3=0;
		lapA=0;	lapB=0;	lapC=0;

	}
}

void sensor1(void)
{
	if(HAL_GPIO_ReadPin(Sensor1_GPIO_Port,Sensor1_Pin)== GPIO_PIN_RESET && (Timeout_loop3++<=Timeout_value3)){
		bouncing3=(bouncing3<<1);

	}

	else if(HAL_GPIO_ReadPin(Sensor1_GPIO_Port,Sensor1_Pin)== GPIO_PIN_RESET && (Timeout_loop3++>Timeout_value3)){
		if(Timeout_loop3>=Timeout_value3+50){
			Timeout_loop3=Timeout_value3+1;
			//HAL_GPIO_TogglePin(User_LED_GPIO_Port,User_LED_Pin);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1,GPIO_PIN_SET);
			HAL_Delay(100);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1,GPIO_PIN_RESET);
			HAL_Delay(100);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1,GPIO_PIN_SET);
			HAL_Delay(100);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1,GPIO_PIN_RESET);
			HAL_Delay(100);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1,GPIO_PIN_SET);
			HAL_Delay(100);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1,GPIO_PIN_RESET);
			HAL_Delay(100);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13,GPIO_PIN_RESET);
			HAL_UART_Transmit(&huart1,(uint8_t*)"\n\rSensor1 : Error",17,10);
		}
	}

	else if(HAL_GPIO_ReadPin(Sensor1_GPIO_Port,Sensor1_Pin)== GPIO_PIN_SET && Timeout_loop3<=Timeout_value3){
		bouncing3 = (bouncing3<<1)|1;
	}

	else{
		Timeout_loop3=0;
		bouncing3=0xFF;
	}

	if (bouncing3==0xFE){

		ms1=ms-mslast1;
		mslast1=ms;

		dd1=dd-ddlast1;
		ddlast1=dd;

		mm1=mm-mmlast1;
		mmlast1=mm;

		if(runstop==1)	lapA++;
		else 			lapA=0;

		if(lapA<5){
			sprintf(buffs,"%d = %d%d:%d%d:%d%d",lapA,(mm1/10),(mm1%10),(dd1/10),(dd1%10),(ms1/10),(ms1%10));
			HAL_UART_Transmit(&huart1,(uint8_t*)"\n\rMobil A Lap:",14,10);
			HAL_UART_Transmit(&huart1,(uint8_t*)buffs,sizeof(buffs),10);
		}
		else{
			lapA=5;
			HAL_UART_Transmit(&huart1,(uint8_t*)"\n\rMobil A = FINISH",18,10);
		}



	}
}

void sensor2(void)
{
	if(HAL_GPIO_ReadPin(Sensor2_GPIO_Port,Sensor2_Pin)== GPIO_PIN_RESET && (Timeout_loop4++<=Timeout_value4)){
		bouncing4=(bouncing4<<1);

	}

	else if(HAL_GPIO_ReadPin(Sensor2_GPIO_Port,Sensor2_Pin)== GPIO_PIN_RESET && (Timeout_loop4++>Timeout_value4)){
		if(Timeout_loop4>=Timeout_value4+50){
			Timeout_loop4=Timeout_value4+1;
			//HAL_GPIO_TogglePin(User_LED_GPIO_Port,User_LED_Pin);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1,GPIO_PIN_SET);
			HAL_Delay(100);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1,GPIO_PIN_RESET);
			HAL_Delay(100);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1,GPIO_PIN_SET);
			HAL_Delay(100);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1,GPIO_PIN_RESET);
			HAL_Delay(100);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1,GPIO_PIN_SET);
			HAL_Delay(100);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1,GPIO_PIN_RESET);
			HAL_Delay(100);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_14,GPIO_PIN_RESET);
			HAL_UART_Transmit(&huart1,(uint8_t*)"\n\rSensor2 : Error",17,10);
		}
	}

	else if(HAL_GPIO_ReadPin(Sensor2_GPIO_Port,Sensor2_Pin)== GPIO_PIN_SET && Timeout_loop4<=Timeout_value4){
		bouncing4 = (bouncing4<<1)|1;
	}

	else{
		Timeout_loop4=0;
		bouncing4=0xFF;
	}

	if (bouncing4==0xFE){

		ms2=ms-mslast2;
		mslast2=ms;

		dd2=dd-ddlast2;
		ddlast2=dd;

		mm2=mm-mmlast2;
		mmlast2=mm;

		if(runstop==1)	lapB++;
		else 			lapB=0;

		if(lapB<5){
			sprintf(buffs,"%d = %d%d:%d%d:%d%d",lapB,(mm2/10),(mm2%10),(dd2/10),(dd2%10),(ms2/10),(ms2%10));
			HAL_UART_Transmit(&huart1,(uint8_t*)"\n\rMobil B Lap:",14,10);
			HAL_UART_Transmit(&huart1,(uint8_t*)buffs,sizeof(buffs),10);
		}
		else{
			lapB=5;
			HAL_UART_Transmit(&huart1,(uint8_t*)"\n\rMobil B = FINISH",18,10);
		}
	}
}

void sensor3(void)
{
	if(HAL_GPIO_ReadPin(Sensor3_GPIO_Port,Sensor3_Pin)== GPIO_PIN_RESET && (Timeout_loop5++<=Timeout_value5)){
		bouncing5=(bouncing5<<1);

	}

	else if(HAL_GPIO_ReadPin(Sensor3_GPIO_Port,Sensor3_Pin)== GPIO_PIN_RESET && (Timeout_loop5++>Timeout_value5)){
		if(Timeout_loop5>=Timeout_value5+50){
			Timeout_loop5=Timeout_value5+1;
			//HAL_GPIO_TogglePin(User_LED_GPIO_Port,User_LED_Pin);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15,GPIO_PIN_SET);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1,GPIO_PIN_SET);
			HAL_Delay(100);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1,GPIO_PIN_RESET);
			HAL_Delay(100);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1,GPIO_PIN_SET);
			HAL_Delay(100);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1,GPIO_PIN_RESET);
			HAL_Delay(100);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1,GPIO_PIN_SET);
			HAL_Delay(100);
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1,GPIO_PIN_RESET);
			HAL_Delay(100);
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_15,GPIO_PIN_RESET);
			HAL_UART_Transmit(&huart1,(uint8_t*)"\n\rSensor3 : Error",17,10);
		}
	}

	else if(HAL_GPIO_ReadPin(Sensor3_GPIO_Port,Sensor3_Pin)== GPIO_PIN_SET && Timeout_loop5<=Timeout_value5){
		bouncing5 = (bouncing5<<1)|1;
	}

	else{
		Timeout_loop5=0;
		bouncing5=0xFF;
	}

	if (bouncing5==0xFE){

		ms3=ms-mslast3;
		mslast3=ms;

		dd3=dd-ddlast3;
		ddlast3=dd;

		mm3=mm-mmlast3;
		mmlast3=mm;

		if(runstop==1)	lapC++;
		else 			lapC=0;

		if(lapC<5){
			sprintf(buffs,"%d = %d%d:%d%d:%d%d",lapC,(mm3/10),(mm3%10),(dd3/10),(dd3%10),(ms3/10),(ms3%10));
			HAL_UART_Transmit(&huart1,(uint8_t*)"\n\rMobil C Lap:",14,10);
			HAL_UART_Transmit(&huart1,(uint8_t*)buffs,sizeof(buffs),10);
		}
		else{
			lapC=5;
			HAL_UART_Transmit(&huart1,(uint8_t*)"\n\rMobil C = FINISH",18,10);
		}


	}
}

void kirim_lcd(void)
{
	///////////////////////////////////////////////////////////////////LCD ATAS
	lcd_send_cmd(0x80);
	sprintf(buffer,"WAKTU    %d%d:%d%d:%d%d",(mm/10),(mm%10),(dd/10),(dd%10),(ms/10),(ms%10));
	lcd_send_string(buffer);

	lcd_send_cmd(0xe2);
	sprintf(buffer," ");
	lcd_send_string(buffer);

	///////////////////////////////////////////////////////////////////////Baris 2
	if(lapA<totlap){
		lcd_send_cmd(0xc0);
		sprintf(buffer,"MA%d=%d%d:%d%d:%d%d",(lapA),(mm1/10),(mm1%10),(dd1/10),(dd1%10),(ms1/10),(ms1%10));
		lcd_send_string(buffer);
	}
	else{
		lcd_send_cmd(0xc0);
		sprintf(buffer,"MA=FINISH"/*,(mm/10),(mm%10),(dd/10),(dd%10),(ms/10),(ms%10)*/);
		lcd_send_string(buffer);
	}
	///////////////////////////////////////////////////////////////////BARIS 3
	if(lapB<totlap){
		lcd_send_cmd(0x94);
		sprintf(buffer,"MB%d=%d%d:%d%d:%d%d",(lapB),(mm2/10),(mm2%10),(dd2/10),(dd2%10),(ms2/10),(ms2%10)/*(lapC),(mm3/10),(mm3%10),(dd3/10),(dd3%10)*/);
		lcd_send_string(buffer);
	}
	else{
		lcd_send_cmd(0x94);
		sprintf(buffer,"MB=FINISH");
		lcd_send_string(buffer);
	}
	if(lapC<totlap){
		lcd_send_cmd(0xd4);
		sprintf(buffer,"MC%d=%d%d:%d%d:%d%d",(lapC),(mm3/10),(mm3%10),(dd3/10),(dd3%10),(ms3/10),(ms3%10));
		lcd_send_string(buffer);
	}
	else{
		lcd_send_cmd(0xd4);
		lcd_send_string("MC=FINISH");
	}

	///////////////////////////////////////////////////////////////////STOP Timer2
	if(lapA>=totlap && lapB>=totlap && lapC>=totlap)HAL_TIM_Base_Stop_IT(&htim2);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Instance==TIM2){
		ms++;
		if(ms>=100){
			dd++;
			ms=0;
		}
		if(dd>=60){
			mm++;
			dd=0;
		}
		if(mm>=100){
			ms=0;
			dd=0;
			mm=0;
		}
	}

	if(htim->Instance==TIM4){

		///////////////////////////////Baca Tombol Dan Sensor
		tombolrunstop();
		tombolreset();
		sensor1();
		sensor2();
		sensor3();

		///////////////////////////////Kirim LCD
		kirim_lcd();
	}
}
