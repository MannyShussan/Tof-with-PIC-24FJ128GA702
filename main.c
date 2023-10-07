/**
  Generated main.c file from MPLAB Code Configurator

  @Company
    Microchip Technology Inc.

  @File Name
    main.c

  @Summary
    This is the generated main.c using PIC24 / dsPIC33 / PIC32MM MCUs.

  @Description
    This source file provides main entry point for system initialization and application code development.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.171.2
        Device            :  PIC24FJ128GA702
    The generated drivers are tested against the following:
        Compiler          :  XC16 v2.10
        MPLAB 	          :  MPLAB X v6.05
 */

/*
    (c) 2020 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
 */

/**
  Section: Included Files
 */
#include "mcc_generated_files/system.h"
#define FCY 8000000UL 
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <libpic30.h>
#include <xc.h>
#include <string.h>
#include <math.h>
#include "vl53l1cx.h"
#include "math.h"


void print(const char string[]);
void printNum(const int temp);
void printNum3digitos(int vol);
int ADC_converter();
float Get_temperature();
unsigned int Get_distance();
long map(long x, long in_min, long in_max, long out_min, long out_max);
unsigned int converteVolume(long mm);
void printResult(unsigned int TEMP, unsigned int TANQUE, unsigned int DIST);

int main(void) {
    SYSTEM_Initialize(); // initializa o dispositivo
    VL53L1CX_Init(); // inicializa o sensor

    __delay_ms(500);
    PORTBbits.RB7 = 0;
    PORTBbits.RB4 = 1;
    while (1) {
        unsigned int temperatura_C = (int) Get_temperature();
        unsigned int distancia_mm = (int) Get_distance();
        unsigned int distancia_bin = (int) converteVolume(distancia_mm);

        printResult(temperatura_C, distancia_bin, distancia_mm);
        __delay_ms(200);
        //PORTBbits.RB13 = ~PORTBbits.RB13;
    }

    return 1;
}

/**
 End of File
 */

void print(const char string[]) {
    for (char l = 0; l < strlen(string) && l < 20; l++)UART1_Write(string[l]);
}

void printNum(const int temp) {
    unsigned int num [4] = {0, 0, 0, 0};
    num[0] = (temp / 1000) + 48;
    num[1] = ((temp % 1000) / 100) + 48;
    num[2] = ((temp % 100) / 10) + 48;
    num[3] = (temp % 10) + 48;
    for (unsigned char i = 0; i < 4; i++)UART1_Write(num[i]);
}

void printNum3digitos(int vol) {
    unsigned int volume [3] = {0, 0, 0};
    volume[0] = (vol / 100) + 48;
    volume[1] = ((vol % 100) / 10) + 48;
    volume[2] = (vol % 10) + 48;
    for (unsigned char i = 0; i < 3; i++)UART1_Write(volume[i]);
}

int ADC_converter() {
    int conversion;
    ADC1_Initialize();

    ADC1_Enable();
    ADC1_ChannelSelect(NTC);
    ADC1_SoftwareTriggerEnable();
    //Provide Delay
    for (int i = 0; i < 1000; i++);
    ADC1_SoftwareTriggerDisable();
    while (!ADC1_IsConversionComplete(NTC));
    conversion = ADC1_ConversionResultGet(NTC);
    ADC1_Disable();
    return conversion;
}

float Get_temperature() {
    //#define B_coefficient 3950.00      /* B coefficient of NTC Thermistor*/
#define B_coefficient 7700.00      /* B coefficient of NTC Thermistor*/
    //#define Room_temperature 26.00
#define Room_temperature 21.00
#define Series_Resistance 10000.00

    long NTC_Resistance;
    int analog_value;

    float Thermistor_temperature;
    analog_value = ADC_converter(); /* store adc value on val register */

    /* calculate the NTC resistance */
    NTC_Resistance = (analog_value * 0.0032258064516)*(Series_Resistance / (3.3 - (analog_value * 0.0032258064516)));
    Thermistor_temperature = log(NTC_Resistance); /* calculate natural log of resistance */

    /* Calculate Temperature using B parameter Equation */
    /* 1/T = 1/T0 + ((1/B_coefficient)*log(NTC_Resistance/Series_Resistance)) */
    Thermistor_temperature = (1.0 / (Room_temperature + 273.15))+(1.0 / B_coefficient) * log(NTC_Resistance / Series_Resistance);
    Thermistor_temperature = (1 / Thermistor_temperature) - 273.15; /* convert kelvin to °C */

    return (Thermistor_temperature * 10);
}

long map(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

unsigned int converteVolume(long mm) {
#define valorTanqueCheio 0
#define valorTanqueVazio 900
#define nivelZero 300  // abaixo desse valor ele apresenta 255 na serial
    if (mm > valorTanqueVazio) mm = valorTanqueVazio;
    if (mm >= (valorTanqueVazio - nivelZero))return 255;
    else return (int) map(mm, valorTanqueVazio, valorTanqueCheio, 0, 4);
}

void printResult(unsigned int TEMP, unsigned int TANQUE, unsigned int DIST) {
    print("S\r\nT");
    printNum(TEMP);
    print("\r\n");
    // para imprimir no terminal, é só descomentar essas 4 linhas
    //print("Dist: "); // linha 1
    //printNum(DIST); // linha 2
    //print(" Bin: "); // linha 3
    printNum3digitos(TANQUE);
    //print("\r\n"); // linha 4
    print("-025\r\n"); //comentar essa linha durante a calibração
}

unsigned int Get_distance() {
    return (int) VL53L1CX_Millimeter_GetValue();
}