/*
--> Realizar un proyecto que utilice los leds RGB del microcontrolador FRDM-KL02Z realizado de la siguiente manera:
 V ON verde - v OFF verde
 R ON rojo - r OFF rojo
 A ON azul - a OFF azul
--> Mostrar los datos arrojados por el acelerometro MMA8451q de la siguiente manera:
 X o x ilustra valores y grados del eje X
 Y o y ilustra valores y grados del eje Y
 Z o z ilustra valores y grados del eje Z

*/

#include <stdio.h>
#include "board.h"
#include "peripherals.h"
#include "pin_mux.h"
#include "clock_config.h"
#include "MKL02Z4.h"
#include "fsl_debug_console.h"
#include "sdk_hal_gpio.h"
#include "sdk_hal_uart0.h"
#include "sdk_hal_i2c0.h"

#define MMA851_I2C_DEVICE_ADDRESS    0x1D



const uint16_t k_SENSITIVITY_2G = 4096;        //Sensibilidad del acelerómetro para el rango de +- 2g.

int main(void) {

    status_t status;
    uint8_t nuevo_byte_uart;
    uint8_t    nuevo_dato_i2c;
    uint8_t    nuevo_dato_i2c_1;
    uint8_t    nuevo_dato_i2c_2;
    uint8_t    nuevo_dato_i2c_3;
    uint8_t    nuevo_dato_i2c_4;
    uint8_t    nuevo_dato_i2c_5;
    uint8_t    nuevo_dato_i2c_6;
    uint16_t dxg=0, dyg=0, dzg=0;
    uint16_t xData=0, yData=0, zData=0;


  	/* Init board hardware. */

    BOARD_InitBootPins();
    BOARD_InitBootClocks();
    BOARD_InitBootPeripherals();
#ifndef BOARD_INIT_DEBUG_CONSOLE_PERIPHERAL
    /* Init FSL debug console. */
    BOARD_InitDebugConsole();
#endif

    (void)uart0Inicializar(115200);    //115200bps
    (void)i2c0MasterInit(100000);    //100kbps

    PRINTF("Usar teclado para controlar LEDs RGB y mostrar valores del acelerometro MMA8451q \r\n");
    PRINTF("R: ON <---> r: OFF. Led Rojo\r\n");
    PRINTF("V: ON <---> v: OFF. Led Verde\r\n");
    PRINTF("A: ON <---> a: OFF. Led Azul\r\n");
    PRINTF("M buscar acelerometro \r\n");
    printf("x o X: Mostrar valores del eje x\r\n");
    printf("y o Y: Mostrar valores del eje y\r\n");
    printf("z o Z: Mostrar valores del eje z\r\n");


   i2c0MasterWriteByte(MMA851_I2C_DEVICE_ADDRESS, CTRL_REG1, 0x01); //Mantiene el acelerometro en estado activo

    while(1) {
        if(uart0CuantosDatosHayEnBuffer()>0){
            status=uart0LeerByteDesdeBuffer(&nuevo_byte_uart);
            if(status==kStatus_Success){
                printf("dato:%c\r\n",nuevo_byte_uart);
                switch (nuevo_byte_uart) {

                case 'a':                // Led azul ON,
                	gpioPutValue(KPTB10,1);
                	break;
                case 'A':                // Led azul ON, led verde OFF Y led rojo OFF.
                    gpioPutValue(KPTB10,0);
                    gpioPutValue(KPTB7,1);
                    gpioPutValue(KPTB6,1);
                    break;

                case 'v':                 //Led verde OFF
                    gpioPutValue(KPTB7,1);
                    break;
                case 'V':                 // Led verde ON, led azul OFF y led rojo OFF.
                    gpioPutValue(KPTB7,0);
                    gpioPutValue(KPTB10,1);
                    gpioPutValue(KPTB6,1);
                    break;

                case 'r':                 //Led rojo OFF.
                    gpioPutValue(KPTB6,1);
                    break;
                case 'R':                 // Led rojo ON, led verde OFF Y led Azul OFF.
                    gpioPutValue(KPTB6,0);
                    gpioPutValue(KPTB10,1);
                    gpioPutValue(KPTB7,1);
                    break;


                case 'M': //Busca al acelerometro
                case 'm':
                     i2c0MasterReadByte(&nuevo_dato_i2c, MMA851_I2C_DEVICE_ADDRESS, WHO_AM_I);

                     if(nuevo_dato_i2c==0x1A)
                                        printf("MMA8451 encontrado!!\r\n");
                     else
                                        printf("MMA8451 error\r\n");

                     break;


                case 'x':
                case 'X':
                    i2c0MasterReadByte(&nuevo_dato_i2c_1, MMA851_I2C_DEVICE_ADDRESS, OUT_X_MSB);
                    i2c0MasterReadByte(&nuevo_dato_i2c_2, MMA851_I2C_DEVICE_ADDRESS, OUT_X_LSB);
                    xData  = (uint16_t)(nuevo_dato_i2c_1 << 6 | nuevo_dato_i2c_2 >> 2);
                    dxg = (xData*90)/k_SENSITIVITY_2G;
                    printf("%i ",xData);
                    printf("\t%i Grados\r\n",dxg);
                    break;
                case 'y':
                case 'Y':
                    i2c0MasterReadByte(&nuevo_dato_i2c_3, MMA851_I2C_DEVICE_ADDRESS, OUT_Y_MSB);
                    i2c0MasterReadByte(&nuevo_dato_i2c_4, MMA851_I2C_DEVICE_ADDRESS, OUT_Y_LSB);
                    yData  = (uint16_t)(nuevo_dato_i2c_3 << 6 | nuevo_dato_i2c_4 >> 2);
                    dyg = (yData*90)/k_SENSITIVITY_2G;
                    printf("%i ",yData);
                    printf("\t%i Grados\r\n",dyg);
                    break;

                case 'z':
                case 'Z':
                    i2c0MasterReadByte(&nuevo_dato_i2c_5, MMA851_I2C_DEVICE_ADDRESS, OUT_Z_MSB);
                    i2c0MasterReadByte(&nuevo_dato_i2c_6, MMA851_I2C_DEVICE_ADDRESS, OUT_Z_LSB);
                    zData  = (uint16_t)(nuevo_dato_i2c_5 << 6 | nuevo_dato_i2c_6 >> 2);
                    dzg = (zData*90)/k_SENSITIVITY_2G;
                    printf("%i ",zData);
                    printf("\t%i Grados\r\n",dzg);
                    break;


 }

            }else{
                printf("error\r\n");
            }
            }
        }
return 0 ;
            }





