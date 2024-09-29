 /******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port.h
 *
 * Description: Header file for TM4C123GH6PM Microcontroller - Port Driver
 *
 * Author: Yahia Hamimi
 ******************************************************************************/

#ifndef PORT_H_
#define PORT_H_

/* Id for the company in the AUTOSAR
 * for example yahia's ID = 1000 :) */
#define PORT_VENDOR_ID    (1000U)

/* PORT Module Id */
#define PORT_MODULE_ID    (124U)

/* PORT Instance Id */
#define PORT_INSTANCE_ID  (0U)

/*
 * Module Version 1.0.0
 */
#define PORT_SW_MAJOR_VERSION           (1U)
#define PORT_SW_MINOR_VERSION           (0U)
#define PORT_SW_PATCH_VERSION           (0U)

/*
 * AUTOSAR Version 4.0.3
 */
#define PORT_AR_RELEASE_MAJOR_VERSION   (4U)
#define PORT_AR_RELEASE_MINOR_VERSION   (0U)
#define PORT_AR_RELEASE_PATCH_VERSION   (3U)

/*
 * Macros for PORT Status
 */
#define PORT_INITIALIZED                (1U)
#define PORT_NOT_INITIALIZED            (0U)


/* Standard AUTOSAR types */
#include "Std_Types.h"

/* AUTOSAR checking between Std Types and PORT Modules */
#if ((STD_TYPES_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 ||  (STD_TYPES_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 ||  (STD_TYPES_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Std_Types.h does not match the expected version"
#endif

/* PORT Pre-Compile Configuration Header file */
#include "Port_Cfg.h"



/* AUTOSAR Version checking between Port_Cfg.h and Port.h files */
#if ((PORT_CFG_AR_RELEASE_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 ||  (PORT_CFG_AR_RELEASE_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 ||  (PORT_CFG_AR_RELEASE_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Port_Cfg.h does not match the expected version"
#endif


/* Software Version checking between Port_Cfg.h and Port.h files */
#if ((PORT_CFG_SW_MAJOR_VERSION != PORT_SW_MAJOR_VERSION)\
 ||  (PORT_CFG_SW_MINOR_VERSION != PORT_SW_MINOR_VERSION)\
 ||  (PORT_CFG_SW_PATCH_VERSION != PORT_SW_PATCH_VERSION))
  #error "The SW version of Port_Cfg.h does not match the expected version"
#endif

/* Non AUTOSAR files includes */
#include "Common_Macros.h"

/******************************************************************************
 *                      API Service Id Macros                                 *
 ******************************************************************************/
/* Service ID for Port Init */
#define PORT_INIT_SID           (uint8)0x00

/* Service ID for Port Set Pin Direction */
#define PORT_SET_PIN_DIRECTION_SID     (uint8)0x01

/* Service ID for Port Refresh Port Direction */
#define PORT_REFREASH_PORT_DIRECTION_SID   (uint8)0x02

/* Service ID for Port Get Version Info */
#define PORT_GET_VERSION_INFO_SID            (uint8)0x03

/* Service ID for Port Set Pin Mode */
#define PORT_SET_PIN_MODE_SID     (uint8)0x04



/*******************************************************************************
 *                      DET Error Codes                                        *
 *******************************************************************************/
/* DET code to report Invalid Port Pin ID */
#define PORT_E_PARAM_PIN                 (uint8)0x0A

/*  DET code to report Port Pin not configured as changeable */
#define PORT_E_DIRECTION_UNCHANGEABLE            (uint8)0x0B

/* API Port_Init service called with wrong parameter */
#define PORT_E_PARAM_CONFIG   (uint8)0x0C

/* API Port_SetPinMode service called when mode is unchangeable */
#define PORT_E_PARAM_INVALID_MODE     (uint8)0x0D
#define PORT_E_MODE_UNCHANGEABLE     (uint8)0x0E




/*
 * API service used without module initialization is reported using following
 * error code
 */
#define PORT_E_UNINIT                   (uint8)0x0F


/*
 * The API service shall return immediately without any further action,
 * beside reporting this development error.
 */
#define PORT_E_PARAM_POINTER             (uint8)0x10







/*******************************************************************************
 *                              Module Data Types                              *
 *******************************************************************************/


/* Type definition for Port_PinType used by the PORT APIs */
typedef uint8 Port_PinType;

/* Type definition for Port_PinDirectionType used by the PORT APIs */
typedef enum
{
    PORT_PIN_IN,
    PORT_PIN_OUT
}Port_PinDirectionType;

/* Type definition for Port_InternalResistor used by the Pin config type structure
 * Description: Holds the value of internal resistor type for PIN
 */
typedef enum
{
    OFF,PULL_UP,PULL_DOWN
}Port_InternalResistor;

/* Type definition for Pin_DirectionChangeable used by the Pin config type structure
 * Description: Holds the value of direction changing ability for PIN
 */
