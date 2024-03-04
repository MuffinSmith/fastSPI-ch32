#include <SPI.h>
#define LED_PIN D5
#define COLD_SIG D12
#define CS D6 //chip select AD5662

#define PERIPH_BASE                             ((uint32_t)0x40000000) /* Peripheral base address in the alias region */
#define APB2PERIPH_BASE                         (PERIPH_BASE + 0x10000)
#define SPI1_BASE                               (APB2PERIPH_BASE + 0x3000)
#define SPI1                                    ((SPI_TypeDef *)SPI1_BASE)

#define SPI_MODULE_ENABLED
const PinMap PinMap_SPI_SCLK[] = {
{PC_5, SPI1, CH_PIN_DATA(CH_MODE_OUTPUT_50MHz, CH_CNF_OUTPUT_AFPP, 0, AFIO_NONE)},
{NC, NP, 0}
};

const PinMap PinMap_SPI_MOSI[] = {
{PC_6, SPI1, CH_PIN_DATA(CH_MODE_OUTPUT_50MHz, CH_CNF_OUTPUT_AFPP, 0, AFIO_NONE)},
{NC, NP, 0}
};

const PinMap PinMap_SPI_MISO[] = {
{PC_7, SPI1, CH_PIN_DATA(CH_MODE_INPUT, CH_CNF_INPUT_FLOAT, 0, AFIO_NONE)},
{NC, NP, 0}
};

const PinMap PinMap_SPI_SSEL[] = {
{PC_1, SPI1, CH_PIN_DATA(CH_MODE_OUTPUT_50MHz, CH_CNF_OUTPUT_AFPP, 0, AFIO_NONE)},
{NC, NP, 0}
};

uint16_t dacV = 32767;

void setup() {
  pinMode(CS, OUTPUT); // init DAC chip select
  digitalWrite(CS, HIGH);
  SPI.beginTransaction(SPISettings(24000000, MSBFIRST, SPI_MODE3));
  SPI.begin();
  digitalWrite(CS,LOW);
  
  // SPI1->DATAR = 0;

}

void loop() {
  delay(20);
  writeDAC(32767);

}

void writeDAC(int16_t data){
  unsigned char dataBuf[2] = {
    (data & 0x0000FF00) >> 8,
    (data & 0x000000FF)};

  while((SPI1->STATR & ((uint16_t)0x0002)) == (uint16_t)RESET);
  SPI1->DATAR = 0;
  while((SPI1->STATR & ((uint16_t)0x0002)) == (uint16_t)RESET);
  SPI1->DATAR = dataBuf[0];
  while((SPI1->STATR & ((uint16_t)0x0002)) == (uint16_t)RESET);
  SPI1->DATAR = dataBuf[1];  
}
