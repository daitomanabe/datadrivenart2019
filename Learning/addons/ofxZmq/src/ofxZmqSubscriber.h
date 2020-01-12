#pragma once

#include "ofMain.h"
#include "ofxZmq.h"

class ofxZmqSubscriber : public ofxZmqSocket
{
public:
	
	ofxZmqSubscriber();
	
	void connect(std::string addr);
	void setFilter(std::string filter);
	void disconnect(std::string addr);

	bool receive(std::string &data);
	bool receive(ofBuffer &data);
	
	bool hasWaitingMessage(long timeout_millis = 0);
	bool getNextMessage(std::string &data);
	bool getNextMessage(ofBuffer &data);

private:
	
	std::string filter;
	
};
