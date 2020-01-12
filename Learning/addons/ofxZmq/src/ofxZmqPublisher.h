#pragma once

#include "ofMain.h"
#include "ofxZmq.h"

class ofxZmqPublisher : public ofxZmqSocket
{
public:
	
	ofxZmqPublisher();
	
	void bind(std::string addr);
	void unbind(std::string addr);
	
	bool send(const void *data, std::size_t len, bool nonblocking = false, bool more = false);
	bool send(void *data, std::size_t len, bool nonblocking = false, bool more = false);
	bool send(const std::string& data, bool nonblocking = false, bool more = false);
	bool send(const ofBuffer& data, bool nonblocking = false, bool more = false);
	
};
