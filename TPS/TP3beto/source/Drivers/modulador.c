/***************************************************************************//**
  @file     App.c
  @brief    Application functions
  @author   Grupo 5
 ******************************************************************************/

/*******************************************************************************
 * INCLUDE HEADER FILES
 ******************************************************************************/

#include "modulador.h"
#include "dma_drv.h"
#include "Drivers/FTM.h"
#include "Drivers/PIT.h"
#include "Queues/queue.h"


/*******************************************************************************
 * CONSTANT AND MACRO DEFINITIONS USING #DEFINE
 ******************************************************************************/
#define		UART_LENGTH		11
#define		LSB_BIT			0b00000001
#define		SINE_2200_OFFSET		11
#define		SINE_1200_OFFSET		6

/*******************************************************************************
 * VARIABLES WITH LOCAL SCOPE
 ******************************************************************************/

static uint32_t duty = 190;
static uint32_t period = 379;

static uint16_t sine[] = {
		190 , 191 , 193 , 195 , 197 , 198 , 200 , 202 ,
		204 , 206 , 207 , 209 , 211 , 213 , 215 , 216 ,
		218 , 220 , 222 , 223 , 225 , 227 , 229 , 230 ,
		232 , 234 , 236 , 237 , 239 , 241 , 243 , 244 ,
		246 , 248 , 249 , 251 , 253 , 255 , 256 , 258 ,
		260 , 261 , 263 , 265 , 266 , 268 , 269 , 271 ,
		273 , 274 , 276 , 277 , 279 , 281 , 282 , 284 ,
		285 , 287 , 288 , 290 , 291 , 293 , 294 , 296 ,
		297 , 299 , 300 , 302 , 303 , 305 , 306 , 307 ,
		309 , 310 , 312 , 313 , 314 , 316 , 317 , 318 ,
		320 , 321 , 322 , 323 , 325 , 326 , 327 , 328 ,
		330 , 331 , 332 , 333 , 334 , 335 , 337 , 338 ,
		339 , 340 , 341 , 342 , 343 , 344 , 345 , 346 ,
		347 , 348 , 349 , 350 , 351 , 352 , 353 , 354 ,
		355 , 355 , 356 , 357 , 358 , 359 , 359 , 360 ,
		361 , 362 , 362 , 363 , 364 , 364 , 365 , 366 ,
		366 , 367 , 368 , 368 , 369 , 369 , 370 , 370 ,
		371 , 371 , 372 , 372 , 373 , 373 , 373 , 374 ,
		374 , 375 , 375 , 375 , 376 , 376 , 376 , 376 ,
		377 , 377 , 377 , 377 , 377 , 377 , 378 , 378 ,
		378 , 378 , 378 , 378 , 378 , 378 , 378 , 378 ,
		378 , 378 , 378 , 378 , 378 , 377 , 377 , 377 ,
		377 , 377 , 377 , 376 , 376 , 376 , 376 , 375 ,
		375 , 375 , 374 , 374 , 373 , 373 , 373 , 372 ,
		372 , 371 , 371 , 370 , 370 , 369 , 369 , 368 ,
		368 , 367 , 366 , 366 , 365 , 364 , 364 , 363 ,
		362 , 362 , 361 , 360 , 359 , 359 , 358 , 357 ,
		356 , 355 , 355 , 354 , 353 , 352 , 351 , 350 ,
		349 , 348 , 347 , 346 , 345 , 344 , 343 , 342 ,
		341 , 340 , 339 , 338 , 337 , 335 , 334 , 333 ,
		332 , 331 , 330 , 328 , 327 , 326 , 325 , 323 ,
		322 , 321 , 320 , 318 , 317 , 316 , 314 , 313 ,
		312 , 310 , 309 , 307 , 306 , 305 , 303 , 302 ,
		300 , 299 , 297 , 296 , 294 , 293 , 291 , 290 ,
		288 , 287 , 285 , 284 , 282 , 281 , 279 , 277 ,
		276 , 274 , 273 , 271 , 269 , 268 , 266 , 265 ,
		263 , 261 , 260 , 258 , 256 , 255 , 253 , 251 ,
		249 , 248 , 246 , 244 , 243 , 241 , 239 , 237 ,
		236 , 234 , 232 , 230 , 229 , 227 , 225 , 223 ,
		222 , 220 , 218 , 216 , 215 , 213 , 211 , 209 ,
		207 , 206 , 204 , 202 , 200 , 198 , 197 , 195 ,
		193 , 191 , 190 , 188 , 186 , 184 , 182 , 181 ,
		179 , 177 , 175 , 173 , 172 , 170 , 168 , 166 ,
		164 , 163 , 161 , 159 , 157 , 156 , 154 , 152 ,
		150 , 149 , 147 , 145 , 143 , 142 , 140 , 138 ,
		136 , 135 , 133 , 131 , 130 , 128 , 126 , 124 ,
		123 , 121 , 119 , 118 , 116 , 114 , 113 , 111 ,
		110 , 108 , 106 , 105 , 103 , 102 , 100 , 98 ,
		97 , 95 , 94 , 92 , 91 , 89 , 88 , 86 ,
		85 , 83 , 82 , 80 , 79 , 77 , 76 , 74 ,
		73 , 72 , 70 , 69 , 67 , 66 , 65 , 63 ,
		62 , 61 , 59 , 58 , 57 , 56 , 54 , 53 ,
		52 , 51 , 49 , 48 , 47 , 46 , 45 , 44 ,
		42 , 41 , 40 , 39 , 38 , 37 , 36 , 35 ,
		34 , 33 , 32 , 31 , 30 , 29 , 28 , 27 ,
		26 , 25 , 24 , 24 , 23 , 22 , 21 , 20 ,
		20 , 19 , 18 , 17 , 17 , 16 , 15 , 15 ,
		14 , 13 , 13 , 12 , 11 , 11 , 10 , 10 ,
		9 , 9 , 8 , 8 , 7 , 7 , 6 , 6 ,
		6 , 5 , 5 , 4 , 4 , 4 , 3 , 3 ,
		3 , 3 , 2 , 2 , 2 , 2 , 2 , 2 ,
		1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 ,
		1 , 1 , 1 , 1 , 1 , 1 , 1 , 2 ,
		2 , 2 , 2 , 2 , 2 , 3 , 3 , 3 ,
		3 , 4 , 4 , 4 , 5 , 5 , 6 , 6 ,
		6 , 7 , 7 , 8 , 8 , 9 , 9 , 10 ,
		10 , 11 , 11 , 12 , 13 , 13 , 14 , 15 ,
		15 , 16 , 17 , 17 , 18 , 19 , 20 , 20 ,
		21 , 22 , 23 , 24 , 24 , 25 , 26 , 27 ,
		28 , 29 , 30 , 31 , 32 , 33 , 34 , 35 ,
		36 , 37 , 38 , 39 , 40 , 41 , 42 , 44 ,
		45 , 46 , 47 , 48 , 49 , 51 , 52 , 53 ,
		54 , 56 , 57 , 58 , 59 , 61 , 62 , 63 ,
		65 , 66 , 67 , 69 , 70 , 72 , 73 , 74 ,
		76 , 77 , 79 , 80 , 82 , 83 , 85 , 86 ,
		88 , 89 , 91 , 92 , 94 , 95 , 97 , 98 ,
		100 , 102 , 103 , 105 , 106 , 108 , 110 , 111 ,
		113 , 114 , 116 , 118 , 119 , 121 , 123 , 124 ,
		126 , 128 , 130 , 131 , 133 , 135 , 136 , 138 ,
		140 , 142 , 143 , 145 , 147 , 149 , 150 , 152 ,
		154 , 156 , 157 , 159 , 161 , 163 , 164 , 166 ,
		168 , 170 , 172 , 173 , 175 , 177 , 179 , 181,
		182, 184, 186, 188
};


