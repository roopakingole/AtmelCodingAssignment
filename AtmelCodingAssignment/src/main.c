/**
 * \file main.c
 *
 * \brief User application for Atmel Coding Assignment
 *
 */

/**
 * \mainpage User Application for Atmel Coding Assignment
 * \brief The complete assignment is implemented with small state machine.
 * \par
 *        Tasks 1-5 are implemented as part of APP_STATE_INIT.
 *            - This state initializes the OLED with "Author Name" & "Company Name"
 *            - This state prints "Author Name" & "Company Name" on UART Console.
 * \par
 *        Task  6.a is implemented as part of APP_STATE_RUN_DATA_SWAP_TEST.
 *			  - This state demonstrates the unit testing of swap_data function with various combinations.
 *			  - The test result output is printed on UART console.
 * \par
 *		  Task  6.b is implemented as part of APP_STATE_RUN_COUNT_SET_BIT_TEST.
 *			  - This state demonstrates the unit testing of count set bits in Byte with various inputs.
 *			  - The test result output is printed on UART console.
 * \par
 *		  Task  6.c is implemented as part of APP_STATE_RUN_SET_BIT_TEST.
 *			  - This state demonstrates the unit testing of check Nth bit set in various 32bit input data.
 *			  - The test result output is printed on UART console.
 * \par
 *		  State can be changed by pressing the "Button 1" on OLED1 module.
 * \par
 *		  States are changed in round robin fasion.
 * \par
 *		  APP_STATE_INIT state runs only ones at powerUp.
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
#include <asf.h>
#include <string.h>
#include "stdio_serial.h"

/* IRQ priority for PIO (The lower the value, the greater the priority) */
#define IRQ_PRIOR_PIO    0
/*! \def MAX_NUM_OF_STATES
    \brief A macro that defines Maximum number of states in state machine.
*/
#define MAX_NUM_OF_STATES 4

/** \enum app_state_t
 *
 */
typedef enum app_state_t_
{
	APP_STATE_INIT = 0,
	APP_STATE_RUN_DATA_SWAP_TEST = 1,
	APP_STATE_RUN_COUNT_SET_BIT_TEST = 2,
	APP_STATE_RUN_SET_BIT_TEST = 3,
}app_state_t;


/*! \var typedef unsigned int UINT32
    \brief A type definition for a .
    
    Details.
*/
/*Global Data Declaration*/
char author_name[] = "Roopak Ingole";
char company_name[] = "Atmel Corp.";
bool test_state_switch = true;
app_state_t test_current_state = APP_STATE_RUN_SET_BIT_TEST;
bool init_complete = false;


/*Function Prototypes*/
static void configure_console(void);
static void test_display_names(void);
static void swap_data(int* data1, int* data2);
static void test_swap_data(void);
static uint8_t count_set_bits(uint8_t data);
static void test_count_set_bits(void);
static bool check_bit_set(uint32_t data, uint8_t index);
static void test_check_bit_set(void);
static void Button1_Handler(uint32_t id, uint32_t mask);
static void configure_buttons(void);


