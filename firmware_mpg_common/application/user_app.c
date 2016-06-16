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

extern AntSetupDataType G_stAntSetupData;                         /* From ant.c */

extern u32 G_u32AntApiCurrentDataTimeStamp;                       /* From ant_api.c */
extern AntApplicationMessageType G_eAntApiCurrentMessageClass;    /* From ant_api.c */
extern u8 G_au8AntApiCurrentData[ANT_APPLICATION_MESSAGE_BYTES];  /* From ant_api.c */



/***********************************************************************************************************************
Global variable definitions with scope limited to this local application.
Variable names shall start with "UserApp_" and be declared as static.
***********************************************************************************************************************/
static fnCode_type UserApp_StateMachine;            /* The state machine function pointer */
static u32 UserApp_u32Timeout;                      /* Timeout counter used across states */

static u8 UserApp_CursorPosition[5];
static u8 u8Brickexsit[5];

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
  
  /* Configure ANT for this application */
  G_stAntSetupData.AntChannel          = ANT_CHANNEL_USERAPP;
  G_stAntSetupData.AntSerialLo         = ANT_SERIAL_LO_USERAPP;
  G_stAntSetupData.AntSerialHi         = ANT_SERIAL_HI_USERAPP;
  G_stAntSetupData.AntDeviceType       = ANT_DEVICE_TYPE_USERAPP;
  G_stAntSetupData.AntTransmissionType = ANT_TRANSMISSION_TYPE_USERAPP;
  G_stAntSetupData.AntChannelPeriodLo  = ANT_CHANNEL_PERIOD_LO_USERAPP;
  G_stAntSetupData.AntChannelPeriodHi  = ANT_CHANNEL_PERIOD_HI_USERAPP;
  G_stAntSetupData.AntFrequency        = ANT_FREQUENCY_USERAPP;
  G_stAntSetupData.AntTxPower          = ANT_TX_POWER_USERAPP;

  /*Initialize some arrays*/
  UserApp_CursorPosition[0] = LINE1_END_ADDR;
  UserApp_CursorPosition[1] = LINE2_END_ADDR-4;
  UserApp_CursorPosition[2] = LINE1_END_ADDR-8;
  UserApp_CursorPosition[3] = LINE2_END_ADDR-12;
  UserApp_CursorPosition[4] = LINE1_END_ADDR-16;
  u8Brickexsit[0]=1;
  u8Brickexsit[1]=1;
  u8Brickexsit[2]=1;
  u8Brickexsit[3]=1;
  u8Brickexsit[4]=1;
  
  PWMAudioSetFrequency(BUZZER1, 1000);
  u8 au8GameName[] = "  Avoid the brick!";
  u8 au8Instructions[] = "Press BUTTON0 start";
  
  /* Clear screen and display start messages */
  LCDCommand(LCD_CLEAR_CMD);
  LCDMessage(LINE1_START_ADDR, au8GameName); 
  LCDMessage(LINE2_START_ADDR, au8Instructions); 
  
  /* If good initialization, set state to Idle */
  if( AntChannelConfig(ANT_MASTER) )
  {
    AntOpenChannel();
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
  /*Define some variables*/
  static u8 au8TestMessage[] = {0, 0, 0, 0, 0xA5, 0, 0, 0};
  u8 au8DataContent[] = "xxxxxxxxxxxxxxxx";
  static u8 u8Brick[]=21;
  static u8 counter=0;
  static u16 counter0=0;
  static u8 counter1=0;
  static u8 flag=FALSE;
  static u8 flag1=FALSE;
  static u8 flag2=TRUE;
  static u8 flag3=3;
  static u8 flag4=0;
  static u16 DelayTime;
  static u32 Randomnumber=2;
  static u32 Midnumber;
  
  static u8 u8tempi=0;
  static u8 u8tempj=0;
  /*Press BUTTON0, start the game and  initialize some flags*/
  if(WasButtonPressed(BUTTON0))
  {
    ButtonAcknowledge(BUTTON0);
    LCDCommand(LCD_CLEAR_CMD); 
    LCDMessage(LINE2_START_ADDR+3, "8");
    flag3=2;
    flag=TRUE;
    LedOff(RED);
    PWMAudioOff(BUZZER1);
    flag1=FALSE;
    flag2=TRUE;
    DelayTime=500;
    
    /*Initialize the flag and send it to control the slave's LCD to start the game*/
    au8TestMessage[6]=1;
    
    au8TestMessage[7]=0;
    flag4=1;
    LedOff(WHITE);
  }
  /*If pressed BUTTON0, the game is going*/
  if(flag)
  {
    counter0++;
    /*Delay a little time*/
    if(counter0==DelayTime)
    {
      counter0=0;
      counter1++;
      counter++;
      /*Change the speed of moving bricks by decreasing the delay time*/
      if (counter==20)
      {
        counter=0;
        if(DelayTime>50)
        {
          DelayTime=DelayTime-30;
        }
      }
     
      /*Contral the "man" up or down*/
      LCDCommand(LCD_CLEAR_CMD);
      if(flag1)
      {
        LCDMessage(LINE1_START_ADDR+3, "8"); 
      }
      else if(flag2)
      {
        LCDMessage(LINE2_START_ADDR+3, "8");
      }
      /*Shot a brick every four spaces*/
      if(counter1==4)
      {
        counter1=0;
        
        /*Create a number randomly*/
        Midnumber=Randomnumber*22695477+1;
        Randomnumber=(Midnumber/65536)&0x7fff;
         
      }
      /*If the brick move to the LCD start address, let it go to the end*/
      for(u8tempi=0;u8tempi<5;u8tempi++)
      {
        if(UserApp_CursorPosition[u8tempi] == LINE1_START_ADDR)
        {
          UserApp_CursorPosition[u8tempi]=LINE1_END_ADDR;
          u8Brickexsit[u8tempi]=0; 
        }
        else if(UserApp_CursorPosition[u8tempi] == LINE2_START_ADDR)
        {
          UserApp_CursorPosition[u8tempi]=LINE2_END_ADDR;
          u8Brickexsit[u8tempi]=0; 
        }
        
      }
      /*shot a brick on the LCD LINE1 or LINE2 randomly*/
      for(u8tempi=0;u8tempi<5;u8tempi++)
      {
        if(u8Brickexsit[u8tempi]==0)
        {
          if(Randomnumber%2==0)
          {
            UserApp_CursorPosition[u8tempi]=LINE1_END_ADDR;
            LCDMessage(UserApp_CursorPosition[u8tempi], u8Brick);
          }
          else if(Randomnumber%2!=0)
          {
            UserApp_CursorPosition[u8tempi]=LINE2_END_ADDR;
            LCDMessage(UserApp_CursorPosition[u8tempi], u8Brick);
          }
          u8Brickexsit[u8tempi]=1;
        }
        
      }
      /*Display all the brick in the LCD at the same time almostly*/
      for(u8tempj=0;u8tempj<5;u8tempj++)
      {
        if(u8Brickexsit[u8tempj]==1)
        {
          
          
          LCDMessage(UserApp_CursorPosition[u8tempj],u8Brick);
          UserApp_CursorPosition[u8tempj]--;
          
        }
      }
      /*If a brick hit the "man", game over*/ 
      for(u8tempi=0;u8tempi<5;u8tempi++)
      {
        if((UserApp_CursorPosition[u8tempi]==LINE1_START_ADDR+2 && flag3==1)||
           (UserApp_CursorPosition[u8tempi]==LINE2_START_ADDR+2 && flag3==2))
        {         
          LCDCommand(LCD_CLEAR_CMD);
          flag3=0;
          flag=FALSE;
          LedOn(RED);
          UserApp_StateMachine = UserAppSM_GameOver;
          PWMAudioOn(BUZZER1);        
        }
        
      }
    }
    /*If pressed BUTTON1, let the "man" move to LINE1*/
    if(WasButtonPressed(BUTTON1))
    {
      ButtonAcknowledge(BUTTON1);
      
      flag1=TRUE;
      flag2=FALSE;
      flag3=1;
      LCDMessage(LINE1_START_ADDR+3, "8"); 
      LCDMessage(LINE2_START_ADDR+3, " ");
    }
    /*If pressed BUTTON2, let the "man" move to LINE2*/
    if(WasButtonPressed(BUTTON2))
    {
      ButtonAcknowledge(BUTTON2);
     
      flag1=FALSE;
      flag2=TRUE;
      flag3=2;
      LCDMessage(LINE2_START_ADDR+3, "8"); 
      LCDMessage(LINE1_START_ADDR+3, " "); 
    }
  }
  /*chack if any new ANT information is in the ANT buffer*/
  if( AntReadData() )
  {
     /* New data message: check what it is */
    if(G_eAntApiCurrentMessageClass == ANT_DATA)
    {
      
       for(u8 i = 0; i < ANT_DATA_BYTES; i++)
       {
         au8DataContent[2 * i]     = HexToASCIICharUpper(G_au8AntApiCurrentData[i] / 16);
         au8DataContent[2 * i + 1] = HexToASCIICharUpper(G_au8AntApiCurrentData[i] % 16);
       }
     
     
     
    }
    else if(G_eAntApiCurrentMessageClass == ANT_TICK)
    {
     /* A channel period has gone by: typically this is when new data should be queued to be sent */
     for(u8 i = 0; i < ANT_DATA_BYTES; i++)
     {
       au8DataContent[2 * i]     = HexToASCIICharUpper(G_au8AntApiCurrentData[i] / 16);
       au8DataContent[2 * i + 1] = HexToASCIICharUpper(G_au8AntApiCurrentData[i] % 16);
     }
     
      /* Update the message counter */
     
        if(flag3==0)
        {
          au8TestMessage[6]=0;
          au8TestMessage[7]=1;
          flag3=2;
          LedOn(WHITE);
          AntQueueBroadcastMessage(au8TestMessage);
        }
         if(flag4==1)
        {
          au8TestMessage[7]=0;
          flag4=0;
          AntQueueBroadcastMessage(au8TestMessage);
        }        
    }   
  } /* end AntReadData() */

} /* end UserAppSM_Idle() */
     
static void UserAppSM_GameOver(void)          
{
  static u32 u32Delay = 1000;
  
  if(u32Delay++ == 1000)
  {
     LCDMessage(LINE1_START_ADDR+5, "Game over!"); 
     LCDMessage(LINE2_START_ADDR, "Press BUTTON0 start");
     UserApp_StateMachine = UserAppSM_Idle;
     u32Delay = 0;
  }
}
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
