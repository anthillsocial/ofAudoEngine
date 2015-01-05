#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    // Load and parse the config file
    bool parsingSuccessful = result.open(CONFIG);
    if (parsingSuccessful){
        ofLogNotice("Loaded config:") << result.getRawString();
    }else{
         ofLogError("Config: ")  << "Failed to parse JSON" << endl;
    }
    int channels = result["audiochannels"].asString();

    // Initialise some sound control objects
	mySounder = new ofSounder*[channels];
	for (int i = 0; i < nSounders; i++){                            
	    string soundfile = "sounds/synth.wav";
	    mySounder[i] = new ofSounder(soundfile);                              
	}

	// OSC Vars: listen on the given port
	cout << "listening for osc messages on port " << PORT << "\n";
	receiver.setup(PORT);
}

//--------------------------------------------------------------
void ofApp::update(){
    
    // Update all the sound objects
    for (int i = 0; i < nSounders; i++){
        mySounder[i]->update();
    }

	// Update the sound playing system:
	ofSoundUpdate();

    // OSC check for waiting messages
	while(receiver.hasWaitingMessages()){
		
		// Get the next message
		ofxOscMessage m;
		receiver.getNextMessage(&m);

		// Load a soundfile
		if(m.getAddress() == "/load/channel"){
			int channel = m.getArgAsInt32(0);
			string soundfile = m.getArgAsString(1);
	        mySounder[channel]->load(soundfile);
			cout << "osc: /load/channel [" << channel << "] " << soundfile << "\n";
		}
	    
	    // Play soundfile
		else if(m.getAddress() == "/play/channel"){
			int channel = m.getArgAsInt32(0);
	        mySounder[channel]->play();
			cout << "osc: /play/channel [" << channel << "] " << "\n";
		}
    	
    	// Set the speed of a channel
    	else if(m.getAddress() == "/set/speed"){
    	    //float speed = 0.5f;
		    //beats.setSpeed(speed);
		    //synth.play();
		    //synth.setSpeed( 0.1f + ((float)(ofGetHeight() - y) / (float)ofGetHeight())*10);
		    //synth.setPan(ofMap(x, 0, widthStep, -1, 1, true));
        }

        // Check for a file being sent 
        // note: the size of the file depends on network buffer sizes 
        // if an file is too big the message won't come through  
        else if(m.getAddress() == "/file" ){
            ofBuffer buffer = m.getArgAsBlob(0);
            //receivedImage.loadImage(buffer); TODO: Enable audio transmission
        }

        // The message is unrecognised
		else{
			string msg_string;
			msg_string = m.getAddress();
			msg_string += ": ";
			for(int i = 0; i < m.getNumArgs(); i++){
				// get the argument type
				msg_string += m.getArgTypeName(i);
				msg_string += ":";
				// display the argument - make sure we get the right type
				if(m.getArgType(i) == OFXOSC_TYPE_INT32){
					msg_string += ofToString(m.getArgAsInt32(i));
				}
				else if(m.getArgType(i) == OFXOSC_TYPE_FLOAT){
					msg_string += ofToString(m.getArgAsFloat(i));
				}
				else if(m.getArgType(i) == OFXOSC_TYPE_STRING){
					msg_string += m.getArgAsString(i);
				}
				else{
					msg_string += "unknown";
				}
			}
			// print the message to stdout
			cout << "osc: " << msg_string << "\n";
		}

	}

}


