/* mbed Microcontroller Library
 * Copyright (c) 2006-2015 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mbed-drivers/mbed.h"
#include "ble/BLE.h"
DigitalOut led(p7, 1);

// PwmOut base_servo(p9);
// PwmOut updown_servo(p16);
// PwmOut fwdrvd_servo(p17);
// PwmOut hand_servo(p18);


uint16_t armServiceUUID     = 0xA000;
uint16_t readCharUUID       = 0xA001;
uint16_t writeCharUUID      = 0xA002;

const static char     DEVICE_NAME[] = "BLE_ARM_MK2";
static const uint16_t uuid16_list[]        = {0xA000}; //Custom UUID, FFFF is reserved for development

/* Set Up custom Characteristics */
static uint8_t readValue[4] = {0};
ReadOnlyArrayGattCharacteristic<uint8_t, sizeof(readValue)> readChar(readCharUUID, readValue);

static uint8_t writeValue[4] = {0};
WriteOnlyArrayGattCharacteristic<uint8_t, sizeof(writeValue)> writeChar(writeCharUUID, writeValue);

/* Set up custom service */
GattCharacteristic *characteristics[] = {&readChar, &writeChar};
GattService        armService(armServiceUUID, characteristics, sizeof(characteristics) / sizeof(GattCharacteristic *));


/*
 *  Restart advertising when phone app disconnects
*/
void disconnectionCallback(const Gap::DisconnectionCallbackParams_t *)
{
  BLE::Instance(BLE::DEFAULT_INSTANCE).gap().startAdvertising();
}

/*
 *  Handle writes to writeCharacteristic
*/
void writeCharCallback(const GattWriteCallbackParams *params)
{
  if (params->handle == writeChar.getValueHandle()) {
    // if (params->len == 1) {
    //   led = params->data[0];
    //   (params->data[0] == 0x00) ? printf("led on\n\r") : printf("led off\n\r"); // print led toggle
    // }
    // else {
    //   printf("Data received: length = %d, data = 0x", params->len);
    //   for (int x = 0; x < params->len; x++) {
    //     printf("%x", params->data[x]);
    //   }
    //   printf("\n\r");
    // }
    BLE::Instance(BLE::DEFAULT_INSTANCE).gattServer().write(readChar.getValueHandle(), params->data, params->len);
  }
}
void periodicCallback(void)
{
  led = !led; /* Do blinky on led while we're waiting for BLE events */
}
/**
 * This function is called when the ble initialization process has failled
 */
void onBleInitError(BLE &ble, ble_error_t error)
{
  /* Initialization error handling should go here */
}

/**
 * Callback triggered when the ble initialization process has finished
 */
void bleInitComplete(BLE::InitializationCompleteCallbackContext *params)
{
  BLE&        ble   = params->ble;
  ble_error_t error = params->error;

  if (error != BLE_ERROR_NONE) {
    /* In case of error, forward the error handling to onBleInitError */
    onBleInitError(ble, error);
    return;
  }

  /* Ensure that it is the default instance of BLE */
  if (ble.getInstanceID() != BLE::DEFAULT_INSTANCE) {
    return;
  }

  ble.gap().onDisconnection(disconnectionCallback);
  ble.gattServer().onDataWritten(writeCharCallback);

  ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::BREDR_NOT_SUPPORTED | GapAdvertisingData::LE_GENERAL_DISCOVERABLE); // BLE only, no classic BT
  ble.gap().setAdvertisingType(GapAdvertisingParams::ADV_CONNECTABLE_UNDIRECTED); // advertising type
  ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LOCAL_NAME, (uint8_t *)DEVICE_NAME, sizeof(DEVICE_NAME)); // add name
  ble.gap().accumulateAdvertisingPayload(GapAdvertisingData::COMPLETE_LIST_16BIT_SERVICE_IDS, (uint8_t *)uuid16_list, sizeof(uuid16_list)); // UUID's broadcast in advertising packet

  /* Add our custom service */
  ble.addService(armService);

  ble.gap().setAdvertisingInterval(100); /* 1000ms. */
  ble.gap().startAdvertising();
}

void app_start(int, char**)
{
  minar::Scheduler::postCallback(periodicCallback).period(minar::milliseconds(500));
  // base_servo.period_ms(20);
  // base_servo.write(0.9f);
  BLE &ble = BLE::Instance();
  ble.init(bleInitComplete);
}
