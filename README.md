AtmelCodingAssignment
=====================
The complete assignment is implemented with small state machine.

- Tasks 1-5 are implemented as part of APP_STATE_INIT.
 * This state initializes the OLED with "Author Name" & "Company Name"
 * This state prints "Author Name" & "Company Name" on UART Console.
- Task 6.a is implemented as part of APP_STATE_RUN_DATA_SWAP_TEST.
 * This state demonstrates the unit testing of swap_data function with various combinations.
 * The test result output is printed on UART console.
- Task 6.b is implemented as part of APP_STATE_RUN_COUNT_SET_BIT_TEST.
 * This state demonstrates the unit testing of count set bits in Byte with various inputs.
 * The test result output is printed on UART console.
- Task 6.c is implemented as part of APP_STATE_RUN_SET_BIT_TEST.
 * This state demonstrates the unit testing of check Nth bit set in various 32bit input data.
 * The test result output is printed on UART console.
- State can be changed by pressing the "Button 1" on OLED1 module.
- States are changed in round robin fasion.
- APP_STATE_INIT state runs only ones at powerUp.
