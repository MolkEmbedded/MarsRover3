

void TWI_InitSlave(void);
void pwmInit();
void dataFunction(unsigned char);
void motorAuto(float, float);

#define Motor1F OCR0A 
#define Motor1R OCR0B 
#define Motor2F OCR2A
#define Motor2R OCR2B
