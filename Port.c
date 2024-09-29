
/******************************************************************************
 *
 * Module: Port
 *
 * File Name: Port.c
 *
 * Description: Source file for TM4C123GH6PM Microcontroller - Port Driver.
 *
 * Author: Yahia Hamimi
 ******************************************************************************/

#include "Port.h"
#include "Port_Regs.h"

#if (PORT_DEV_ERROR_DETECT == STD_ON)

#include "Det.h"
/* AUTOSAR Version checking between Det and Port Modules */
#if ((DET_AR_MAJOR_VERSION != PORT_AR_RELEASE_MAJOR_VERSION)\
 || (DET_AR_MINOR_VERSION != PORT_AR_RELEASE_MINOR_VERSION)\
 || (DET_AR_PATCH_VERSION != PORT_AR_RELEASE_PATCH_VERSION))
  #error "The AR version of Det.h does not match the expected version"
#endif

#endif

/* status of the Port whether:
 *  (1) PORT_NOT_INITIALIZED
 *  (2) PORT_INITIALIZED        ( set by Port_Init() )
 */
STATIC uint8 Port_Status = PORT_NOT_INITIALIZED;

/* pointer to be used to point to every pin structure in the port configuration structure */
STATIC const Pin_ConfigType_Options *Pin_Ptr = NULL_PTR;

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

void Port_Init(const Port_ConfigType *ConfigPtr)
{

#if (PORT_DEV_ERROR_DETECT == STD_ON)
    /* check if the input configuration pointer is not a NULL_PTR */
    if (ConfigPtr == NULL_PTR)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_INIT_SID, PORT_E_PARAM_CONFIG);

    }
    else
    {
        /* Do Nothing */
    }
