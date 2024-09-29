 /******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port_Cfg.h
 *
 * Description: Pre-Compile Configuration Header file for TM4C123GH6PM Microcontroller - Port Driver
 *
 * Author: Yahia Hamimi
 ******************************************************************************/
#ifndef PORT_CFG_H_
#define PORT_CFG_H_

/*
 * Module Version 1.0.0
 */
#define PORT_CFG_SW_MAJOR_VERSION              (1U)
#define PORT_CFG_SW_MINOR_VERSION              (0U)
#define PORT_CFG_SW_PATCH_VERSION              (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define PORT_CFG_AR_RELEASE_MAJOR_VERSION     (4U)
#define PORT_CFG_AR_RELEASE_MINOR_VERSION     (0U)
#define PORT_CFG_AR_RELEASE_PATCH_VERSION     (3U)


/* Pre-compile option for Development Error Detect */
#define PORT_DEV_ERROR_DETECT               (STD_ON)

/* Pre-compile option for Version Info API */
#define PORT_VERSION_INFO_API               (STD_ON)

/* Pre-compile option for presence of Port_SetPinDirection API */
#define PORT_SET_PIN_DIRECTION_API          (STD_ON)

/* Pre-compile option for presence of Port_SetPinMode API */
#define PORT_SET_PIN_MODE_API               (STD_ON)

/* Number of the configured Port Channels */
#define PORT_CONFIGURED_CHANNLES             (43U)


/* Tiva-c Ports Symbolic names for PB configuration structure */
#define TIVA_C_PORTA                 (0U)
#define TIVA_C_PORTB                 (1U)
#define TIVA_C_PORTC                 (2U)
#define TIVA_C_PORTD                 (3U)
#define TIVA_C_PORTE                 (4U)
#define TIVA_C_PORTF                 (5U)

/* Tiva-c Pins Symbolic names for PB configuration structure */
#define TIVA_C_PORT_PIN0                   (0U)
#define TIVA_C_PORT_PIN1                   (1U)
#define TIVA_C_PORT_PIN2                   (2U)
#define TIVA_C_PORT_PIN3                   (3U)
#define TIVA_C_PORT_PIN4                   (4U)
#define TIVA_C_PORT_PIN5                   (5U)
#define TIVA_C_PORT_PIN6                   (6U)
#define TIVA_C_PORT_PIN7                   (7U)



#endif /* PORT_CFG_H_ */
