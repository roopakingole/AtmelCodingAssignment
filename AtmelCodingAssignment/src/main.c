/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * This is a bare minimum user application template.
 *
 * For documentation of the board, go \ref group_common_boards "here" for a link
 * to the board-specific documentation.
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# Minimal main function that starts with a call to board_init()
 * -# Basic usage of on-board LED and button
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
#include <asf.h>
#include <string.h>
#include "stdio_serial.h"


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

static void test_display_names(void)
{
#define AUTHOR_NAME_STRING "Roopak Ingole"
#define COMPANY_NAME_STRING "Atmel Corp."

	// Clear screen.
	ssd1306_clear();
	ssd1306_set_page_address(0);
	ssd1306_set_column_address(0);
	ssd1306_write_text(AUTHOR_NAME_STRING);      /* defined above as #define AUTHOR_NAME_STRING "Roopak Ingole" */
	puts(AUTHOR_NAME_STRING);
	ssd1306_set_page_address(1);
	ssd1306_set_column_address(0);
	ssd1306_write_text(COMPANY_NAME_STRING);    /* defined above as #define COMPANY_NAME_STRING "Atmel Corp."  */
	puts(COMPANY_NAME_STRING);
}



static void swap_data(int* data1, int* data2)
{
	if((data1 != NULL) && (data2 != NULL) && (*data1 != *data2))
	{
		*data1 = *data1 ^ *data2;
		*data2 = *data1 ^ *data2;
		*data1 = *data1 ^ *data2;
	}
	
}

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

static uint8_t count_set_bits(uint8_t data)
{
	uint8_t x = data;
	x=((0xaa&x)>>1)+(0x55&x);		// 4 groups of 2 bits are joined to form 2 groups of 4 bits
	x=((0xcc&x)>>2)+(0x33&x);		// 2 groups of 4 bits are joined to form 1 group of 8 bits
	x=((0xf0&x)>>4)+(0x0f&x);		// 1 group of 8 bits contains total no. of 1s
	return x;
}

static void test_count_set_bits(void)
{
	uint8_t test_sample = 0;

	printf("--Number of Set Bits in a Byte Test--\r\n");

	for(int i = 0; i < 5; i++)
	{
		test_sample = rand() % 0xFF;
		printf("Number of Set Bits in: 0x%X = %d\r\n", test_sample, count_set_bits(test_sample));
	}
	
}

static bool check_bit_set(uint32_t data, uint8_t index)
{
	uint32_t mask = (uint32_t)(1 << index);
	return ((data & mask) == mask);
}

static void test_check_bit_set(void)
{
	uint32_t test_sample = 0;
	uint8_t test_index = 0;
	
	printf("--Nth Bit Set Test--\r\n");
	for(int i = 0; i < 5; i++)
	{
		test_sample = rand() % 0xFFFFFFFF;
		test_index = rand() % 32;
		printf("Is Bit %d set in 0x%X? = %s(%d)\r\n", test_index, test_sample, check_bit_set(test_sample,test_index)?"TRUE":"FALSE", check_bit_set(test_sample,test_index));
	}
	
}


int main (void)
{

	// Initialize clocks.
	sysclk_init();

	// Initialize GPIO states.
	board_init();

	/* Configure UART for debug message output. */
	configure_console();

	// Initialize SPI and SSD1306 controller.
	ssd1306_init();
	ssd1306_clear();

	// Insert application code here, after the board has been initialized.
	test_display_names();
	test_swap_data();
	test_count_set_bits();
	test_check_bit_set();	


	// This skeleton code simply sets the LED to the state of the button.
	while (1) {
		// Is button pressed?
		if (ioport_get_pin_level(BUTTON_0_PIN) == BUTTON_0_ACTIVE) {
			// Yes, so turn LED on.
			ioport_set_pin_level(LED_0_PIN, LED_0_ACTIVE);
		} else {
			// No, so turn LED off.
			ioport_set_pin_level(LED_0_PIN, !LED_0_ACTIVE);
		}
	}
}