#endif

    /*
     * Set the module state to initialized and point to the first structure in the array of pins structure in PB configuration structure using a global pointer.
     * This global pointer is global to be used by other functions to read the PB configuration structures
     */
    Port_Status = PORT_INITIALIZED;
    Pin_Ptr = ConfigPtr->Pin_Configuration; /* address of the first pin structure --> Pin_Configuration[0] */

    volatile uint32 *PortGpio_Ptr = NULL_PTR; /* point to the required Port Registers base address */

    uint8 Pin_index;

    /* looping for each pin structure of configuration in the port PB general configuration structure */
    for (Pin_index = 0; Pin_index < PORT_CONFIGURED_CHANNLES; Pin_index++)
    {

        //PortGpio_Ptr = NULL_PTR; /* Erase the port_ptr value each iteration */

        switch (Pin_Ptr[Pin_index].Port_num)
        {
        case 0:
            PortGpio_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
            break;
        case 1:
            PortGpio_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
            break;
        case 2:
            PortGpio_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
            break;
        case 3:
            PortGpio_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
            break;
        case 4:
            PortGpio_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
            break;
        case 5:
            PortGpio_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
            break;
        }

        if (((Pin_Ptr[Pin_index].Port_num == 3) && (Pin_Ptr[Pin_index].Pin_num == 7)) || ((Pin_Ptr[Pin_index].Port_num == 5) && (Pin_Ptr[Pin_index].Pin_num == 0))) /* PD7 or PF0 */
        {
            /* Unlock the GPIOCR register */
            *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_LOCK_REG_OFFSET) = 0x4C4F434B;

            /* Set the corresponding bit in GPIOCR register to allow changes on this pin */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_COMMIT_REG_OFFSET), Pin_Ptr[Pin_index].Pin_num);
        }
        else if ((Pin_Ptr[Pin_index].Port_num == 2) && (Pin_Ptr[Pin_index].Pin_num <= 3)) /* PC0 to PC3 */
        {
            /* Do Nothing ...  this is the JTAG pins */
            continue;
        }
        else
        {
            /* Do Nothing ... No need to unlock the commit register for this pin */
        }

        /********************************_Configuring Direction_***********************************************/

        if (Pin_Ptr[Pin_index].Direction == PORT_PIN_OUT)
        {
            /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET), Pin_Ptr[Pin_index].Pin_num);
            if (Pin_Ptr[Pin_index].Initial_value == STD_HIGH)
            {
                /* Set the corresponding bit in the GPIODATA register to provide initial value 1 */
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DATA_REG_OFFSET), Pin_Ptr[Pin_index].Pin_num);
            }
            else
            {
                /* Clear the corresponding bit in the GPIODATA register to provide initial value 0 */
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DATA_REG_OFFSET), Pin_Ptr[Pin_index].Pin_num);
            }
        }

        /********************************_Configuring Internal Resistors_***********************************************/
        else if (Pin_Ptr[Pin_index].Direction == PORT_PIN_IN)
        {
            /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET), Pin_Ptr[Pin_index].Pin_num);

            if (Pin_Ptr[Pin_index].Resistor == PULL_UP)
            {
                /* Set the corresponding bit in the GPIOPUR register to enable the internal pull up pin */
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_UP_REG_OFFSET), Pin_Ptr[Pin_index].Pin_num);
            }
            else if (Pin_Ptr[Pin_index].Resistor == PULL_DOWN)
            {
                /* Set the corresponding bit in the GPIOPDR register to enable the internal pull down pin */
                SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_DOWN_REG_OFFSET), Pin_Ptr[Pin_index].Pin_num);
            }
            else
            {
                /* Clear the corresponding bit in the GPIOPUR register to disable the internal pull up pin */
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_UP_REG_OFFSET), Pin_Ptr[Pin_index].Pin_num);

                /* Clear the corresponding bit in the GPIOPDR register to disable the internal pull down pin */
                CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_PULL_DOWN_REG_OFFSET), Pin_Ptr[Pin_index].Pin_num);
            }
        }
        else
        {
            /* Do Nothing */
        }

        /********************************_Configuring Modes_***********************************************/
        if (Pin_Ptr[Pin_index].Mode == PORT_PIN_DIO_MODE)
        {
            /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET), Pin_Ptr[Pin_index].Pin_num);

            /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET), Pin_Ptr[Pin_index].Pin_num);

            /* Clear the PMCx bits for this pin
             * The idea here that we have mask of ones needs to be shifted by (pin number*4) to map GPIOPCTL (32bit) bits
             * after shifting to the right position flip ones to zeroes to set the Dio mode */
            *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Pin_Ptr[Pin_index].Pin_num * 4));

            /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET), Pin_Ptr[Pin_index].Pin_num);
        }
        else if (Pin_Ptr[Pin_index].Mode == PORT_PIN_ADC_MODE)
        {
            /* Clear the corresponding bit in the GPIODEN register to disable digital functionality on this pin */
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET), Pin_Ptr[Pin_index].Pin_num);

            /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET), Pin_Ptr[Pin_index].Pin_num);

            /* Clear the PMCx bits for this pin (The value of PMCx dosen't matter in this case)
             * The idea here that we have mask of ones needs to be shifted by (pin number*4) to map GPIOPCTL (32bit) bits
             * after shifting to the right position flip ones to zeroes to set the ADC mode */
            *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Pin_Ptr[Pin_index].Pin_num * 4));

            /* Set the corresponding bit in the GPIOAMSEL register to enable analog functionality on this pin */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET), Pin_Ptr[Pin_index].Pin_num);
        }
        /* Alternative modes */
        else
        {
            /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET), Pin_Ptr[Pin_index].Pin_num);

            /* Enable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET), Pin_Ptr[Pin_index].Pin_num);

            /* set the PMCx bits for this pin with respect to the mode
             * The idea here that we have mask of ones needs to be shifted by (pin number*4) to map GPIOPCTL (32bit) bits
             * after shifting to the right position anding this mask to the required mode then oring with GPIOPCTL
             * this will set the mode bits in the corresponding PMCx bits then*/
            *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (Pin_Ptr[Pin_index].Mode & 0x0000000F << (Pin_Ptr[Pin_index].Pin_num * 4));

            /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET), Pin_Ptr[Pin_index].Pin_num);
        }
    }
}

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
#if (PORT_SET_PIN_DIRECTION_API == STD_ON)
void Port_SetPinDirection(Port_PinType Pin, Port_PinDirectionType Direction)
{
    boolean error = FALSE;

#if (PORT_DEV_ERROR_DETECT == STD_ON)
    /* check if the Port is not initialized */
    if (Port_Status == PORT_NOT_INITIALIZED)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_DIRECTION_SID, PORT_E_UNINIT);
        error = TRUE;
    }
    else
    {
        /* Do Nothing */
    }

    /* check if incorrect Port Pin ID passed */
    if (Pin >= PORT_CONFIGURED_CHANNLES)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_DIRECTION_SID, PORT_E_PARAM_PIN);
        error = TRUE;
    }
    else
    {
        /* Do Nothing */
    }

    /* check if Port Pin not configured as changeable */
    if (Pin_Ptr[Pin].Direction_Change_Ability == STD_OFF)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_DIRECTION_SID, PORT_E_DIRECTION_UNCHANGEABLE);
        error = TRUE;
    }
    else
    {
        /* Do Nothing */
    }
