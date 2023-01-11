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
    int bg_time=time/4;
    int a;
    char mes[10];
    UART_Write_Text("The Guardian will lock your phone for ");
    UART_Write_Text(itoa(mes, time, 10));
    UART_Write_Text(" seconds\r\n");
    LATD=15;
    
    __delay_ms(1000);
    CCPR1L = 30;
    __delay_ms(2000);
    while(time > 0){
        strcpy(str,GetString());
        if(strcmp(str,"a\r")==0){
            succeed = 0;
            break;
        }
        __delay_ms(1000);
        //char mes[50] = "One second passed. \r";
        //UART_Write_Text(mes);
        //UART_Write('\n');
        time--;
        
        if(time/bg_time==2)
        {
            LATDbits.LD3=0;
        }
        else if(time/bg_time==1)
        {
            LATDbits.LD2=0;
        }
        else if(time==0)
        {
            LATDbits.LD0=0;
        }
        else if(time/bg_time==0)
        {
            LATDbits.LD1=0;
        }
        
        TMR1H = 0;                //Sets the Initial Value of Timer
        TMR1L = 0;                //Sets the Initial Value of Timer
        RA2 = 1;                  //TRIGGER HIGH
        __delay_us(10);           //10uS Delay 
        RA2 = 0;                  //TRIGGER LOW
        
        while(!RA3);              //Waiting for Echo
        TMR1ON = 1;               //Timer Starts
        while(RA3);               //Waiting for Echo goes LOW
        TMR1ON = 0;               //Timer Stops
        a = (TMR1L | (TMR1H<<8)); //Reads Timer Value
        a=a/58.82;              //Converts Time to Distance
        a = a + 1;    
        
        if(a<5){
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
    UART_Write_Text("You have entered the zen mode.\r\n");
    __delay_ms(1000);
    CCPR1L = 30;
    __delay_ms(2000);
    while(1){
        strcpy(str,GetString());
        if(strcmp(str,"password\r")==0){
            LATD=0;
            break;
        }
        __delay_ms(1000);
        //char mes[50] = "One second passed. \r";
        //UART_Write_Text(mes);
        //UART_Write('\n');
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
    
//    PORTD=1;
//    TRISDbits.RD0=0;
//    TRISDbits.RD1=0;
//    TRISDbits.RD2=0;
//    TRISDbits.RD3=0;
   
    SYSTEM_Initialize() ;
    //ADCON1=15;
    int adc;
    int a;
    char tmp[10];
    TRISD = 0;
    LATD = 0;
    TRISAbits.TRISA2=0; // TRIG pin
    TRISAbits.TRISA3=1;// ECHO Pin
    
    T1CON = 0x10;
    

    while(1) {
        char intstr[10];
        //int adc = 10;     
         
        //UART_Write_Text(itoa(tmp, adc , 10));
        //__delay_ms(1000);
        //UART_Write('\n');
                   //Distance Calibration
          
        
        
        if (!flag){
            char mes[] = "Welcome to the Guardian\r";
            UART_Write_Text(mes);
            UART_Write('\n');
            flag = 1;
        }
        //strcpy(str, GetString()); // TODO : GetString() in uart.c
        strcpy(str,GetString());

        if(strcmp(str,"G\r")==0){
            ClearBuffer();
            UART_Write('\n');
            adc= ADC_Read(60);
            Mode1(adc);
            ClearBuffer();
        }
        else if(strcmp(str,"Z\r")==0){
            ClearBuffer();
            UART_Write('\n');
            Mode2();
            ClearBuffer();
        }
        else if(strcmp(str,"\r")==0){
            ClearBuffer();
            UART_Write('\n');
            ClearBuffer();
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

