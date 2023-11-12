/***************************************************************************//**
  @file     ModulatorV1.c
  @brief    modulator functions
  @author   Grupo 5
 ******************************************************************************/

#include "ModuladorV1.h"
#include "DAC.h"
#include "PIT.h"
#include "Queues/queue.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/

#define		UART_LENGTH		11
#define		LSB_BIT			0b00000001
#define		SINE_2200_OFFSET		11
#define		SINE_1200_OFFSET		6

void (*MsgSentClb)(void);

/*******************************************************************************
 * VARIABLES WITH LOCAL SCOPE
 ******************************************************************************/

static uint32_t duty = 190;
static uint32_t period = 379;

static uint16_t sine[] = {
		1024,1034,1043,1053,1063,1073,1082,1092,1102,1112,1121,1131,1141,1150,1160,1170,
		1179,1189,1199,1208,1218,1227,1237,1246,1256,1265,1275,1284,1294,1303,1312,1322,
		1331,1340,1350,1359,1368,1377,1386,1396,1405,1414,1423,1432,1440,1449,1458,1467,
		1476,1485,1493,1502,1510,1519,1528,1536,1544,1553,1561,1569,1578,1586,1594,1602,
		1610,1618,1626,1634,1642,1649,1657,1665,1672,1680,1687,1695,1702,1709,1716,1724,
		1731,1738,1745,1752,1758,1765,1772,1778,1785,1791,1798,1804,1811,1817,1823,1829,
		1835,1841,1847,1852,1858,1864,1869,1875,1880,1885,1891,1896,1901,1906,1911,1916,
		1920,1925,1930,1934,1939,1943,1947,1951,1955,1959,1963,1967,1971,1975,1978,1982,
		1985,1988,1992,1995,1998,2001,2004,2007,2009,2012,2014,2017,2019,2021,2024,2026,
		2028,2029,2031,2033,2035,2036,2038,2039,2040,2041,2042,2043,2044,2045,2046,2046,
		2047,2047,2048,2048,2048,2048,2048,2048,2048,2047,2047,2046,2046,2045,2044,2043,
		2042,2041,2040,2039,2038,2036,2035,2033,2031,2029,2028,2026,2024,2021,2019,2017,
		2014,2012,2009,2007,2004,2001,1998,1995,1992,1988,1985,1982,1978,1975,1971,1967,
		1963,1959,1955,1951,1947,1943,1939,1934,1930,1925,1920,1916,1911,1906,1901,1896,
		1891,1885,1880,1875,1869,1864,1858,1852,1847,1841,1835,1829,1823,1817,1811,1804,
		1798,1791,1785,1778,1772,1765,1758,1752,1745,1738,1731,1724,1716,1709,1702,1695,
		1687,1680,1672,1665,1657,1649,1642,1634,1626,1618,1610,1602,1594,1586,1578,1569,
		1561,1553,1544,1536,1528,1519,1510,1502,1493,1485,1476,1467,1458,1449,1440,1432,
		1423,1414,1405,1396,1386,1377,1368,1359,1350,1340,1331,1322,1312,1303,1294,1284,
		1275,1265,1256,1246,1237,1227,1218,1208,1199,1189,1179,1170,1160,1150,1141,1131,
		1121,1112,1102,1092,1082,1073,1063,1053,1043,1034,1024,1014,1005,995,985,975,
		966,956,946,936,927,917,907,898,888,878,869,859,849,840,830,821,
		811,802,792,783,773,764,754,745,736,726,717,708,698,689,680,671,
		662,652,643,634,625,616,608,599,590,581,572,563,555,546,538,529,
		520,512,504,495,487,479,470,462,454,446,438,430,422,414,406,399,
		391,383,376,368,361,353,346,339,332,324,317,310,303,296,290,283,
		276,270,263,257,250,244,237,231,225,219,213,207,201,196,190,184,
		179,173,168,163,157,152,147,142,137,132,128,123,118,114,109,105,
		101,97,93,89,85,81,77,73,70,66,63,60,56,53,50,47,
		44,41,39,36,34,31,29,27,24,22,20,19,17,15,13,12,
		10,9,8,7,6,5,4,3,2,2,1,1,0,0,0,0,
		0,0,0,1,1,2,2,3,4,5,6,7,8,9,10,12,
		13,15,17,19,20,22,24,27,29,31,34,36,39,41,44,47,
		50,53,56,60,63,66,70,73,77,81,85,89,93,97,101,105,
		109,114,118,123,128,132,137,142,147,152,157,163,168,173,179,184,
		190,196,201,207,213,219,225,231,237,244,250,257,263,270,276,283,
		290,296,303,310,317,324,332,339,346,353,361,368,376,383,391,399,
		406,414,422,430,438,446,454,462,470,479,487,495,504,512,520,529,
		538,546,555,563,572,581,590,599,608,616,625,634,643,652,662,671,
		680,689,698,708,717,726,736,745,754,764,773,783,792,802,811,821,
		830,840,849,859,869,878,888,898,907,917,927,936,946,956,966,975,
		985,995,1005,1014
};