#endif
    if (error == FALSE)
    {
        volatile uint32 *PortGpio_Ptr = NULL_PTR; /* point to the required Port Registers base address */

        switch (Pin_Ptr[Pin].Port_num)
        {
        case 0:
            PortGpio_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
            break;
        case 1:
            PortGpio_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
            break;
        case 2:
            PortGpio_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
            break;
        case 3:
            PortGpio_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
            break;
        case 4:
            PortGpio_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
            break;
        case 5:
            PortGpio_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
            break;
        }

        if ((Pin_Ptr[Pin].Port_num == 2) && (Pin_Ptr[Pin].Pin_num <= 3)) /* PC0 to PC3 */
        {
            /* Do Nothing ...  this is the JTAG pins */

        }
        else{
        if (Direction == PORT_PIN_OUT)
        {
            /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET), Pin_Ptr[Pin].Pin_num);
        }
        else if (Direction == PORT_PIN_IN)
        {
            /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET), Pin_Ptr[Pin].Pin_num);
        }
        else
        {
            /* Do Nothing */
        }
    }
    }
}
#endif

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
void Port_RefreshPortDirection(void)
{
    boolean error = FALSE;

#if (PORT_DEV_ERROR_DETECT == STD_ON)
    /* check if the Port is not initialized */
    if (Port_Status == PORT_NOT_INITIALIZED)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_REFREASH_PORT_DIRECTION_SID, PORT_E_UNINIT);
        error = TRUE;
    }
    else
    {
        /* Do Nothing */
    }
#endif



    if (error == FALSE)
    {
        uint8 Pin_index;
        for (Pin_index = 0; Pin_index < PORT_CONFIGURED_CHANNLES; Pin_index++){

        volatile uint32 *PortGpio_Ptr = NULL_PTR; /* point to the required Port Registers base address */

        switch (Pin_Ptr[Pin_index].Port_num)
        {
        case 0:
            PortGpio_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
            break;
        case 1:
            PortGpio_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
            break;
        case 2:
            PortGpio_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
            break;
        case 3:
            PortGpio_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
            break;
        case 4:
            PortGpio_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
            break;
        case 5:
            PortGpio_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
            break;
        }

        if ((Pin_Ptr[Pin_index].Port_num == 2) && (Pin_Ptr[Pin_index].Pin_num <= 3)) /* PC0 to PC3 */
        {
            /* Do Nothing ...  this is the JTAG pins */
        }
        else{
        if (Pin_Ptr[Pin_index].Direction == PORT_PIN_OUT)
        {
            /* Set the corresponding bit in the GPIODIR register to configure it as output pin */
            SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET), Pin_Ptr[Pin_index].Pin_num);
        }
        else if (Pin_Ptr[Pin_index].Direction == PORT_PIN_IN)
        {
            /* Clear the corresponding bit in the GPIODIR register to configure it as input pin */
            CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIR_REG_OFFSET), Pin_Ptr[Pin_index].Pin_num);
        }
        else
        {
            /* Do Nothing */
        }
    }
   }
    }
}

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
#if (PORT_VERSION_INFO_API == STD_ON)
void Port_GetVersionInfo(Std_VersionInfoType *versioninfo)
{
    boolean error = FALSE;
#if (PORT_DEV_ERROR_DETECT == STD_ON)
    /* check if the input configuration pointer is not a NULL_PTR */
    if (versioninfo == NULL_PTR)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_GET_VERSION_INFO_SID, PORT_E_PARAM_POINTER);
        error = TRUE;
    }
    else
    {
        /* Do Nothing */
    }

    /* Check if the Driver is initialized before using this function */
    if (Port_Status == PORT_NOT_INITIALIZED)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_GET_VERSION_INFO_SID, PORT_E_UNINIT);
        error = TRUE;
    }
    else
    {
        /* Do Nothing */
    }
#endif
    if (error == FALSE)
    {
        /* Copy the module Id */
        versioninfo->moduleID = (uint16)PORT_MODULE_ID;
        /* Copy the vendor Id */
        versioninfo->vendorID = (uint16)PORT_VENDOR_ID;
        /* Copy Software Major Version */
        versioninfo->sw_major_version = (uint8)PORT_SW_MAJOR_VERSION;
        /* Copy Software Minor Version */
        versioninfo->sw_minor_version = (uint8)PORT_SW_MINOR_VERSION;
        /* Copy Software Patch Version */
        versioninfo->sw_patch_version = (uint8)PORT_SW_PATCH_VERSION;
    }
}
#endif

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
#if (PORT_SET_PIN_MODE_API == STD_ON)
void Port_SetPinMode(Port_PinType Pin, Port_PinModeType Mode)
{
    boolean error = FALSE;
#if (PORT_DEV_ERROR_DETECT == STD_ON)

    /* Check if the Driver is initialized before using this function */
    if (Port_Status == PORT_NOT_INITIALIZED)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID, PORT_E_UNINIT);
        error = TRUE;
    }
    else
    {
        /* Do Nothing */
    }

    /* check if incorrect Port Pin ID passed */
    if (Pin >= PORT_CONFIGURED_CHANNLES)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID, PORT_E_PARAM_PIN);
        error = TRUE;
    }
    else
    {
        /* Do Nothing */
    }

    /* check if the Port Pin Mode passed not valid */
    if (PORT_PIN_DIO_MODE <= Mode && Mode <= PORT_PIN_ADC_MODE)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID, PORT_E_PARAM_INVALID_MODE);
    }
    else
    {
        /* Do Nothing */
    }

    /* check if the API called when the mode is unchangeable */
    if (Pin_Ptr[Pin].Direction_Change_Ability == STD_OFF)
    {
        Det_ReportError(PORT_MODULE_ID, PORT_INSTANCE_ID, PORT_SET_PIN_MODE_SID, PORT_E_MODE_UNCHANGEABLE);
    }
    else
    {
        /* Do Nothing */
    }
