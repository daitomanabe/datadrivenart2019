#include "ofMain.h"
#include "ofxOsc.h"

class ofApp : public ofBaseApp {
    ofxOscSender sender;
public:
    void setup() {
        sender.setup("localhost", 26666);
        ofSetWindowPosition(0, 0);
        ofSetWindowTitle("OscSender");
    }
    void update() {
        
    }
    void draw() {
        
    }
    
    void keyPressed(int key) {
        switch(key) {
            case 'd': {
                ofxOscMessage m;
                m.setAddress("/double");
                m.addDoubleArg(1.2);
                sender.sendMessage(m);
                break;
            }
            case 'f': {
                ofxOscMessage m;
                m.setAddress("/float");
                m.addFloatArg(1.1f);
                sender.sendMessage(m);
                break;
            }
            case 'l': {
                ofxOscMessage m;
                m.setAddress("/long");
                m.addInt64Arg(1);
                sender.sendMessage(m);
                break;
            }
            case 'i': {
                ofxOscMessage m;
                m.setAddress("/int");
                m.addInt32Arg(2);
                sender.sendMessage(m);
                break;
            }
            case 's': {
                ofxOscMessage m;
                m.setAddress("/string");
                m.addStringArg("string");
                m.addSymbolArg("symbol");
                sender.sendMessage(m);
                break;
            }
            case 'T': {
                ofxOscMessage m;
                m.setAddress("/t");
                m.addBoolArg(true);
                sender.sendMessage(m);
                break;
            }
            case 'F': {
                ofxOscMessage m;
                m.setAddress("/f");
                m.addBoolArg(false);
                sender.sendMessage(m);
                break;
            }
        }
    }
    
    void mouseMoved(int x, int y) {
        ofxOscMessage m;
        m.setAddress("/mouseMoved");
        m.addInt32Arg(x);
        m.addInt32Arg(y);
        sender.sendMessage(m);
    }
    void mousePressed(int x, int y, int button) {
        
    }
    void mouseReleased(int x, int y, int button) {
        
    }
};

int main() {
    ofSetupOpenGL(360, 640, OF_WINDOW);
    ofRunApp(new ofApp());

}
