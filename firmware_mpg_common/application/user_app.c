/**********************************************************************************************************************
File: user_app.c                                                                

----------------------------------------------------------------------------------------------------------------------
To start a new task using this user_app as a template:
 1. Copy both user_app.c and user_app.h to the Application directory
 2. Rename the files yournewtaskname.c and yournewtaskname.h
 3. Add yournewtaskname.c and yournewtaskname.h to the Application Include and Source groups in the IAR project
 4. Use ctrl-h (make sure "Match Case" is checked) to find and replace all instances of "user_app" with "yournewtaskname"
 5. Use ctrl-h to find and replace all instances of "UserApp" with "YourNewTaskName"
 6. Use ctrl-h to find and replace all instances of "USER_APP" with "YOUR_NEW_TASK_NAME"
 7. Add a call to YourNewTaskNameInitialize() in the init section of main
 8. Add a call to YourNewTaskNameRunActiveState() in the Super Loop section of main
 9. Update yournewtaskname.h per the instructions at the top of yournewtaskname.h
10. Delete this text (between the dashed lines) and update the Description below to describe your task
----------------------------------------------------------------------------------------------------------------------

Description:
This is a user_app.c file template 

------------------------------------------------------------------------------------------------------------------------
API:

Public functions:


Protected System functions:
void UserAppInitialize(void)
Runs required initialzation for the task.  Should only be called once in main init section.

void UserAppRunActiveState(void)
Runs current task state.  Should only be called once in main loop.


**********************************************************************************************************************/

#include "configuration.h"

/***********************************************************************************************************************
Global variable definitions with scope across entire project.
All Global variable names shall start with "G_"
***********************************************************************************************************************/
/* New variables */
volatile u32 G_u32UserAppFlags;                       /* Global state flags */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Existing variables (defined in other files -- should all contain the "extern" keyword) */
extern volatile u32 G_u32SystemFlags;                  /* From main.c */
extern volatile u32 G_u32ApplicationFlags;             /* From main.c */

extern volatile u32 G_u32SystemTime1ms;                /* From board-specific source file */
extern volatile u32 G_u32SystemTime1s;                 /* From board-specific source file */


/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp_StateMachine;            /* The state machine function pointer */
static u32 UserApp_u32Timeout;                      /* Timeout counter used across states */


/**********************************************************************************************************************
Function Definitions
**********************************************************************************************************************/

/*--------------------------------------------------------------------------------------------------------------------*/
/* Public functions                                                                                                   */
/*--------------------------------------------------------------------------------------------------------------------*/


/*--------------------------------------------------------------------------------------------------------------------*/
/* Protected functions                                                                                                */
/*--------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
Function: UserAppInitialize

Description:
Initializes the State Machine and its variables.

Requires:
  -

Promises:
  - 
*/
void UserAppInitialize(void)
{
  PWMAudioSetFrequency(BUZZER1, 1000);

  u8 au8GameName[] = "  Avoid the brick!";
  u8 au8Instructions[] = "Press BUTTON0 start";
  static u8 UserApp_CursorPosition = LINE2_END_ADDR;
  
  /* Clear screen and place start messages */
  LCDCommand(LCD_CLEAR_CMD);
  LCDMessage(LINE1_START_ADDR, au8GameName); 
  LCDMessage(LINE2_START_ADDR, au8Instructions); 
  /* If good initialization, set state to Idle */
  if( 1 )
  {
    UserApp_StateMachine = UserAppSM_Idle;
  }
  else
  {
    /* The task isn't properly initialized, so shut it down and don't run */
    UserApp_StateMachine = UserAppSM_FailedInit;
  }

} /* end UserAppInitialize() */


/*----------------------------------------------------------------------------------------------------------------------
Function UserAppRunActiveState()

Description:
Selects and runs one iteration of the current state in the state machine.
All state machines have a TOTAL of 1ms to execute, so on average n state machines
may take 1ms / n to execute.

Requires:
  - State machine function pointer points at current state

Promises:
  - Calls the function to pointed by the state machine function pointer
*/
void UserAppRunActiveState(void)
{
  UserApp_StateMachine();

} /* end UserAppRunActiveState */


/*--------------------------------------------------------------------------------------------------------------------*/
/* Private functions                                                                                                  */
/*--------------------------------------------------------------------------------------------------------------------*/


/**********************************************************************************************************************
State Machine Function Definitions
**********************************************************************************************************************/

