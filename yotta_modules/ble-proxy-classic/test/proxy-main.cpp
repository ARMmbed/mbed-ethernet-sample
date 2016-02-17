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

#include "mbed.h"

#include "BLE/ble.h"
#include "ble-proxy/BLEProxy.h"
#include "ble-proxy/Endpoint.h"
#include "ble-proxy/UDPSocket.h"
#include "ble-proxy/TCPSocketConnection.h"

#include <string>

static BLE ble;

static char buffer[240];
static uint16_t bufferLength = sizeof(buffer);

static Endpoint echo;
static UDPSocket socket;
static TCPSocketConnection wget;

// debug led - blinks to show liveness
static Ticker ticker;
static DigitalOut mbed_led1(LED1);

// enable buttons to initiate transfer
static InterruptIn button1(BUTTON1);
static InterruptIn button2(BUTTON2);

static Ticker test;
static Timeout timeout;
static Timeout decouple;
/*****************************************************************************/
/* Debug                                                                     */
/*****************************************************************************/
/*
    Called once every second. Blinks LED.
*/
void periodicCallback(void)
{
    mbed_led1 = !mbed_led1;
}

/*****************************************************************************/
/* Buttons                                                                   */
/*****************************************************************************/

static bool tcp = false;

/*
*/
void button1ISR()
{
    printf("Button: TCP\r\n");

    tcp = true;

    const char* index = "GET /index.html HTTP/1.1\n\n";

    wget.send_all(index, strlen(index));
}

void button2ISR()
{
    printf("Button: UDP\r\n");

    tcp = false;

    socket.sendTo(echo, "Hello World", 11);
}

/*****************************************************************************/
/* BLE                                                                       */
/*****************************************************************************/

void whenConnected(const Gap::ConnectionCallbackParams_t* params)
{
    printf("ble::proxy: Connected: %d %d %d %d\r\n", params->connectionParams->minConnectionInterval,
                                                      params->connectionParams->maxConnectionInterval,
                                                      params->connectionParams->slaveLatency,
                                                      params->connectionParams->connectionSupervisionTimeout);
}

void whenDisconnected(const Gap::DisconnectionCallbackParams_t* params)
{
    printf("ble::proxy: Disconnected!\r\n");
    printf("ble::proxy: Restarting the advertising process\r\n");

    (void) params;
}

void onReceiveHandler()
{
    if (tcp)
    {
        int result = wget.receive_all(buffer, sizeof(buffer));

        if (result > 0)
        {
            buffer[result] = '\0';

            printf("%s\r\n", buffer);
        }
    }
    else
    {
        int result = socket.receiveFrom(echo, buffer, bufferLength);

        if (result > 0)
        {
            buffer[result] = '\0';
            printf("received: %s\r\n", (char*) buffer);
        }
    }
}

/*****************************************************************************/
/* App start                                                                 */
/*****************************************************************************/
void app_start(int, char *[])
{
    // setup buttons
    button1.mode(PullUp);
    // Delay for initial pullup to take effect
    wait(.01);
    button1.fall(button1ISR);

    button2.mode(PullUp);
    // Delay for initial pullup to take effect
    wait(.01);
    button2.fall(button2ISR);

    // blink led
    ticker.attach_us(periodicCallback, 1000 * 1000);

    /*************************************************************************/
    /*************************************************************************/
    ble.init();

    // connection status handlers
    ble.gap().onConnection(whenConnected);
    ble.gap().onDisconnection(whenDisconnected);

    echo.set_address("10.1.26.160", 2000);
    socket.init();

    wget.connect("www.google.com", 80);

    BLEProxy::onReceive(onReceiveHandler);

    printf("BLE Proxy: %s %s\r\n", __DATE__, __TIME__);
}

/*****************************************************************************/
/* Compatibility                                                             */
/*****************************************************************************/

#if defined(YOTTA_MINAR_VERSION_STRING)
/*********************************************************/
/* Build for mbed OS                                     */
/*********************************************************/

#else
/*********************************************************/
/* Build for mbed Classic                                */
/*********************************************************/
int main(void)
{
    app_start(0, NULL);

    for(;;)
    {
        BLEProxy::callFromMainLoop();
        ble.waitForEvent();
    }
}
#endif
