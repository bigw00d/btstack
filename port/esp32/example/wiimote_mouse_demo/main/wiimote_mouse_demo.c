/*
 * Copyright (C) 2019 jp-96
 *
 * Copyright (C) 2017 BlueKitchen GmbH
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holders nor the names of
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 * 4. Any redistribution, use, or modification is done solely for
 *    personal benefit and not for any commercial purpose or for
 *    monetary gain.
 *
 * THIS SOFTWARE IS PROVIDED BY BLUEKITCHEN GMBH AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL MATTHIAS
 * RINGWALD OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
 * THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 * Please inquire about commercial licensing options at
 * contact@bluekitchen-gmbh.com
 *
 */

#define __BTSTACK_FILE__ "wiimote_led_demo.c"

/*
 * wiimote_led_demo.c
 */

/* EXAMPLE_START(wiimote_led_demo): Wiimote LED Demo
 *
 * @text This example implements an Wiimote LED. For now, it connnects to a fixed device, opens the Interrupt channels
 */

 // check usb device id (use $ ls -l /dev/serial/by-id/)
 // view id (example: /dev/ttyUSB0)
 // set usb device id (use $ make menuconfig)

#include <inttypes.h>
#include <stdio.h>

#include "btstack_config.h"
#include "btstack.h"

// Wiimote
static const char * wiimote_addr_string = "00-23-31-0F-7E-47";
static bd_addr_t wiimote_addr;

#define WIIMOTE_DATA_PSM        0x13
#define WIIMOTE_MTU             672

uint8_t wiimote_led = 0;

// L2CAP
static uint16_t           l2cap_wiimote_interrupt_cid = -1;

static btstack_packet_callback_registration_t hci_event_callback_registration;

/* @section Main application configuration
 *
 * @text In the application configuration, L2CAP is initialized
 */

static void hci_packet_handler (uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size);

static void wiimote_host_setup(void){

    // register for HCI events
    hci_event_callback_registration.callback = &hci_packet_handler;
    hci_add_event_handler(&hci_event_callback_registration);

    // Initialize L2CAP
    l2cap_init();

    // Disable stdout buffering
    setbuf(stdout, NULL);
}

/*
 * @section HCI Packet Handler
 *
 * @text The hci packet handler responds to various HCI Events.
 */
static void hci_packet_handler (uint8_t packet_type, uint16_t channel, uint8_t *packet, uint16_t size)
{
    uint8_t   event;
    uint8_t   status;
    uint16_t  l2cap_cid;

    switch (packet_type) {
        case HCI_EVENT_PACKET:
            event = hci_event_packet_get_type(packet);
            switch (event) {
                case BTSTACK_EVENT_STATE:
                    if (btstack_event_state_get_state(packet) == HCI_STATE_WORKING){
                        status = l2cap_create_channel(hci_packet_handler, wiimote_addr, WIIMOTE_DATA_PSM, WIIMOTE_MTU, &l2cap_wiimote_interrupt_cid);
                        if (status){
                            printf("Connecting to Wiimote Interrupt failed: 0x%02x\n", status);
                            break;
                        }
                    }
                    break;

                case L2CAP_EVENT_CHANNEL_OPENED:
                    status = l2cap_event_channel_opened_get_status(packet);
                    if (status){
                        printf("L2CAP Connection failed: 0x%02x\n", status);
                        break;
                    }
                    l2cap_cid  = little_endian_read_16(packet, 13);
                    if (!l2cap_cid) break;
                    if (l2cap_cid == l2cap_wiimote_interrupt_cid){
                        printf("Wiimote Connection established\n");
                    }
                    break;

                case L2CAP_EVENT_CAN_SEND_NOW:
                    if(channel == l2cap_wiimote_interrupt_cid){
                        uint8_t report[] = { 0xA2, 0x11, wiimote_led };
                        l2cap_send(l2cap_wiimote_interrupt_cid, (uint8_t*) report, sizeof(report));
                    }
                    break;
                default:
                    break;
            }
            break;
        case L2CAP_DATA_PACKET:
            if (channel == l2cap_wiimote_interrupt_cid){
                printf("Wiimote Interrupt: ");
                printf_hexdump(packet, size);
                wiimote_led = (packet[2] & 0xF) << 4;
                printf("Wiimote LEDs: 0x%02x\n", wiimote_led);
                l2cap_request_can_send_now_event(l2cap_wiimote_interrupt_cid);
            } else {
                printf("Unknown channel.\n");
            }
            break;
        default:
            break;
    }
}

int btstack_main(int argc, const char * argv[]);
int btstack_main(int argc, const char * argv[]){

    (void)argc;
    (void)argv;

    wiimote_host_setup();

    // parse human readable Bluetooth address
    sscanf_bd_addr(wiimote_addr_string, wiimote_addr);

    // Turn on the device
    hci_power_control(HCI_POWER_ON);

    return 0;
}

/* EXAMPLE_END */
