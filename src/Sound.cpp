#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "../inc/Sound.h"
#include "../inc/DAC.h"
#include "../inc/random.h"
#include "../inc/ff.h"
#include "../inc/uart.h"
#include "../inc/diskio.h"
#include "../inc/ST7735.h"
#include "../inc/UART.h"
#include "../inc/IO.h"

#define BUFFER 10000
#define SONGS 1
#define LOAD_SAMPLES 1

extern "C" void SysTick_Handler(void);

static FATFS g_sFatFs;
FIL Handle;
FRESULT MountFresult;
FRESULT Fresult;

uint32_t sample = 0;
uint32_t limit = 0;
uint8_t* currentBuf;

uint8_t buf[BUFFER];
uint32_t tail;
char songFile[] = "sng0.wav";

uint32_t pausedSample;
uint32_t pausedTail;
uint32_t pausedLimit = BUFFER - 1;
bool paused;

void Sound_Unpause(){
    NVIC_ST_CTRL_R &= ~0x02; // disable interrupt
    paused = false;
    sample = pausedSample;
    tail = pausedTail;
    limit = pausedLimit;
    NVIC_ST_CTRL_R |= 0x02;
}

void Sound_Test(){
    if(sample == tail){
        return;
    }
    //toggle_blue();
    //DAC_Out(currentBuf[sample++] >> 2);
    
    UART_OutString((char*)"\n\rtail: ");
    UART_OutUDec(tail);
    UART_OutString((char*)"\n\rlimit: ");
    UART_OutUDec(limit);
    UART_OutString((char*)"\n\rsample: ");
    UART_OutUDec(sample);
    UART_OutChar(' ');
    UART_OutUDec(currentBuf[sample++]);
    
    //if(currentBuf[sample++] > 0){
      //  toggle_blue();
    //}
    if (sample == limit) {
        if(paused){
            Sound_Unpause();
        }
        else{
            sample = 0;
        }
    }
}

void SysTick_Handler(void){
    toggle_blue();
    if(sample == tail){
        return;
    }
    //DAC_Out(currentBuf[sample++] >> 2);
    /*
    UART_OutString((char*)"\n\rtail: ");
    UART_OutUDec(tail);
    UART_OutString((char*)"\n\rlimit: ");
    UART_OutUDec(limit);
    UART_OutString((char*)"\n\rsample: ");
    UART_OutUDec(sample);
    */
   // UART_OutChar('\n');
    //UART_OutChar('\r');
    //UART_OutUDec(currentBuf[sample++]);
    if (sample == limit) {
        if(paused){
            Sound_Unpause();
        }
        else{
            sample = 0;
        }
    }
}

void SysTick_Init() {
    NVIC_ST_CTRL_R = 0;
    NVIC_ST_CURRENT_R = 0;
    NVIC_ST_RELOAD_R = 0;
    NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & NVIC_SYS_PRI3_TICK_M) | 0x00000000; // priority 0
    NVIC_ST_CTRL_R = 0x07;
}

void Sound_Init(void){
    SysTick_Init();
    //DAC_Init();
}

#define AUDIO_FREQ (80 * 1000 * 1000 / 8000) // 8 khz
void Effect_Play(const uint8_t* pt, uint32_t count){
    pausedSample = sample;
    pausedTail = tail;
    paused = true;
    sample = 0;
    currentBuf = (uint8_t*)pt; // casting away const
    limit = count;
    NVIC_ST_RELOAD_R = AUDIO_FREQ;
    NVIC_ST_CTRL_R |= 0x02; // enable interrupt
}

void error(char *err){
    UART_OutString(err);
}

void open_song(){
    songFile[3] = '0' + Random()%SONGS;
    sample = 0;
    tail = 0;
    Fresult = f_open(&Handle, songFile, FA_READ);
    if(Fresult){
        error((char*)"error opening song");
    }
    else{
        //error((char*)"opened song");
    }
}

void Sound_Play(){
    open_song();
    Sound_Load();
    currentBuf = buf;
    NVIC_ST_RELOAD_R = 0xFFFF;
}

void close_file(){
    Fresult = f_close(&Handle);
    if(Fresult){
        error((char*)"error closing file");
    }
}

void Sound_Load(){
    if((tail < sample) && ((tail + LOAD_SAMPLES) > sample)){
        return;
    }
    UINT samples_loaded = 0;
    if(tail + LOAD_SAMPLES > BUFFER-1){
        limit = tail;
        tail = 0;
        return;
    }
    Fresult = f_read(&Handle, &buf[tail], LOAD_SAMPLES, &samples_loaded);
    if(samples_loaded < LOAD_SAMPLES){
        close_file();
        open_song();
        sample = 0;
        tail = 0;
        limit = BUFFER - 1;
    }
    tail += samples_loaded;
    if(!(NVIC_ST_CTRL_R & 0x02)){
        NVIC_ST_CTRL_R |= 0x02;
    }
}

void SD_Mount(){
    MountFresult = f_mount(&g_sFatFs, "", 0);
    if (MountFresult) {
        error((char*)"error mounting sd card");
    }
}

void Sound_Stop(){
    NVIC_ST_CTRL_R &= ~0x02; // disable interrupt
}

const char inFilename[] = "lor.txt";   // 8 characters or fewer

void sd_test(){
    UINT successfulreads;
    uint8_t c, x, y;
    ST7735_FillScreen(0);               // set screen to black

    MountFresult = f_mount(&g_sFatFs, "", 0);
    if (MountFresult) {
        ST7735_DrawString(0, 0, "f_mount error", ST7735_Color565(0, 0, 255));
        while (1);
    }

    Fresult = f_open(&Handle, inFilename, FA_READ);
    if (Fresult == FR_OK) {
        ST7735_DrawString(0, 0, (char*)"Opened ", ST7735_Color565(0, 255, 0));
        ST7735_DrawString(7, 0, (char*)inFilename, ST7735_Color565(0, 255, 0));
        //char testbuf[46];
        //Fresult = f_read(&Handle, &testbuf, 46, &successfulreads);
        //ST7735_OutString(testbuf);
        //get a character in 'c' and the number of successful reads in 'successfulreads'
      Fresult = f_read(&Handle, &c, 1, &successfulreads);
        x = 0;                           //start in the first column
        y = 10;                          //start in the second row
        while ((Fresult == FR_OK) && (successfulreads == 1)) {                   //the character is printable, so print it
            if(c == '\n'){
                x = 0;                          // go to the first column (this seems implied)
                y = y + 10;                     // go to the next row
            } else if(c == '\r'){
                x = 0;                          // go to the first column
            } else{                           // the character is printable, so print it
                ST7735_DrawChar(x, y, c, ST7735_Color565(255, 255, 255), 0, 1);
                x = x + 6;                      // go to the next column
                if(x > 122){                    // reached the right edge of the screen
                    x = 0;                        // go to the first column
                    y = y + 10;                   // go to the next row
                }
            }
            //get the next character in 'c'
            Fresult = f_read(&Handle, &c, 1, &successfulreads);
        }
        ST7735_SetCursor(16, 0);
        ST7735_OutUDec(c);
        //close the file
        Fresult = f_close(&Handle);
    } else{
    //print the error code
        ST7735_DrawString(0, 0, (char*)"Error          (  )", ST7735_Color565(255, 0, 0));
        ST7735_DrawString(6, 0, (char*)inFilename, ST7735_Color565(255, 0, 0));
        ST7735_SetCursor(16, 0);
        ST7735_SetTextColor(ST7735_Color565(255, 0, 0));
        ST7735_OutUDec((uint32_t)Fresult);
    }
}
