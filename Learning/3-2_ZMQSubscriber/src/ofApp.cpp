#include "ofMain.h"
#include "ofxZmq.h"

class ofApp : public ofBaseApp {
    ofxZmqSubscriber sub;
    ofxZmqSubscriber video_sub;
    int x, y;
    std::string key_string;
    ofImage received_image;
public:
    inline virtual void setup() override {
        // connect: 自分のコンピュータ(127.0.0.1)の36666番ポートにデータを取得しにいく
        sub.connect("tcp://127.0.0.1:36666");
        // 自分のコンピュータ(127.0.0.1)の36667番ポートにデータを取得しにいく
        video_sub.connect("tcp://127.0.0.1:36667");

        ofSetBackgroundColor(20);
        ofSetColor(220);
        ofSetWindowTitle("Subscriber");
        received_image.allocate(640, 480, OF_IMAGE_COLOR);
    }
    inline virtual void update() override {
        // ofxOscReceiver と似たような構造
        while(sub.hasWaitingMessage()) {
            std::string data;
            sub.getNextMessage(data);
            
            // 受信後は自分で決めたフォーマットに従って処理をする
            ofJson json = ofJson::parse(data);
            std::string type = json["type"];
            if(type == "keyPressed") {
                int key = json["key"];
                if(isprint(key)) {
                    key_string += static_cast<char>(key);
                } else if(key == OF_KEY_RETURN) {
                    key_string = "";
                }
            }
            if(type == "mouseMoved") {
                x = json["x"];
                y = json["y"];
            }
        }
        
        while(video_sub.hasWaitingMessage()) {
            ofBuffer data;
            video_sub.getNextMessage(data);
            received_image.setFromPixels((unsigned char *)data.getData(), 640, 480, OF_IMAGE_COLOR);
        }
    }
    inline virtual void draw() override {
        ofSetColor(255);
        received_image.draw(0, 0, 320, 240);
        ofSetColor(220);
        ofDrawCircle(x, y, 10);
        ofDrawBitmapString("x : " + ofToString(x), 40, 40);
        ofDrawBitmapString("y : " + ofToString(y), 40, 60);
        ofDrawBitmapString(key_string, 40, 60);
    }
    inline virtual void exit() override {
        
    }
    
    inline virtual void keyPressed(int key) override {}
    inline virtual void keyReleased(int key) override {}
    inline virtual void mouseMoved(int x, int y) override {}
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
