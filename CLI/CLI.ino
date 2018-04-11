#include <utils.h>
#define DEBUG 6
int pos = 0;
int t = getDeviceID();
int type = getDeviceType();

void err(int errCode){
	errorBlink();
	if(errCode == 201){

	}
}

void sen_c(){
	if(getChoice() == 1){
      	//add device
  	  	uint8_t pre = getPrevAddress();

      	send_dev_params();

      	setPrevAddress(pre);
  	}else if(getChoice() == 2){
      	//get previous device address so we can djust after the sync
      	uint8_t pre = getPrevAddress();
      	//synchronize devices
      	sync();
      	//correct the previous address
      	setPrevAddress(pre);
  	}else if(getChoice() == 4){
    	if(getDeviceType() == 1){
        	//this is a starter
        	starter();
        	if(switchTo(SER)){
            	setSendHandler(starter_send_start_millis);
            	setSendCallback(starter_send_start_millis_cb);
            	send();
            	if(getRaceMode() == 3){
        			if(switchTo(LORA)){
        				//wiat for the pace time to be sent back to us
        				setReceiveHandler(rec_pace_time);
        				setReceiveCallback(rec_pace_time_cb);

        				while(!available()); //wait
        				receive();
        				
        				if(switchTo(SER)){
        					send_pace_time_init();
        				}
					}
        		}
        	}

    	}else if(getDeviceType() == 2){
        	//this is an interval
    	}else if(getDeviceType() == 3){
        	//this is monitor node
        
    	}
  	}
}

void rec_c(){
  if(isI2COriginating()){
  	setI2COriginating(false);
    if(getChoice() == 1){
        get_add_device_parameters();
    }else if(getChoice() == 2){
         setRecv(true);
    }else if(getChoice() == 200){
        send_dev_info();
    }else if(getChoice() == 201){
        setSendHandler(sen);
        setSendCallback(sen_c);
    }else if(getChoice() == 4){
        //race mode
        timing_mode();
    }else if(getChoice() == 202){
        setReceiveHandler(rec_start_params);
        setReceiveCallback(rec_start_params_cb);
    }else if(getChoice() == 204){
        setRecv(true);
    }
  }else{ 

  	//we're in LoRa mode here
    //here we need to check if this packet is supposed to come to us
    if(getNextAddress() != 255 || getPrevAddress() != 255){
        //there are only two nodes in the network this one and one other 
        if(pos != getMyPosition()){
            //this message was not intended for us
        }
    }else if (getNextAddress() != 255 && getPrevAddress() != 255){
        //there is a distinct next node and previous node
        if(pos != getMyPosition()){
            //this message was not intended for us
        }
    }

    if(getChoice() == 1){
      //1 == add device
      //set handlers/callbacks
      get_add_device_parameters();
      
      //receive the device ID being added
      while(!available());
      receive();


      //recieve the position it's being added at
      while(!available());
      receive();
        
      //if the device to add is this device change this position to the new position
      if(getAddDevice() == getThisAddress()){
         setMyPosition(getAddPosition());
      }
        
      //send info to the RPi
      wake_up_pi();
      
      //check if we woke up the Pi
      if(isPiAwake()){

        //send the choice of what we're doing
        make_choice();
          
        //check if the choice was sent
        if(wasChoiceSent()){
            //send the paramaters of the device to add
            if(switchTo(SER)){
            	send_dev_params();
        	}
        }
      }
    }else if(getChoice() == 2){
        sync_rec();
        
        while(!available());
        
        receive();
    }else if(getChoice() == 4){
        if(getDeviceType() == 1){
            //this is a starter
            
        }else if(getDeviceType() == 2){
            //this is an interval
            interval_timer();
            
            //recieve bib number
            while(!available());
            receive();
            
            //receive start time
            while(!available());
            receive();

            //receive race mode
            while(!available());
            receive();
            
            
        }else if(getDeviceType() == 3){
            //this is monitor node
            
        }
    }
  }
}

void rec(Packet * p){
  pos = p->position;
  setChoice(*(int*) p->data);
}

Packet * sen(){
  int g = getChoice();
  int next = getMyPosition() + 1;
  if(g == 1){
    setErrorFunction(sen_dev_params_err);
    setSendAddress(getAddDevice());
    next = getAddPosition();
    setI2COriginating(false);
    switchTo(LORA);
  }else if(g == 2){
      if(getMyPosition() != 1){
          next = getMyPosition() - 1;
      }
  }else if(g == 4){
  	setI2COriginating(false);
    switchTo(LORA);
    int state = digitalRead(START);
    while(state == 0){
        state = digitalRead(START);
    }
    setStartTime(getTime());
  }
  return new Packet(&g, INT, sizeof(g), next);
}

void debugBlink(){
	digitalWrite(6, HIGH);
	delay(500);
	digitalWrite(6, LOW);
}

void resetBlink(){
	digitalWrite(OUT, HIGH);
	digitalWrite(IN, HIGH);
	digitalWrite(ERR, HIGH);
	delay(500);
	digitalWrite(OUT, LOW);
	digitalWrite(IN, LOW);
	digitalWrite(ERR, LOW);
}

void reset(){
  	setBlocking(false);
  	setI2COriginating(false);
  	setThisAddress(t);
  	setPiAwake(false);
  	setChoiceSent(false);
  	setRecv(false);
  	setHasFoundDistance(false);
  	setResetStatus(false);
  	setCurrentError(0);
  	setChoice(0);
  	setSendAddress(255);
  	setReceiveCallback(rec_c);
  	setSendCallback(sen_c);
  	setHandlers(rec, sen);
  	setErrorFunction(err);
  	setResetFunction(reset);
  	switchTo(LORA);
}

void setup() {
  Serial.begin(115200);
  pinMode(START, INPUT_PULLUP);
  while (!Serial);
  initialize();
  initEye();
  resetBlink();
  reset();
}

void loop() {
	 /*this waits until either an I2C master has sent some information, or there is information
  available via LoRa connection, if hasRecv() then it's I2C, if available() then it's LoRa
  */
  	if(available() || hasRecv() || getResetStatus()){
  		bool skip = (hasRecv || available()) ? false : true;
  		if(getChoice() == 204){
  			skip = true;
			find_timing_distance();
			while(!hasFoundDistance()){
				getEye()->distance();  
			}
    		setChoice(0);
  		}
  		if(!skip){
  			setReceiveCallback(rec_c);
  			setSendCallback(sen_c);
  			setHandlers(rec, sen);
  			if(hasRecv()){
    			//I2C, but sends via LoRa
    			if(switchTo(LORA)){
        			send();
    			}
  			}else{
    			//LoRa
    			receive();
  			}
		}
		reset();
	}
}