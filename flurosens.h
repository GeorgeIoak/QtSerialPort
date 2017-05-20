#ifndef FLUROSENS_H
#define FLUROSENS_H

#endif // FLUROSENS_H

#define CYCLES              0x00; //2 Bytes
#define CYCLETIME           0x01; //2 Bytes
#define STARTMODE           0x02;
#define METHOD_TYPE         0x03;
#define DARK_SIGNAL_TYPE    0x04;
#define AVERAGE             0x05;
#define LED_MODE            0x06;
#define TRIGGER_DELAY       0x07;
#define E1D1_FACTOR         0x08; //4 Bytes
#define E1D2_FACTOR         0x0A; //4 Bytes
#define E2D2_FACTOR         0x0C; //4 Bytes
#define E1D1_OFFSET         0x0E; //4 Bytes
#define E1D2_OFFSET         0x10; //4 Bytes
#define E2D2_OFFSET         0x12; //4 Bytes
#define ON_DELAY_LED1       0x14; //2 Bytes
#define ON_DELAY_LED2       0x15; //2 Bytes
#define OFF_DELAY_LED1      0x16; //2 Bytes
#define OFF_DELAY_LED2      0x17; //2 Bytes
#define LED1_CURRENT        0x18;
#define LED2_CURRENT        0x19;
#define LED1_CURRENT_DEFAULT        0x1A;
#define LED2_CURRENT_DEFAULT        0x1B;
#define LED1_CURRENT_MAX    0x1B;
#define LED2_CURRENT_MAX    0x1C;
#define LED1_CURRENT_MIN    0x1D;
#define LED2_CURRENT_MIN    0x1E;
#define ADC_SAMPLING        0x1F;
#define BOARD_NAME          0x80; //32 Bytes
#define BOARD_SERIAL_NUM    0x90; //8 Bytes
#define BOARD_ID            0x94; //16 Bytes
#define HARDWARE_REV        0x9C; //8 Bytes
#define OPTIC_REV           0xA0; //8 Bytes
#define BOARD_TYPE          0xA4;
#define MODBUS_ADD          0xA5;
#define BAUDRATE            0xA6; //4 Bytes
#define TICKET              0x100; //4 Bytes
#define TEMP                0x102; //4 Bytes
#define ON_VALUE_1          0x104; //4 Bytes
