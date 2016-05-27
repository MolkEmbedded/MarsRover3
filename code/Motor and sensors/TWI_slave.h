

void TWI_InitSlave(void);
void pwmInit();
void dataFunction(unsigned char);
void motorAuto(float, float);

#define RightF OCR0A 
#define RightR OCR0B 
#define LeftF OCR2A
#define LeftR OCR2B

#define Motor1F OCR0A
#define Motor1R OCR0B
#define Motor2F OCR2A
#define Motor2R OCR2B

#define leftSpeed 100
#define rightSpeed 100