#include "mbed.h"
#include "MQTTClient.h"
#include "MQTTEthernet.h"
#include "rtos.h"
#include "k64f.h"

// connect options for MQTT broker
#define BROKER "broker.mqttdashboard.com"   // MQTT broker URL
#define PORT 1883                           // MQTT broker port number
#define CLIENTID ""                         // use K64F MAC address without colons
#define USERNAME ""                         // not required for MQTT Dashboard public broker 
#define PASSWORD ""                         // not required for MQTT Dashboard public broker
#define TOPIC ""                            // MQTT topic

Queue<uint32_t, 6> messageQ;

// LED color control function
void controlLED(color_t led_color) {
    switch(led_color) {
        case red :
            greenLED = blueLED = 1;          
            redLED = 0.7;
            break;
        case green :
            redLED = blueLED = 1;
            greenLED = 0.7;
            break;
        case blue :
            redLED = greenLED = 1;
            blueLED = 0.7;
            break;
        case off :
            redLED = greenLED = blueLED = 1;
            break;
    }
}
    
// Switch 2 interrupt handler
void sw2_ISR(void) {
    messageQ.put((uint32_t*)22);
}

// Switch3 interrupt handler
void sw3_ISR(void) {
    messageQ.put((uint32_t*)33);
}
 
// MQTT message arrived callback function
void messageArrived(MQTT::MessageData& md) {
    MQTT::Message &message = md.message;
    pc.printf("Receiving MQTT message:  %.*s\r\n", message.payloadlen, (char*)message.payload);
    
    if (message.payloadlen == 3) {
        if (strncmp((char*)message.payload, "red", 3) == 0)
            controlLED(red);
        
        else if(strncmp((char*)message.payload, "grn", 3) == 0)
            controlLED(green);
        
        else if(strncmp((char*)message.payload, "blu", 3) == 0)
            controlLED(blue);
        
        else if(strncmp((char*)message.payload, "off", 3) == 0)
            controlLED(off);
    }        
}

namespace active_low
{
    const bool on = false;
    const bool off = true;
}

DigitalOut g_led_red(LED1);
DigitalOut g_led_green(LED2);
DigitalOut g_led_blue(LED3);

InterruptIn g_button_resetCapacity(SW3);
InterruptIn g_button_increaseCapacity(SW2);

//capacity variables
int currCapacity;
int maxCapacity;

//metronome *metronomeObj = new metronome();

float delay = 0, pulse = 0.1f;

void on_reset()
{
    //decrease in capacity
    currCapacity=0;
    g_led_red = active_low::on;
    wait(pulse);
    g_led_red = active_low::off;
}

void on_increase()
{
    //increase in capacity
    currCapacity++; 
    g_led_green = active_low::on;
    wait(pulse);
    g_led_green = active_low::off;
}