typedef enum
{
    DIRECTION_NOT_CHANGEABLE,DIRECTION_CHANGEABLE
}Pin_DirectionChangeable;

/* Type definition for Pin_ModeChangeable used by the Pin config type structure
 * Description: Holds the value of mode changing ability for PIN
 */
typedef enum
{
    MODE_NOT_CHANGEABLE,MODE_CHANGEABLE
}Pin_ModeChangeable;

/* Type definition for Port_PinModeType */

typedef enum
{
    PORT_PIN_DIO_MODE,
    PORT_PIN_ALTFUNC_1_MODE_,
    PORT_PIN_ALTFUNC_2_MODE_,
    PORT_PIN_ALTFUNC_3_MODE_,
    PORT_PIN_ALTFUNC_4_MODE_,
    PORT_PIN_ALTFUNC_5_MODE_,
    PORT_PIN_ALTFUNC_6_MODE_,
    PORT_PIN_ALTFUNC_7_MODE_,
    PORT_PIN_ALTFUNC_8_MODE_,
    PORT_PIN_ALTFUNC_9_MODE_,
    PORT_PIN_ALTFUNC_14_MODE_ = 14 ,
    PORT_PIN_ADC_MODE

}Port_PinModeType;

/* Description: Structure to configure each individual PIN:
 *  1. The PORT Which the pin belongs to --> 0, 1, 2, 3, 4 or 5
 *  2. The number of the pin in the PORT --> Range [0-39]
 *  3. The direction of pin --> INPUT or OUTPUT
 *  4. The internal resistor --> Disable, Pull up or Pull down
 *  5. The alternate mode of pin
 *  6. The ability of changing pin direction during run time --> STD_ON, STD_OFF
 *  7. The ability of changing pin mode during run time --> STD_ON, STD_OFF
 *  8. The initial value of pin --> STD_HIGH, STD_LOW
 */
typedef struct
{
       uint8 Port_num;
       Port_PinType Pin_num;
       Port_PinDirectionType Direction;
       Port_InternalResistor Resistor;
       Port_PinModeType Mode;
       Pin_DirectionChangeable Direction_Change_Ability;
       Pin_ModeChangeable Mode_Change_Ability;
       uint8 Initial_value;
} Pin_ConfigType_Options;

/* Data Structure required for initializing the PORT Driver Containing array of structure for each pin */
typedef struct
{
 Pin_ConfigType_Options Pin_Configuration[PORT_CONFIGURED_CHANNLES];
} Port_ConfigType;


/*******************************************************************************
 *                      Function Prototypes                                    *
 *******************************************************************************/


/************************************************************************************
* Service Name: Port_Init
* Sync/Async: Synchronous
* Reentrancy: Non Reentrant
* Parameters (in): ConfigPtr - Pointer to post-build configuration data
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to Initialize the Port Driver module
* by looping on each individual pin then:
*              - Setup the pin initial value
*              - Setup the direction of  pin
*              - Setup the internal resistor for i/p pin
*              - Setup the mode for pin
*              - Setup the pin direction change ability
*              - Setup the pin mode change ability
*
*
************************************************************************************/

void Port_Init( const Port_ConfigType* ConfigPtr );

/************************************************************************************
* Service Name: Port_SetPinDirection
* Sync/Async: Synchronous
* Reentrancy: Reentrant
* Parameters (in): Pin -->Port Pin ID number
*                  Direction--> Port Pin Direction
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to Set the port pin direction
*
*
************************************************************************************/

void Port_SetPinDirection( Port_PinType Pin, Port_PinDirectionType Direction );

/************************************************************************************
* Service Name: Port_RefreshPortDirection
* Sync/Async: Synchronous
* Reentrancy: Non Reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out): None
* Return value: None
* Description: Function to Refresh port direction.
*
*
************************************************************************************/

void Port_RefreshPortDirection(void);

/************************************************************************************
* Service Name: Port_GetVersionInfo
* Sync/Async: Synchronous
* Reentrancy: Non Reentrant
* Parameters (in): None
* Parameters (inout): None
* Parameters (out):versioninfo-->Pointer to where to
* store the version information of this module.
* Return value: None
* Description: Function to Refresh port direction.
*
*
************************************************************************************/

void Port_GetVersionInfo( Std_VersionInfoType* versioninfo);


/************************************************************************************
* Service Name: Port_SetPinMode
* Sync/Async: Synchronous
* Reentrancy: Non Reentrant
* Parameters (in): Pin -->Port Pin ID number
*                  Mode--> New Port Pin mode to be set on port pin
* Parameters (inout): None
* Parameters (out):None
* store the version information of this module.
* Return value: None
* Description: Function Sets the port pin mode
*
*
************************************************************************************/
void Port_SetPinMode( Port_PinType Pin, Port_PinModeType Mode);

/*******************************************************************************
 *                       External Variables                                    *
 *******************************************************************************/
/* Extern PB structures to be used by PORT and other modules */
extern const Port_ConfigType Port_Configuration;

#endif /* PORT_H_ */