/*Start from here...*/
int main (void)
{

	// Initialize clocks.
	sysclk_init();

	// Initialize GPIO states.
	board_init();

	/* Configure UART for debug message output. */
	configure_console();
	
	// Configure IO1 buttons.
	configure_buttons();	

	// Initialize SPI and SSD1306 controller.
	ssd1306_init();
	ssd1306_clear();

	// Insert application code here, after the board has been initialized.
	while(true)
	{
		if(test_state_switch == true)
		{
			test_current_state = (test_current_state + 1) % MAX_NUM_OF_STATES;
			//Skip init state
			if((test_current_state == APP_STATE_INIT)&&(init_complete))
				test_current_state++;			

			//printf("State = %d\r\n", test_current_state);
			switch(test_current_state)
			{
				case APP_STATE_INIT:
				{
					/********************************************************************************************************************************
					1) Download Atmel Studio and update to the latest ASF version

					2) Bring up the SAM4S Xplained Pro and display some text on the OLED display
					- create a new project from the ASF examples for SAM4S
					- Change the source code in the example to output to OLED display when device first boots
					Your name
					Atmel Corp

					Using your camera phone, take a snapshot of your development board and include the photo in your tutorial.

					3) Restart the firmware under debug, set a breakpoint at the line of code where you are outputting your name
					a.  add the C variable containing your name into the “watch” window
					b.  Take a screen shot of your Atmel Studio environment with the break point set and stopped on that line of code
					c.  Include screen shot in your report

					4) Download and install the Terminal Window plug-in from the Atmel Studio gallery

					5) Bring up the firmware in Atmel Studio and output your name to the terminal window through the EDBG port of the development board.
					a. Include a screenshot of your terminal window in your report.
					********************************************************************************************************************************/
					test_display_names();
					init_complete = true;
					break;
				}
				case APP_STATE_RUN_DATA_SWAP_TEST:
				{
					/********************************************************************************************************************************
					6) Code exercises
					a. write a function that can swap two variables without using an intermediate variable, output test results to terminal window
					********************************************************************************************************************************/
					test_swap_data();
					break;
				}
				case APP_STATE_RUN_COUNT_SET_BIT_TEST:
				{
					/********************************************************************************************************************************
					6) Code exercises
					b. write a function that will count the number of bits set in a byte, output test results to terminal window
					********************************************************************************************************************************/
					test_count_set_bits();
					break;
				}
				case APP_STATE_RUN_SET_BIT_TEST:
				{
					/********************************************************************************************************************************
					6) Code exercises
					c. write a function that takes a 32 bit word, a bit index (nth bit), tests to see if the nth bit is set, returns result, output test results to terminal window
					********************************************************************************************************************************/
					test_check_bit_set();
					break;
				}
				default:
				{
					break;
				}
			}
			test_state_switch = false;
		}
		else
		{
			delay_ms(100);
		}
	}
}





/**\fn void Button1_Handler(uint32_t id, uint32_t mask)
 * \brief Handler for Button 1 rising edge interrupt.
 * \param id The button ID.
 * \param mask The button mask.
 */
static void Button1_Handler(uint32_t id, uint32_t mask)
{
	if ((PIN_PUSHBUTTON_1_ID == id) && (PIN_PUSHBUTTON_1_MASK == mask))
		test_state_switch = true;
}


/**
 * \brief Configure the Pushbuttons.
 *
 * Configure the PIO as inputs and generate corresponding interrupt when
 * pressed or released.
 */
static void configure_buttons(void)
{
	/* Configure Pushbutton 1. */
	pmc_enable_periph_clk(PIN_PUSHBUTTON_1_ID);
	pio_set_debounce_filter(PIN_PUSHBUTTON_1_PIO, PIN_PUSHBUTTON_1_MASK, 10);
	pio_handler_set(PIN_PUSHBUTTON_1_PIO, PIN_PUSHBUTTON_1_ID,
			PIN_PUSHBUTTON_1_MASK, PIN_PUSHBUTTON_1_ATTR, Button1_Handler);
	NVIC_EnableIRQ((IRQn_Type) PIN_PUSHBUTTON_1_ID);
	pio_handler_set_priority(PIN_PUSHBUTTON_1_PIO, (IRQn_Type) PIN_PUSHBUTTON_1_ID, IRQ_PRIOR_PIO);
	pio_enable_interrupt(PIN_PUSHBUTTON_1_PIO, PIN_PUSHBUTTON_1_MASK);
}

/**
 * \brief Configure the UART Console.
 *
 * Configure the UART Console
 */
static void configure_console(void)
{
	const usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
		.paritytype = CONF_UART_PARITY
	};

	/* Configure console UART. */
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	stdio_serial_init(CONF_UART, &uart_serial_options);
}

/**
 * \brief Display Names on OLED & Console
 *
 * Display Author Name & Company Name on OLED & Console
 */