int main() {
    // turn off LED  
    controlLED(off);
    
    // set SW2 and SW3 to generate interrupt on falling edge 
    switch2.fall(&sw2_ISR);
    switch3.fall(&sw3_ISR);
    
    pc.printf("\r\n\r\nWelcome to the K64F MQTT Demo!\r\n");
    pc.printf("\r\nAttempting connect to local network...\r\n");
        
    // initialize ethernet interface
    MQTTEthernet ipstack = MQTTEthernet();
    
    // get and display client network info
    EthernetInterface& eth = ipstack.getEth();
    pc.printf("IP address is %s\r\n", eth.getIPAddress());
    pc.printf("MAC address is %s\r\n", eth.getMACAddress());
    pc.printf("Gateway address is %s\r\n", eth.getGateway());
    
    // construct the MQTT client
    MQTT::Client<MQTTEthernet, Countdown> client = MQTT::Client<MQTTEthernet, Countdown>(ipstack);
    
    char* hostname = BROKER;
    int port = PORT;
    int rc;
    
    pc.printf("\r\nAttempting TCP connect to %s:%d:  ", hostname, port);
    
    // connect to TCP socket and check return code
    if ((rc = ipstack.connect(hostname, port)) != 0)
        pc.printf("failed: rc= %d\r\n", rc);
        
    else
        pc.printf("success\r\n");
    
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;       
    data.MQTTVersion = 3;
    data.clientID.cstring = CLIENTID;
//    data.username.cstring = USERNAME;
//    data.password.cstring = PASSWORD;
    
    // send MQTT connect packet and check return code
    pc.printf("Attempting MQTT connect to %s:%d: ", hostname, port);
    if ((rc = client.connect(data)) != 0)
        pc.printf("failed: rc= %d\r\n", rc);
        
    else
        pc.printf("success\r\n");
        
    char* topic = TOPIC;
    
    // subscribe to MQTT topic
    pc.printf("Subscribing to MQTT topic %s: ", topic);
    if ((rc = client.subscribe(topic, MQTT::QOS0, messageArrived)) != 0)
        pc.printf("failed: rc= %d\r\n", rc);
        
    else
        pc.printf("success\r\n");
        
    MQTT::Message message;
    char buf[100];
    message.qos = MQTT::QOS0;
    message.retained = false;
    message.dup = false;
    message.payload = (void*)buf;
    message.payloadlen = strlen(buf)+1;
    
    while(true) {
        osEvent switchEvent = messageQ.get(100);
        
        if (switchEvent.value.v == 22 || switchEvent.value.v == 33) {
            switch(switchEvent.value.v) {
                case 22 :
                    sprintf(buf, "sw2");
                    break;
                case 33 :
                    sprintf(buf, "sw3");
                    break;
            }
            pc.printf("Publishing MQTT message: %.*s\r\n", message.payloadlen, (char*)message.payload);
            rc = client.publish(topic, message);
            client.yield(100);
        }
        
        else {
            client.yield(100);
        }           
    }
    
    // Seed the RNG for networking purposes
    //unsigned seed = utils::entropy_seed();
//    srand(seed);
    
    

    // LEDs are active LOW - true/1 means off, false/0 means on
    // Use the constants for easier reading
    g_led_red = active_low::off;
    g_led_green = active_low::off;
    g_led_blue = active_low::off;

    // Button falling edge is on push (rising is on release)
    g_button_resetCapacity.fall(&on_reset);
    g_button_increaseCapacity.fall(&on_increase);
    
    //giving capacity vars value
    currCapacity = 0;
    maxCapacity = 5;
    
    while(true){
        
        if(currCapacity == maxCapacity){
            g_led_blue = active_low::on;
        }
        
        else if(currCapacity > maxCapacity){
            g_led_blue = active_low::off;
            while(currCapacity > maxCapacity){
                g_led_red = active_low::on;
                wait(1);
                g_led_red = active_low::off;
                wait(1);
            }
        }
        else{
            g_led_red = active_low::off;
            g_led_green = active_low::off;
            g_led_blue = active_low::off;
        }
    }
    
    
    return 0;
}
//#ifdef IOT_ENABLED
//  // Turn on the blue LED until connected to the network
//    g_led_blue = active_low::on;
//
//  // Need to be connected with Ethernet cable for success
//    EthernetInterface ethernet;
//    if (ethernet.connect() != 0)
//        return 1;
//
//  // Pair with the device connector
//    frdm_client client("coap://api.connector.mbed.com:5684", &ethernet);
//    if (client.get_state() == frdm_client::state::error)
//        return 1;
//
//  // The REST endpoints for this device
//  // Add your own M2MObjects to this list with push_back before client.connect()
//    M2MObjectList objects;
//    
//
//    M2MDevice* device = frdm_client::make_device();
//    objects.push_back(device);
//
//    
//    M2MObject* frequency_object = M2MInterfaceFactory::create_object("3318");
//    M2MObjectInstance* frequency_object_instance = frequency_object->create_object_instance();
//    
//    M2MResource* set_point_value = frequency_object_instance->create_dynamic_resource("5900", "Set",
//      M2MResourceInstance::STRING, true);
//    set_point_value->set_operation(M2MBase::GET_PUT_ALLOWED);
//    set_point_value->set_value((uint8_t*)"0", 1);
//    
//    M2MResource* min = frequency_object_instance->create_dynamic_resource("5601", "Min",
//      M2MResourceInstance::STRING, true);
//  min->set_operation(M2MBase::GET_PUT_ALLOWED);
//  min->set_value((uint8_t*)"0", 1);
//  
//  M2MResource* max = frequency_object_instance->create_dynamic_resource("5602", "Max",
//      M2MResourceInstance::STRING, true);
//  max->set_operation(M2MBase::GET_PUT_ALLOWED);
//  max->set_value((uint8_t*)"0", 1);
//  
//  M2MResource* units = frequency_object_instance->create_dynamic_resource("5701", "Units",
//      M2MResourceInstance::STRING, true);
//  units->set_operation(M2MBase::GET_PUT_ALLOWED);
//  units->set_value((uint8_t*)"BPM", 3);
//
//  objects.push_back(frequency_object);
//  
//  
//  // Publish the RESTful endpoints
//    client.connect(objects);
//
//  // Connect complete; turn off blue LED forever
//    g_led_blue = active_low::off;
//#endif
//
//    while (true)
//    {
//#ifdef IOT_ENABLED
//        if (client.get_state() == frdm_client::state::error)
//            break;
//#endif
//      
//        // Insert any code that must be run continuously here
//        
//      char buffer[20];
//        int size;
//        if(!metronomeObj->is_timing()){
//            size = sprintf(buffer,"%d",metronomeObj->get_bpm());
//            set_point_value->set_value((uint8_t*)buffer, size);
//          //set_point_value->set_value((uint8_t*) m->get_bpm(), 1);
//          delay = metronomeObj->get_delay() / 1000.0f;
//          if(delay != 0){
//              g_led_green = active_low::on;
//              wait(pulse);
//              g_led_green = active_low::off;
//              wait(delay - pulse);    
//          }               
//        }
//    }
//
//#ifdef IOT_ENABLED
//    client.disconnect();
//#endif
//
//    return 1;
//}