/*-------------------------------------------------------------------------------------------------------------------*/
/* Wait for a message to be queued */
static void UserAppSM_Idle(void)
{
  static u8 counter=0;
  static u16 counter0=0;
  static u8 counter1=0;
  static u8 flag=FALSE;
  static u8 flag1=FALSE;
  static u8 flag2=TRUE;
  static u8 flag3=FALSE;
  static u16 DelayTime;
  
  static u8 number[100];
  static u32 Randomnumber=1;
  static u32 Midnumber;
  static u8 counter2=0;
  static u8 counter3=0;
  static u8 UserApp_CursorPosition1[100];
  static u8 UserApp_CursorPosition2[100];
  
  static u8 BrickLine=0;
  //
  static u16 delaytimecounter=0;
  static u8 u8buzzerrateIndex=0;
  static u16 u16buzzerrate[]={330,294,262,294,330,330,330,0,
                              294,294,294,0,
                              330,392,392,0,
                              330,294,262,294,330,330,330,0,
                              330,294,294,330,294,262,0};
  
  /*delaytimecounter++;
 
  if(delaytimecounter==200)
  {
    delaytimecounter=0;
    u8buzzerrateIndex++;

    PWMAudioSetFrequency(BUZZER1,u16buzzerrate[u8buzzerrateIndex]);
    PWMAudioOn(BUZZER1);

        
    if(u8buzzerrateIndex==31)
    {
      u8buzzerrateIndex=0;
    } 
  }*/
  //
  if(WasButtonPressed(BUTTON0))
  {
    ButtonAcknowledge(BUTTON0);
    LCDCommand(LCD_CLEAR_CMD); 
    LCDMessage(LINE2_START_ADDR+2, "8");
    //flag3=TRUE;
    flag=TRUE;
    LedOff(RED);
    PWMAudioOff(BUZZER1);
    flag1=FALSE;
    flag2=TRUE;
    DelayTime=500;
  }
  
  if(flag)
  {
    counter0++;
    if(counter0==DelayTime)
    {
      counter0=0;
      counter1++;
      counter++;
      if (counter==20)
      {
        counter=0;
        if(DelayTime>50)
        {
          DelayTime=DelayTime-10;
        }
      }
     
      /*Contral the "man" up or down*/
      LCDCommand(LCD_CLEAR_CMD);
      if(flag1)
      {
        LCDMessage(LINE1_START_ADDR+2, "8"); 
      }
      else if(flag2)
      {
        LCDMessage(LINE2_START_ADDR+2, "8");
      }
      if(counter1==3)
      {
        counter1=0;
       
        Midnumber=Randomnumber*22695477+1;
        Randomnumber=(Midnumber/65535)&0x7fff;
        DebugPrintNumber(Randomnumber);
        DebugPrintf("  ");
        /*shot a brick randomly*/
        if(Randomnumber%2==0)
        {
           
          BrickLine=1;
          UserApp_CursorPosition1[counter2]=19;
          LCDMessage(LINE1_START_ADDR+UserApp_CursorPosition1[counter2], "0");
          counter2++;
        }
        else if(Randomnumber%2!=0)
        {
          
          BrickLine=2;
          UserApp_CursorPosition2[counter3]=19;
          LCDMessage(LINE2_START_ADDR+UserApp_CursorPosition2[counter3], "0");
          counter3++;
        }        
      }
      for(u8 m=0;m<counter2;m++)
      {
        UserApp_CursorPosition1[m]--;
        LCDMessage(LINE1_START_ADDR+UserApp_CursorPosition1[m], "0");
      }
      for(u8 n=0;n<counter3;n++)
      {
        UserApp_CursorPosition2[n]--;
        LCDMessage(LINE2_START_ADDR+UserApp_CursorPosition2[n], "0");
      }
      /*if(BrickLine==1)
      {
        LCDMessage(LINE1_START_ADDR+UserApp_CursorPosition1[counter2], "0"); 
        UserApp_CursorPosition1[counter2]--;  
      }
      if(BrickLine==2)
      {
        LCDMessage(LINE2_START_ADDR+UserApp_CursorPosition2[counter3], "0"); 
        UserApp_CursorPosition2[counter3]--;  
      }*/
    }
    if(WasButtonPressed(BUTTON1))
    {
      ButtonAcknowledge(BUTTON1);
      //LCDCommand(LCD_CLEAR_CMD);
      flag1=TRUE;
      flag2=FALSE;
      flag3=FALSE;
      LCDMessage(LINE1_START_ADDR+2, "8"); 
    }
    if(WasButtonPressed(BUTTON2))
    {
      ButtonAcknowledge(BUTTON2);
      //LCDCommand(LCD_CLEAR_CMD);
      flag1=FALSE;
      flag2=TRUE;
      flag3=TRUE;
      LCDMessage(LINE2_START_ADDR+2, "8"); 
    }
    
    /*if(UserApp_CursorPosition[j]+1==LINE2_START_ADDR+2)
    {
      if(flag3)
      {
        
        flag3=FALSE;
        flag=FALSE;
        LedOn(RED);
        
        PWMAudioOn(BUZZER1);
        LCDCommand(LCD_CLEAR_CMD);
        LCDMessage(LINE1_START_ADDR+5, "Game over!"); 
        LCDMessage(LINE2_START_ADDR, "Press BUTTON0 start");
      }
    }*/
  }
} /* end UserAppSM_Idle() */
     

/*-------------------------------------------------------------------------------------------------------------------*/
/* Handle an error */
static void UserAppSM_Error(void)          
{
  
} /* end UserAppSM_Error() */


/*-------------------------------------------------------------------------------------------------------------------*/
/* State to sit in if init failed */
static void UserAppSM_FailedInit(void)          
{
    
} /* end UserAppSM_FailedInit() */


/*--------------------------------------------------------------------------------------------------------------------*/
/* End of File                                                                                                        */
/*--------------------------------------------------------------------------------------------------------------------*/
