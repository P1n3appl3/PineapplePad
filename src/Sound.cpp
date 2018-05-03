
#include <stdint.h>
#include "../inc/tm4c123gh6pm.h"
#include "../inc/sound.h"
#include "../inc/DAC.h"
#include "../inc/random.h"
#include "../inc/ff.h"
#include "../inc/UART.h"
#include "../inc/diskio.h"
#include "../inc/ST7735.h"
#include "../inc/UART.h"
#include "../inc/IO.h"

#define BUFFER 20000
#define SONGS 1
#define LOAD_SAMPLES 1000
#define AUDIO_FREQ (80 * 1000 * 1000 / 8000) // 8 khz

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
char songFile[] = "menu.wav";

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

void SysTick_Handler(void){
    toggle_blue();
    if (sample == tail && !paused) { // didn't load fast enough
        toggle_green();
        return;
    }
    DAC_Out(currentBuf[sample++] >> 2);
    if (sample == limit) {
        Sound_Stop();
    }
}

void SysTick_Init() {
    NVIC_ST_CTRL_R = 0;
    NVIC_ST_CURRENT_R = 0;
    NVIC_ST_RELOAD_R = AUDIO_FREQ;
    NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & NVIC_SYS_PRI3_TICK_M) | 0x00000000; // priority 0
    NVIC_ST_CTRL_R = 0x05;
}

void Sound_Init(void){
    SysTick_Init();
    DAC_Init();
}

void Effect_Play(const uint8_t* pt, uint32_t count){
    pausedSample = sample;
    pausedTail = tail;
    paused = true;
    sample = 0;
    currentBuf = (uint8_t*)pt; // casting away const
    limit = count;
    NVIC_ST_CTRL_R |= 0x02; // enable interrupt
}

void error(char* err){
    UART_OutString(err);
}

void open_song(){
    //songFile[3] = '0' + Random()%SONGS;
    sample = 0;
    tail = 0;
    Fresult = f_open(&Handle, songFile, FA_READ);
    if (Fresult) {
        error((char*)"error opening song");
    }
}

void Sound_Play(){
    open_song();
    Sound_Load();
    currentBuf = buf;
    if (!(NVIC_ST_CTRL_R & 0x02)) {
        NVIC_ST_CTRL_R |= 0x02;
    }
}

void close_file(){
    Fresult = f_close(&Handle);
    if (Fresult) {
        error((char*)"error closing file");
    }
}

void Sound_Load(){
    if ((tail < sample) && ((tail + LOAD_SAMPLES) > sample)) {
        return;
    }
    UINT samples_loaded = 0;
    if (tail + LOAD_SAMPLES > BUFFER - 1) {
        limit = tail;
        tail = 0;
        return;
    }
    Fresult = f_read(&Handle, &buf[tail], LOAD_SAMPLES, &samples_loaded);
    tail += samples_loaded;
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
