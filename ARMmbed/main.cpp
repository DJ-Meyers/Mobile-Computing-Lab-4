#include "mbed.h"
#include "utils.hpp"

#include "EthernetInterface.h"
#include "frdm_client.hpp"

//#include "capacity.hpp"

#define IOT_ENABLED

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

int main()
{
	// Seed the RNG for networking purposes
    unsigned seed = utils::entropy_seed();
    srand(seed);
    
    

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
//	// Turn on the blue LED until connected to the network
//    g_led_blue = active_low::on;
//
//	// Need to be connected with Ethernet cable for success
//    EthernetInterface ethernet;
//    if (ethernet.connect() != 0)
//        return 1;
//
//	// Pair with the device connector
//    frdm_client client("coap://api.connector.mbed.com:5684", &ethernet);
//    if (client.get_state() == frdm_client::state::error)
//        return 1;
//
//	// The REST endpoints for this device
//	// Add your own M2MObjects to this list with push_back before client.connect()
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
//    	M2MResourceInstance::STRING, true);
//    set_point_value->set_operation(M2MBase::GET_PUT_ALLOWED);
//    set_point_value->set_value((uint8_t*)"0", 1);
//    
//    M2MResource* min = frequency_object_instance->create_dynamic_resource("5601", "Min",
//    	M2MResourceInstance::STRING, true);
//	min->set_operation(M2MBase::GET_PUT_ALLOWED);
//	min->set_value((uint8_t*)"0", 1);
//	
//	M2MResource* max = frequency_object_instance->create_dynamic_resource("5602", "Max",
//		M2MResourceInstance::STRING, true);
//	max->set_operation(M2MBase::GET_PUT_ALLOWED);
//	max->set_value((uint8_t*)"0", 1);
//	
//	M2MResource* units = frequency_object_instance->create_dynamic_resource("5701", "Units",
//		M2MResourceInstance::STRING, true);
//	units->set_operation(M2MBase::GET_PUT_ALLOWED);
//	units->set_value((uint8_t*)"BPM", 3);
//
//	objects.push_back(frequency_object);
//	
//	
//	// Publish the RESTful endpoints
//    client.connect(objects);
//
//	// Connect complete; turn off blue LED forever
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
//    	char buffer[20];
//        int size;
//        if(!metronomeObj->is_timing()){
//            size = sprintf(buffer,"%d",metronomeObj->get_bpm());
//            set_point_value->set_value((uint8_t*)buffer, size);
//        	//set_point_value->set_value((uint8_t*) m->get_bpm(), 1);
//        	delay = metronomeObj->get_delay() / 1000.0f;
//        	if(delay != 0){
//        		g_led_green = active_low::on;
//	        	wait(pulse);
//	        	g_led_green = active_low::off;
//        		wait(delay - pulse);	
//        	}        		
//        }
//    }
//
//#ifdef IOT_ENABLED
//    client.disconnect();
//#endif
//
//    return 1;
//}
