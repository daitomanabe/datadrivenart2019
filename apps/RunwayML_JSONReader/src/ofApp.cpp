#include "ofMain.h"

class ofApp : public ofBaseApp {

public:
    inline virtual void setup() override {
        
    }
    inline virtual void update() override {
        
    }
    inline virtual void draw() override {
        
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
    ofSetupOpenGL(1280, 720, OF_WINDOW);
    ofRunApp(new ofApp());

}
