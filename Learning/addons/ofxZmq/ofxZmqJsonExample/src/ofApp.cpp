#include "ofMain.h"
#include "ofxZmq.h"
#include "ofxJsonUtils.h"

#include "ndarray.h"

class ofApp : public ofBaseApp {
    ofxZmqSubscriber subscriber;
    ndarray<double, 3, 2, 2> arr;
    flexarray<double, 3> farr;
public:
    void setup() {
        ofSetBackgroundColor(0);
        ofSetColor(255);
        subscriber.connect("ipc:///tmp/numpy_json");
    }
    void update() {
        std::string json_str;
        while(subscriber.hasWaitingMessage()) {
            subscriber.getNextMessage(json_str);
            
            ofLogNotice() << "received data!!";
            ofJson json = ofxJsonUtils::parse(json_str);
            
            ofLogNotice() << "parse to arr";
            ofxJsonUtils::parse(json, arr);
            
            for(std::size_t i = 0; i < arr.size(); i++) {
                for(std::size_t j = 0; j < arr[i].size(); j++) {
                    for(std::size_t k = 0; k < arr[i][j].size(); k++) {
                        std::cout << arr[i][j][k] << ", ";
                    }
                    std::cout << std::endl;
                }
                std::cout << std::endl;
            }
            
            ofLogNotice() << "parse to farr";
            ofxJsonUtils::parse(json, farr);
            
            for(std::size_t i = 0; i < farr.size(); i++) {
                for(std::size_t j = 0; j < farr[i].size(); j++) {
                    for(std::size_t k = 0; k < farr[i][j].size(); k++) {
                        std::cout << farr[i][j][k] << ", ";
                    }
                    std::cout << std::endl;
                }
                std::cout << std::endl;
            }
        }
    }
    void draw() {
        std::ostringstream ss;
        ss << "[" << std::endl;
        for(std::size_t i = 0; i < arr.size(); i++) {
            ss << "  [" << std::endl;
            for(std::size_t j = 0; j < arr[i].size(); j++) {
                ss << "    [ ";
                for(std::size_t k = 0; k < arr[i][j].size(); k++) {
                    ss << arr[i][j][k] << " ";
                }
                ss << "]" << std::endl;
            }
            ss << "  ]" << std::endl;
        }
        ss << "]";
        ofDrawBitmapString(ss.str(), 20, 20);
    }
};

int main() {
    ofSetupOpenGL(300, 300, OF_WINDOW);
    ofRunApp(new ofApp());
}