static uint8_t uartData[UART_LENGTH];
static uint8_t	char_index = UART_LENGTH;
static queue_uint8_t buffer;

static int sineIndex = 0;
static uint16_t counter;
/*******************************************************************************
 * FUNCTION PROTOTYPES FOR PRIVATE FUNCTIONS WITH FILE LEVEL SCOPE
 ******************************************************************************/


/**
 * @brief
 */
static void send_data_to_modulate(void);


/**
 * @brief
 */
static void create_bit_frame(uint8_t data);


/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


void Modulator_Init(void(*clb)(void))
{
	queue_Init_uint8(&buffer);
	char_index = UART_LENGTH;	// no data to be sent

	MsgSentClb = clb;

	DAC_init();

	PIT_Init();

	PIT_set_Timer(0, 758, send_data_to_modulate);

	PIT_Start_Timer(0);
}



int8_t modulator_sendChar(uint8_t data)
{

	return push_Queue_Element_uint8(&buffer, data);
}


/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/


static void create_bit_frame(uint8_t data)
{
	uartData[0] = 0;
	uint8_t parity = 1;
	char_index = 0;

	uint8_t i;
	for(i = 1; i < UART_LENGTH - 2; i++)
	{
		uartData[i] = data & LSB_BIT;
		parity ^= uartData[i];
		data >>= 1;
	}

	uartData[UART_LENGTH - 2] = parity;
	uartData[UART_LENGTH - 1] = 1;
}


static void send_data_to_modulate(void)
{

	static uint8_t  index = 0;
	static uint8_t  step  = 0;

	//Proceso la siguiente palabra que viene.
	if(char_index > 10)
	{
		if(get_Queue_Status_uint8(&buffer))
		{
			uint8_t data = pull_Queue_Element_uint8(&buffer);
			create_bit_frame(data);
			counter = 0;
		}
		else //Pasa a estado idle
		{
			DAC_SetData(SINE_1200_OFFSET * sizeof(sine[0]));
		}
	}

	if(char_index < 11)
	{
		if(uartData[char_index] == 1)
		{
			DAC_SetData(SINE_1200_OFFSET * sizeof(sine[0]));
			//change_dma(SINE_1200_OFFSET);
		}
		else if (uartData[char_index] == 0 && char_index < 10)
		{
			DAC_SetData(sine[]);
			//change_dma(SINE_2200_OFFSET);
		}
		else
		{
			DAC_SetData(SINE_1200_OFFSET * sizeof(sine[0]));
			//change_dma(SINE_1200_OFFSET);	// asegurar bit stop en 1
		}

		char_index++;
	}
}


/*******************************************************************************
 ******************************************************************************/
