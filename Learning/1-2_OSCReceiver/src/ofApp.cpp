#include "ofMain.h"
#include "ofxOsc.h"

#include "ScreenConsole.h"

class ofApp : public ofBaseApp {
    ofxOscReceiver receiver;
    int x;
    int y;
    
    ScreenConsole console;
public:
    void setup() {
        receiver.setup(26666);
        ofSetWindowPosition(360, 0);
        ofSetWindowTitle("OscReceiver");
    }
    void update() {
        // OSCメッセージを受信してる場合 receiver.hasWaitingMessages() が true になる
        // メッセージを処理し続けて溜まっているメッセージが空になったら false になってwhileを抜ける
        while(receiver.hasWaitingMessages()) {
            ofxOscMessage m;
            
            // 上で準備した ofxOscMessage に受信したメッセージをか格納する
            receiver.getNextMessage(m);
            
            // OSCメッセージの宛先 (/hogehoge) を取得
            std::string address = m.getAddress();
            console("address") << address << " " << m.getNumArgs();
            
            if(address == "/double") {
                 // 最初の引数の型を確認, dobule型を表す d が入っている
                console(address) << m.getArgTypeName(0);
                 // double型 の値を取得
                console(address) << m.getArgAsDouble(0);
            } else if(address == "/float") {
                 // 最初の引数の型を確認, float型を表す f が入っている
                console(address) << m.getArgTypeName(0);
                 // double型 の値を取得
                console(address) << m.getArgAsFloat(0);
            } else if(address == "/string") {
                 // 最初の引数の型を確認, string型を表す s が入っている
                console(address) << m.getArgTypeName(0);
                 // string型の値を取得
                console(address) << m.getArgAsString(0);
                 // 2個目の引数の型を確認, symbol型を表す S が入っている
                console(address) << m.getArgTypeName(1);
                 // symbol型 (oFではstring型になる) の値を取得
                console(address) << m.getArgAsSymbol(1);
            } else if(address == "/t") {
                 // 最初の引数の型を確認, true 型を表す T が入っている
                console(address) << m.getArgTypeName(0);
                 // bool型の値を取得
                console(address) << m.getArgAsBool(0);
            } else if(address == "/f") {
                 // 最初の引数の型を確認, false 型を表す F が入っている
               console(address) << m.getArgTypeName(0);
                 // bool型の値を取得
               console(address) << m.getArgAsBool(0);
            } else if(address == "/mouseMoved") {
                x = m.getArgAsInt32(0);
                y = m.getArgAsInt32(1);
                console(address) << x << ", " << y;
            }
        }
    }
    virtual void draw() {
        ofSetColor(255);
        ofDrawCircle(x, y, 10);
        console.print(40, 40);
    }
};

int main() {
    ofSetupOpenGL(360, 640, OF_WINDOW);
    ofRunApp(new ofApp());
}

