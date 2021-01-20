
#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL02Z4.h"
#include "fsl_debug_console.h"
#include "sdk_hal_gpio.h"
#include "sdk_hal_uart0.h"

int main(void) {

    status_t status;
    uint8_t nuevo_byte_uart;

  	/* Init board hardware. */
    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    (void)uart0Inicializar(115200);    //115200bps

    PRINTF("Usar teclado para controlar LEDs\r\n");
    PRINTF("r-R led ROJO\r\n");
    PRINTF("v-V led VERDE\r\n");
    PRINTF("a-A led AZUL\r\n");

    while(1) {
        if(uart0CuantosDatosHayEnBuffer()>0){
            status=uart0LeerByteDesdeBuffer(&nuevo_byte_uart);
            if(status==kStatus_Success){
                printf("dato:%c\r\n",nuevo_byte_uart);
                switch (nuevo_byte_uart) {
                case 'a':
                case 'A':
                    gpioPutToggle(KPTB10);
                    break;

                case 'v':
                    gpioPutHigh(KPTB7);
                    break;
                case 'V':
                    gpioPutLow(KPTB7);
                    break;

                case 'r':
                    gpioPutValue(KPTB6,1);
                    break;
                case 'R':
                    gpioPutValue(KPTB6,0);
                    break;
                }
            }else{
                printf("error\r\n");
            }
        }
    }
    return 0 ;
}
