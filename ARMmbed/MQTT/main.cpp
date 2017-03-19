#include "mbed.h"
#include "MQTTClient.h"
#include "MQTTEthernet.h"
#include "rtos.h"
#include "k64f.h"

// connect options for MQTT broker
#define BROKER "192.168.0.104"   // MQTT broker URL, original"broker.mqttdashboard.com"
#define PORT 1883                           // MQTT broker port number
#define CLIENTID "baecons2"                         // use K64F MAC address without colons
#define USERNAME ""                         // not required for MQTT Dashboard public broker 
#define PASSWORD ""                         // not required for MQTT Dashboard public broker
#define TOPIC "hotbaecons"                            // MQTT topic

Queue<uint32_t, 6> messageQ;

// LED color control function 
void controlLED(color_t led_color) {
    switch(led_color) {
        case red :
            greenLED = blueLED = 1;          
            redLED = 0.7;
            wait(1.0f);
            break;
        case green :
            redLED = blueLED = 1;
            greenLED = 0.7;
            wait(1.0f);
            break;
        case blue :
            redLED = greenLED = 1;
            blueLED = 0.7;
            wait(1.0f);
            break;
        case off :
            redLED = greenLED = blueLED = 1;
            break;
        }
}
    
// Switch 2 ibutton click add person
void sw2_ISR(void) {
    messageQ.put((uint32_t*)1);
    //first number is to increase the count of people in the region
    //second number is to denote which region the board is in
}

// Switch3 same as SW2
void sw3_ISR(void) {
   messageQ.put((uint32_t*)1);
}
 
// MQTT message arrived callback function
// MQTT message received after message sent to the broker
void messageArrived(MQTT::MessageData& md) {
    MQTT::Message &message = md.message;
    pc.printf("Receiving MQTT message:  %.*s\r\n", message.payloadlen, (char*)message.payload);
    redLED = greenLED = .7;
    wait(0.5f);
    redLED = greenLED = 1;
    blueLED = .7;
    wait(0.5f);
    blueLED = 1;
    wait(0.5f);
    
    if (message.payloadlen == 3) {
        if (strncmp((char*)message.payload, "red", 3) == 0){
            controlLED(red);
            wait(1.0f);
            }
        
        else if(strncmp((char*)message.payload, "grn", 3) == 0){
            controlLED(green);
            wait(1.0f);
            }
        
        else if(strncmp((char*)message.payload, "blu", 3) == 0){
            controlLED(blue);
            wait(1.0f);
            }
        
        else if(strncmp((char*)message.payload, "off", 3) == 0){
            controlLED(off);
            wait(1.0f);
            }
    }        
}

int main() {
    // turn off LED  
    controlLED(off);
    controlLED(red);
    wait(1.0f);
    controlLED(off);
    
    // set SW2 and SW3 to generate interrupt on falling edge 
    switch2.fall(&sw2_ISR);
    switch3.fall(&sw3_ISR);
    
    pc.printf("\r\n\r\nWelcome to the K64F MQTT Demo!\r\n");
    pc.printf("\r\nAttempting connect to local network...\r\n");
        
    // initialize ethernet interface
    MQTTEthernet ipstack = MQTTEthernet();
    
    controlLED(red);
    wait(1.0f);
    controlLED(off);
    
    // get and display client network info
    EthernetInterface& eth = ipstack.getEth();
    pc.printf("IP address is %s\r\n", eth.getIPAddress());
    pc.printf("MAC address is %s\r\n", eth.getMACAddress());
    pc.printf("Gateway address is %s\r\n", eth.getGateway());
    
    controlLED(red);
    wait(1.0f);
    controlLED(off);
    
    // construct the MQTT client
    MQTT::Client<MQTTEthernet, Countdown> client = MQTT::Client<MQTTEthernet, Countdown>(ipstack);
    
    controlLED(blue);
    wait(1.0f);
    controlLED(off);
    wait(1.0f);
    
    char* hostname = BROKER;
    int port = PORT;
    int rc;
    
    controlLED(blue);
    wait(1.0f);
    controlLED(off);
    
    
    pc.printf("\r\nAttempting TCP connect to %s:%d:  ", hostname, port);
    
    // connect to TCP socket and check return code
    if ((rc = ipstack.connect(hostname, port)) != 0){
        pc.printf("failed: rc= %d\r\n", rc);
        controlLED(red);
        wait(1.0f);
        controlLED(off);
        }
        
    else{
        pc.printf("success\r\n");
        controlLED(green);
        wait(1.0f);
        controlLED(off);
        }
        
    controlLED(blue);
    wait(1.0f);
    controlLED(off);
    
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;       
    data.MQTTVersion = 3;
    data.clientID.cstring = CLIENTID;
    //dont need username and pass since using a public broker
//    data.username.cstring = USERNAME;
//    data.password.cstring = PASSWORD;

    wait(1.0f);
    controlLED(blue);
    wait(1.0f);
    controlLED(off);
    
    // send MQTT connect packet and check return code
    pc.printf("Attempting MQTT connect to %s:%d: ", hostname, port);
    if ((rc = client.connect(data)) != 0){
        pc.printf("failed: rc= %d\r\n", rc);
        controlLED(red);
        wait(1.0f);
        controlLED(off);
        }
        
    else{
        pc.printf("success\r\n");
        controlLED(green);
        controlLED(red);
        wait(1.0f);
        controlLED(off);
        }
        
    char* topic = TOPIC;
    
    // subscribe to MQTT topic
    pc.printf("Subscribing to MQTT topic %s: ", topic);
    if ((rc = client.subscribe(topic, MQTT::QOS0, messageArrived)) != 0){
        pc.printf("failed: rc= %d\r\n", rc);
        controlLED(red);
        wait(1.0f);
        controlLED(off);
        }
        
    else{
        pc.printf("success\r\n");
        wait(1.0f);
        controlLED(green);
        wait(1.0f);
        controlLED(off);
        }
        
    MQTT::Message message;
    //buf contains two values: [beaconId, incrementAmt]
    char buf[2] = {'1', '1'};
    message.qos = MQTT::QOS0;
    message.retained = false;
    message.dup = false;
    message.payload = (void*)buf;
    message.payloadlen = strlen(buf)+1;
    
    while(true) {
        osEvent switchEvent = messageQ.get(100);
        
        if (switchEvent.value.v == 1) {
            switch(switchEvent.value.v) {
                case 1 :
                    sprintf(buf, "sw2");
                }
            pc.printf("Publishing MQTT message: %.*s\r\n", message.payloadlen, (char*)message.payload);
            rc = client.publish(topic, message);
            controlLED(green);
            wait(1.0f);
            controlLED(off);
            client.yield(100);
        }
        
        else {
            client.yield(100);
        }           
    }
}