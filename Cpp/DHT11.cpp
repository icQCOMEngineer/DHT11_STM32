#include "DHT11.h"

extern "C" {
	GPIO_PinState HAL_GPIO_ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
	void delay_us(uint32_t delay);
}
void DHT11::DHT11StartSignal()
{
}

uint8_t DHT11::DHT11Response()
{
	return uint8_t();
}

uint8_t DHT11::DHT11_ReadData()
{
	unsigned j;
	for (unsigned i = 0; i < 8; i++)
	{
		while (!HAL_GPIO_ReadPin(GPIO, *GPIO_Pin_Num));
		delay_us(40);
		if (!HAL_GPIO_ReadPin(GPIO, *GPIO_Pin_Num))
			j &= ~(1 << (7 - i));
		else
			j |= (1 << (7 - i));
		while (HAL_GPIO_ReadPin(GPIO, *GPIO_Pin_Num));
	}
	return j;
}

uint8_t DHT11::DHT11_Init()
{
	DHT11StartSignal();
	return DHT11Response();
}

DHT11::DHT11(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
	:h1(0),h2(0),t1(0),t2(0),check(0),GPIO(GPIOx),GPIO_Pin_Num(&GPIO_Pin)
{
	DHT11_Init();
	h1 = DHT11_ReadData();
	h2 = DHT11_ReadData();
	t1 = DHT11_ReadData();
	t2 = DHT11_ReadData();
}

float DHT11::DHT11_GetHumidity()
{
	char hum[5];
	sprintf(hum, "%.2i.%.2i", h1, h2);
	return atof(hum);
}

float DHT11::DHT11_GetTemperature()
{
	char temp[5];
	sprintf(temp, "%.2i.%.2i", t1, t2);
	return atof(temp);
}

float DHT11::DHT11_ConvertToCelsuis(float t)
{
	return DHT11_GetTemperature()*1.8 + 32;
}

float DHT11::DHT11_ConvertToFahr(float t)
{
	return (DHT11_ConvertToCelsuis(t)-32) /1.8;
}