static void test_display_names(void)
{

	// Clear screen.
	ssd1306_clear();
	ssd1306_set_page_address(0);
	ssd1306_set_column_address(0);
	ssd1306_write_text(author_name);
	puts(author_name); 
	ssd1306_set_page_address(1);
	ssd1306_set_column_address(0);
	ssd1306_write_text(company_name);
	puts(company_name);
}


/**
 * \brief Swap Data without intermediate data.
 * \param data1 First variable data.
 * \param data2 Second variable data.
 * Swaps contents of two variables without using intermediate data.
 */
static void swap_data(int* data1, int* data2)
{
	if((data1 != NULL) && (data2 != NULL) && (*data1 != *data2))
	{
		*data1 = *data1 ^ *data2;
		*data2 = *data1 ^ *data2;
		*data1 = *data1 ^ *data2;
	}
	
}

/**
 * \brief Test function to test Swap Data
 *
 * Test function to test swap_data().
 */
static void test_swap_data(void)
{
	int d1 = 10;
	int d2 = 20;
	printf("--Swap Data Test--\r\n");
	printf("Input(D1=%d, D2=%d) --> ", d1, d2);
	swap_data(&d1, &d2);
	printf("Output(D1=%d, D2=%d)\r\n", d1, d2);
	
	d1 = 10;
	d2 = 10;
	printf("Input(D1=%d, D2=%d) --> ", d1, d2);
	swap_data(&d1, &d2);
	printf("Output(D1=%d, D2=%d)\r\n", d1, d2);

	d1 = 0;
	d2 = 20;
	printf("Input(D1=%d, D2=%d) --> ", d1, d2);
	swap_data(&d1, &d2);
	printf("Output(D1=%d, D2=%d)\r\n", d1, d2);

	d1 = 10;
	d2 = 0;
	printf("Input(D1=%d, D2=%d) --> ", d1, d2);
	swap_data(&d1, &d2);
	printf("Output(D1=%d, D2=%d)\r\n", d1, d2);
}


/**
 * \brief Count number of bits set in byte data.
 * \param data Input data
 */
static uint8_t count_set_bits(uint8_t data)
{
	uint8_t x = data;
	x=((0xaa&x)>>1)+(0x55&x);		// 4 groups of 2 bits are joined to form 2 groups of 4 bits
	x=((0xcc&x)>>2)+(0x33&x);		// 2 groups of 4 bits are joined to form 1 group of 8 bits
	x=((0xf0&x)>>4)+(0x0f&x);		// 1 group of 8 bits contains total no. of 1s
	return x;
}

/**
 * \brief Test function to test count set bits
 *
 * Test function to test count_set_bits().
 */
static void test_count_set_bits(void)
{
	uint8_t test_sample = 0;

	printf("--Number of Set Bits in a Byte Test--\r\n");

	for(int i = 0; i < 5; i++)
	{
		test_sample = rand() % 0xFF;
		printf("Number of Set Bits in: 0x%X = %d\r\n", test_sample, count_set_bits(test_sample));
		delay_ms(50);
	}
	
}

/**
 * \brief Check index(0 based) bit set in 32bit data.
 * \param data Input data
 * \param index Index(0-based) to check if that bit is set.
 */
static bool check_bit_set(uint32_t data, uint8_t index)
{
	uint32_t mask = (uint32_t)(1 << index);
	return ((data & mask) == mask);
}


/**
 * \brief Test function to test set bit check
 *
 * Test function to test check_bit_set().
 */
static void test_check_bit_set(void)
{
	uint32_t test_sample = 0;
	uint8_t test_index = 0;
	
	printf("--Nth Bit Set Test--\r\n");
	for(int i = 0; i < 5; i++)
	{
		test_sample = rand() % 0xFFFFFFFF;
		test_index = rand() % 32;
		printf("Is Bit %d set in 0x%X? = %s(%d)\r\n", test_index, (unsigned int)test_sample, check_bit_set(test_sample,test_index)?"TRUE":"FALSE", check_bit_set(test_sample,test_index));
		delay_ms(50);
	}
	
}