#endif
    if (error == FALSE){
    volatile uint32 *PortGpio_Ptr = NULL_PTR; /* point to the required Port Registers base address */

    switch (Pin_Ptr[Pin].Port_num)
    {
    case 0:
        PortGpio_Ptr = (volatile uint32 *)GPIO_PORTA_BASE_ADDRESS; /* PORTA Base Address */
        break;
    case 1:
        PortGpio_Ptr = (volatile uint32 *)GPIO_PORTB_BASE_ADDRESS; /* PORTB Base Address */
        break;
    case 2:
        PortGpio_Ptr = (volatile uint32 *)GPIO_PORTC_BASE_ADDRESS; /* PORTC Base Address */
        break;
    case 3:
        PortGpio_Ptr = (volatile uint32 *)GPIO_PORTD_BASE_ADDRESS; /* PORTD Base Address */
        break;
    case 4:
        PortGpio_Ptr = (volatile uint32 *)GPIO_PORTE_BASE_ADDRESS; /* PORTE Base Address */
        break;
    case 5:
        PortGpio_Ptr = (volatile uint32 *)GPIO_PORTF_BASE_ADDRESS; /* PORTF Base Address */
        break;
    }

    if ((Pin_Ptr[Pin].Port_num == 2) && (Pin_Ptr[Pin].Pin_num <= 3)) /* PC0 to PC3 */
    {
        /* Do Nothing ...  this is the JTAG pins */

    }
    else{
    if (Pin_Ptr[Pin].Mode == PORT_PIN_DIO_MODE)
    {
        /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET), Pin_Ptr[Pin].Pin_num);

        /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET), Pin_Ptr[Pin].Pin_num);

        /* Clear the PMCx bits for this pin
         * The idea here that we have mask of ones needs to be shifted by (pin number*4) to map GPIOPCTL (32bit) bits
         * after shifting to the right position flip ones to zeroes to set the Dio mode */
        *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Pin_Ptr[Pin].Pin_num * 4));

        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
        SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET), Pin_Ptr[Pin].Pin_num);
    }
    else if (Pin_Ptr[Pin].Mode == PORT_PIN_ADC_MODE)
    {
        /* Clear the corresponding bit in the GPIODEN register to disable digital functionality on this pin */
        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET), Pin_Ptr[Pin].Pin_num);

        /* Disable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET), Pin_Ptr[Pin].Pin_num);

        /* Clear the PMCx bits for this pin (The value of PMCx dosen't matter in this case)
         * The idea here that we have mask of ones needs to be shifted by (pin number*4) to map GPIOPCTL (32bit) bits
         * after shifting to the right position flip ones to zeroes to set the ADC mode */
        *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) &= ~(0x0000000F << (Pin_Ptr[Pin].Pin_num * 4));

        /* Set the corresponding bit in the GPIOAMSEL register to enable analog functionality on this pin */
        SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET), Pin_Ptr[Pin].Pin_num);
    }
    /* Alternative modes */
    else
    {
        /* Clear the corresponding bit in the GPIOAMSEL register to disable analog functionality on this pin */
        CLEAR_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ANALOG_MODE_SEL_REG_OFFSET), Pin_Ptr[Pin].Pin_num);

        /* Enable Alternative function for this pin by clear the corresponding bit in GPIOAFSEL register */
        SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_ALT_FUNC_REG_OFFSET), Pin_Ptr[Pin].Pin_num);

        /* set the PMCx bits for this pin with respect to the mode
         * The idea here that we have mask of ones needs to be shifted by (pin number*4) to map GPIOPCTL (32bit) bits
         * after shifting to the right position anding this mask to the required mode then oring with GPIOPCTL
         * this will set the mode bits in the corresponding PMCx bits then*/
        *(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_CTL_REG_OFFSET) |= (Pin_Ptr[Pin].Mode & 0x0000000F << (Pin_Ptr[Pin].Pin_num * 4));

        /* Set the corresponding bit in the GPIODEN register to enable digital functionality on this pin */
        SET_BIT(*(volatile uint32 *)((volatile uint8 *)PortGpio_Ptr + PORT_DIGITAL_ENABLE_REG_OFFSET), Pin_Ptr[Pin].Pin_num);
    }
    }
    }
}
#endif