static uint8_t uartData[UART_LENGTH];
static uint8_t	char_index = UART_LENGTH;
static queue_uint8_t buffer;

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
static void change_dma(uint8_t soff);

/**
 * @brief
 */
static void create_bit_frame(uint8_t data);

/*******************************************************************************
 *******************************************************************************
                        GLOBAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
/**
 * @brief
 */
void modulador_init(void)
{

	queue_Init_uint8(&buffer);

	FTM_Init(FTM_0, FTM_PSC_x1, 0xFFFF, 0);
    FTM_CH_PWM_Init(FTM_0, FTM_CH_0, FTM_PWM_HIGH_PULSES, FTM_PWM_EDGE_ALIGNED, duty, period, NULL);		//90% duty cycle (en hexa)
    FTM_CH_EnableDMA(FTM_0, FTM_CH_0);
    FTM_Start(FTM_0);

    uint32_t * CnV_pointer = FTM_CH_GetCnVPointer(FTM_0, FTM_CH_0);

    DMA_source_t source_number = FTM0CH0;
	uint8_t channel = 0;

	uint32_t source_address = (uint32_t)sine;
	uint32_t dest_address = (uint32_t) CnV_pointer;
	uint8_t soff = SINE_1200_OFFSET * (sizeof(sine[0]));
	uint8_t doff = 0;
	uint8_t sSize = 2;
	uint32_t nbytes = 2;
	uint32_t citer = (sizeof(sine) / sizeof(sine[0])) / SINE_1200_OFFSET;
	uint32_t sourceBuffer_sizeof = sizeof(sine);
	uint32_t destBuffer_sizeof = 0;
	void (*cb) (void) = 0;

	dma0_init(source_number, channel, source_address, dest_address,
			soff, doff, sSize, nbytes,
			citer, sourceBuffer_sizeof, destBuffer_sizeof, cb);

	char_index = UART_LENGTH;	// no data to be sent

	PIT_init();
	PIT_set_Timer(1, 41667, send_data_to_modulate);
	PIT_Start_Timer(1);
}



