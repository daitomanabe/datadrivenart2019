#include "ofMain.h"
#include "ofxZmq.h"

class ofApp : public ofBaseApp {
    ofxZmqPublisher pub;
    
    ofVideoGrabber grabber;
    ofxZmqPublisher video_pub;
    std::string key_string;
public:
    inline virtual void setup() override {
        // bind: 任意のコンピュータ(*)の36666番ポートへデータを送信する
        pub.bind("tcp://*:36666");
        // 自分のコンピュータ(127.0.0.1)の36667番ポートへデータを送信する
        video_pub.bind("tcp://127.0.0.1:36667");
        
        grabber.setup(640, 480);
        
        ofSetWindowTitle("Publisher");
    }
    inline virtual void update() override {
        grabber.update();
        if(grabber.isFrameNew()) {
            auto &&pix = grabber.getPixels();
            video_pub.send(pix.getData(), pix.size(), true);
        }
    }
    inline virtual void draw() override {
        if(ofGetMousePressed()) {
            ofBackground(220, 0, 0);
        } else {
            ofBackground(20);
        }
        ofSetColor(255);
        grabber.draw(0, 0, 320, 240);
        ofSetColor(220);
        ofDrawBitmapString("x : " + ofToString(ofGetMouseX()), 40, 40);
        ofDrawBitmapString("y : " + ofToString(ofGetMouseY()), 40, 60);
    }
    inline virtual void exit() override {
        
    }
    
    inline virtual void keyPressed(int key) override {
        ofJson json;
        json["type"] = "keyPressed";
        json["key"] = key;
        std::string data = json.dump();
        pub.send(data, true);
        
        if(isprint(key)) {
            key_string += static_cast<char>(key);
        } else if(key == OF_KEY_RETURN) {
            key_string = "";
        }
    }
    inline virtual void keyReleased(int key) override {}
    inline virtual void mouseMoved(int x, int y) override {
        ofJson json;
        json["type"] = "mouseMoved";
        json["x"] = x;
        json["y"] = y;
        std::string data = json.dump();
        pub.send(data, true);
    }
    inline virtual void mouseDragged(int x, int y, int button) override {}
    inline virtual void mousePressed(int x, int y, int button) override {}
    inline virtual void mouseReleased(int x, int y, int button) override {}
    inline virtual void mouseEntered(int x, int y) override {}
    inline virtual void mouseExited(int x, int y) override {}
    inline virtual void windowResized(int w, int h) override {}
    inline virtual void dragEvent(ofDragInfo dragInfo) override {}
    inline virtual void gotMessage(ofMessage msg) override {}
};

int main() {
    ofSetupOpenGL(640, 480, OF_WINDOW);
    ofRunApp(new ofApp());

}
