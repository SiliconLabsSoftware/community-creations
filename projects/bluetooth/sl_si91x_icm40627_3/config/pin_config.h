#ifndef PIN_CONFIG_H
#define PIN_CONFIG_H

// $[USART0]
// [USART0]$

// $[UART1]
// [UART1]$

// $[ULP_UART]
// [ULP_UART]$

// $[I2C0]
// [I2C0]$

// $[I2C1]
// [I2C1]$

// $[ULP_I2C]
// [ULP_I2C]$

// $[SSI_MASTER]
// [SSI_MASTER]$

// $[SSI_SLAVE]
// [SSI_SLAVE]$

// $[ULP_SSI]
// ULP_SSI MOSI_ on ULP_GPIO_1/GPIO_65
#ifndef ULP_SSI_MOSI__PORT                      
#define ULP_SSI_MOSI__PORT                       ULP
#endif
#ifndef ULP_SSI_MOSI__PIN                       
#define ULP_SSI_MOSI__PIN                        1
#endif
#ifndef ULP_SSI_MOSI_LOC                        
#define ULP_SSI_MOSI_LOC                         0
#endif

// ULP_SSI MISO_ on ULP_GPIO_2/GPIO_66
#ifndef ULP_SSI_MISO__PORT                      
#define ULP_SSI_MISO__PORT                       ULP
#endif
#ifndef ULP_SSI_MISO__PIN                       
#define ULP_SSI_MISO__PIN                        2
#endif
#ifndef ULP_SSI_MISO_LOC                        
#define ULP_SSI_MISO_LOC                         12
#endif

// ULP_SSI SCK_ on ULP_GPIO_8/GPIO_72
#ifndef ULP_SSI_SCK__PORT                       
#define ULP_SSI_SCK__PORT                        ULP
#endif
#ifndef ULP_SSI_SCK__PIN                        
#define ULP_SSI_SCK__PIN                         8
#endif
#ifndef ULP_SSI_SCK_LOC                         
#define ULP_SSI_SCK_LOC                          7
#endif

// ULP_SSI CS0_ on ULP_GPIO_10/GPIO_74
#ifndef ULP_SSI_CS0__PORT                       
#define ULP_SSI_CS0__PORT                        ULP
#endif
#ifndef ULP_SSI_CS0__PIN                        
#define ULP_SSI_CS0__PIN                         10
#endif
#ifndef ULP_SSI_CS0_LOC                         
#define ULP_SSI_CS0_LOC                          9
#endif

// [ULP_SSI]$

// $[GSPI_MASTER]
// [GSPI_MASTER]$

// $[I2S0]
// [I2S0]$

// $[ULP_I2S]
// [ULP_I2S]$

// $[SCT]
// [SCT]$

// $[SIO]
// [SIO]$

// $[PWM]
// [PWM]$

// $[PWM_CH0]
// [PWM_CH0]$

// $[PWM_CH1]
// [PWM_CH1]$

// $[PWM_CH2]
// [PWM_CH2]$

// $[PWM_CH3]
// [PWM_CH3]$

// $[ADC_CH1]
// [ADC_CH1]$

// $[ADC_CH2]
// [ADC_CH2]$

// $[ADC_CH3]
// [ADC_CH3]$

// $[ADC_CH4]
// [ADC_CH4]$

// $[ADC_CH5]
// [ADC_CH5]$

// $[ADC_CH6]
// [ADC_CH6]$

// $[ADC_CH7]
// [ADC_CH7]$

// $[ADC_CH8]
// [ADC_CH8]$

// $[ADC_CH9]
// [ADC_CH9]$

// $[ADC_CH10]
// [ADC_CH10]$

// $[ADC_CH11]
// [ADC_CH11]$

// $[ADC_CH12]
// [ADC_CH12]$

// $[ADC_CH13]
// [ADC_CH13]$

// $[ADC_CH14]
// [ADC_CH14]$

// $[ADC_CH15]
// [ADC_CH15]$

// $[ADC_CH16]
// [ADC_CH16]$

// $[ADC_CH17]
// [ADC_CH17]$

// $[ADC_CH18]
// [ADC_CH18]$

// $[ADC_CH19]
// [ADC_CH19]$

// $[COMP1]
// [COMP1]$

// $[COMP2]
// [COMP2]$

// $[DAC0]
// [DAC0]$

// $[DAC1]
// [DAC1]$

// $[SYSRTC]
// [SYSRTC]$

// $[UULP_VBAT_GPIO]
// [UULP_VBAT_GPIO]$

// $[GPIO]
// [GPIO]$

// $[QEI]
// [QEI]$

// $[HSPI_SECONDARY]
// [HSPI_SECONDARY]$

// $[CUSTOM_PIN_NAME]
#ifndef _PORT                                   
#define _PORT                                    HP
#endif
#ifndef _PIN                                    
#define _PIN                                     6
#endif

// [CUSTOM_PIN_NAME]$

#endif // PIN_CONFIG_H// $[SDC_CH1]
// [SDC_CH1]$

// $[SDC_CH2]
// [SDC_CH2]$

// $[SDC_CH3]
// [SDC_CH3]$

// $[SDC_CH4]
// [SDC_CH4]$

// $[OPAMP1]
// [OPAMP1]$

// $[OPAMP2]
// [OPAMP2]$

// $[OPAMP3]
// [OPAMP3]$