/**
 * @brief
 * @return 	int8_t	-1 if max limit is exceeded
 */
int8_t modulador_send_char(uint8_t data)
{
	return push_Queue_Element_uint8(&buffer, data);
}

/*******************************************************************************
 *******************************************************************************
                        LOCAL FUNCTION DEFINITIONS
 *******************************************************************************
 ******************************************************************************/
/**
 * @brief
 */
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


/**
 * @brief
 */
static void send_data_to_modulate(void)
{
	if(char_index > 10)
	{
		if(get_Queue_Status_uint8(&buffer))
		{
			uint8_t data = pull_Queue_Element_uint8(&buffer);
			create_bit_frame(data);
		}
		else
		{
			change_dma(SINE_1200_OFFSET);
		}
	}
	if(char_index < 11)
	{
		if(uartData[char_index] == 1)
		{
			change_dma(SINE_1200_OFFSET);
		}
		else if (uartData[char_index] == 0 && char_index < 10)
		{
			change_dma(SINE_2200_OFFSET);
		}
		else
		{
			change_dma(SINE_1200_OFFSET);	// asegurar bit stop en 1
		}

		char_index++;
	}
}

/**
 * @brief
 */
static void change_dma(uint8_t step)
{
	dma0_disable(0);

	uint8_t channel = 0;
	uint32_t source_address = get_dma0_saddr(channel);
	uint32_t biter = (sizeof(sine) / sizeof(sine[0])) / step;
	uint32_t citer = biter - ( ( (source_address - (uint32_t)sine) / (sizeof(sine[0])) ) / step) ;
	uint32_t slast = sizeof(sine);


	set_dma0_source_offset(channel, step*sizeof(sine[0]));
	set_dma0_citer(channel, citer);
	set_dma0_biter(channel, biter);
	set_dma0_saddr(channel, source_address);
	set_dma0_slast(channel, slast);

	dma0_enable(0);
}



/*******************************************************************************
 ******************************************************************************/
