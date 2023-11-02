/*
 * spi.c
 *
 *  Created on: Sep 20, 2022
 *      Author: hussamaldean
 */


#include "spi.h"
#include "stm32f4xx.h"

#define AF05 0x05


void st7789_spi_init()
{

	RCC->AHB1ENR|=RCC_AHB1ENR_GPIOAEN;
	/*SPI pins*/
	GPIOA->MODER|=GPIO_MODER_MODE5_1|GPIO_MODER_MODE6_1|GPIO_MODER_MODE7_1;
	GPIOA->MODER&=~(GPIO_MODER_MODE5_0|GPIO_MODER_MODE6_0|GPIO_MODER_MODE7_0);
	GPIOA->AFR[0]|=(AF05<<GPIO_AFRL_AFSEL5_Pos)|(AF05<<GPIO_AFRL_AFSEL6_Pos)|(AF05<<GPIO_AFRL_AFSEL7_Pos);

	/*DC and RST pins*/
	GPIOA->MODER|=GPIO_MODER_MODE0_0|GPIO_MODER_MODE1_0;
	GPIOA->MODER&=~(GPIO_MODER_MODE0_1|GPIO_MODER_MODE1_1);

	/*SPI configuration*/

	RCC->APB2ENR|=RCC_APB2ENR_SPI1EN;
	SPI1->CR1|=SPI_CR1_CPHA|SPI_CR1_CPOL|SPI_CR1_SSM|SPI_CR1_BR_0|SPI_CR1_SSI|SPI_CR1_MSTR|SPI_CR1_SPE;
}

void st7789_spi_transmit(uint8_t *buff,uint32_t buff_size)
{
	if( buff_size <= 0xFFFF ){
		HAL_SPI_Transmit(&ST7789_SPI_HAL, buff, buff_size, HAL_MAX_DELAY);
	}
	else{
		while( buff_size > 0xFFFF ){
			HAL_SPI_Transmit(&ST7789_SPI_HAL, buff, 0xFFFF, HAL_MAX_DELAY);
			buff_size-=0xFFFF;
			buff+=0xFFFF;
		}
		HAL_SPI_Transmit(&ST7789_SPI_HAL, buff, buff_size, HAL_MAX_DELAY);
	}

	while(HAL_SPI_GetState(&ST7789_SPI_HAL) != HAL_SPI_STATE_READY){};
}
