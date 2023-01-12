#include "setting_hardaware/setting.h"
#include <stdlib.h>
#include "stdio.h"
#include "string.h"
#include "setting_hardaware/pin_manager.h"
#define _XTAL_FREQ 4000000
// using namespace std;

char str[20];
int flag = 0;

void Mode1(int time){   // Todo : Mode1 
    int succeed = 1;
    int distance;
    UART_Write_Text("You have entered GUARD mode. \r\n");
    char mes[10];
    UART_Write_Text("The Guardian will lock your phone for ");
    UART_Write_Text(itoa(mes, time, 10));
    UART_Write_Text(" seconds\r\n");
    LATD=15;
    
    __delay_ms(1000);
    CCPR1L = 30;
    __delay_ms(2000);
    int flag = 0;
    while(time > 0 && flag == 0){
        strcpy(str,GetString());
        for(int i=0;i<20;i++){
            if(str[i]=='\r'){
                str[i] = '\0';
                if(str[0]=='p' && str[1] == 'a' && str[2] == 's' && str[3] == 's' && str[4] == 'w' && str[5] == 'o' && str[6] == 'r' && str[7] == 'd' && str[8] == '\0'){ // Mode1
                    LATD = 0;
                    succeed = 0;
                    flag = 1;
                    ClearBuffer();
                    break;
                }
                else{
                    UART_Write_Text("Password incorrect\r\n");
                    ClearBuffer();
                }
            }
        }
        __delay_ms(990);
        time--;
        LATD = time % 15;
        TMR1H = 0;                //Sets the Initial Value of Timer
        TMR1L = 0;                //Sets the Initial Value of Timer
        RA2 = 1;                  //TRIGGER HIGH
        __delay_us(10);           //10uS Delay 
        RA2 = 0;                  //TRIGGER LOW
        while(!RA3);              //Waiting for Echo
        TMR1ON = 1;               //Timer Starts
        while(RA3);               //Waiting for Echo goes LOW
        TMR1ON = 0;               //Timer Stops
        distance = (TMR1L | (TMR1H<<8)); //Reads Timer Value
        distance=distance/58.82;              //Converts Time to Distance
        distance = distance + 1;    
        if(distance<5){
            UART_Write_Text("How dare you. PENALTY 10 SEC. \r\n");
            time+=10;
        }
    }
    if (succeed){
        UART_Write_Text("Challenge Complete, your phone is unlocked\r\n");
    }
    else{
        UART_Write_Text("Unlocked, try again next time\r\n");
    }
    __delay_ms(1000);
    CCPR1L = 90;
    __delay_ms(2000);
    return ;
}
void Mode2(){   // Todo : Mode2 
    int timer = 0;
    LATD=15;
    UART_Write_Text("You have entered the ZEN mode.\r\n");
    __delay_ms(1000);
    CCPR1L = 30;
    __delay_ms(2000);
    int flag = 0;
    while(!flag){
        LATD = 0;
        strcpy(str,GetString());
        for(int i=0;i<20;i++){
            if(str[i]=='\r'){
                str[i] = '\0';
                if(str[0]=='p' && str[1] == 'a' && str[2] == 's' && str[3] == 's' && str[4] == 'w' && str[5] == 'o' && str[6] == 'r' && str[7] == 'd' && str[8] == '\0'){ // Mode1
                    LATD=0;
                    flag = 1;
                    break;
                    ClearBuffer();
                }
                else{
                    UART_Write_Text("Password incorrect\r\n");
                    ClearBuffer();
                }
            }
        }
        __delay_ms(1000);
        LATD = timer % 15;
        timer++;
    }
    int min = timer / 60;
    int sec = timer % 60;
    char m[50], s[50];
    UART_Write_Text("Box is unlocked, you lasted ");
    UART_Write_Text(itoa(m, min, 10));
    UART_Write_Text(" minutes ");
    UART_Write_Text(itoa(s, sec, 10));
    UART_Write_Text(" seconds\r\n");
    __delay_ms(1000);
   CCPR1L = 90;
   __delay_ms(2000);
    return ;
}

void main(void) 
{
    SYSTEM_Initialize() ;
    
    int adc;
    TRISD = 0;
    LATD = 0;
    TRISAbits.TRISA2=0; // TRIG pin
    TRISAbits.TRISA3=1;// ECHO Pin
    
    T1CON = 0x10;
    
    while(1) {
        char intstr[10];
        if (!flag){
            UART_Write_Text("Welcome to the Guardian\r\n");
            UART_Write_Text("Press G to enter GUARD mode and Z to enter ZEN mode.\r\n");
            flag = 1;
            ClearBuffer();
        }
        strcpy(str, GetString()); // TODO : GetString() in uart.c
        for(int i=0;i<20;i++){
            if(str[i]=='\r'){
                str[i] = '\0';
                if(str[0]=='G' && str[1] == '\0'){ // Mode1
                    ClearBuffer();
                    adc= ADC_Read(60);
                    Mode1(adc);
                    ClearBuffer();
                }
                else if(str[0]=='Z' && str[1] == '\0'){ // Mode2
                    ClearBuffer();
                    Mode2();
                    ClearBuffer();
                }
                ClearBuffer();
            }
        }
    }
    return;
}

void __interrupt(high_priority) Hi_ISR(void)
{
    char intstr[12];
    int number = ADC_Read(60);
    UART_Write_Text(itoa(intstr, number, 10));
}

