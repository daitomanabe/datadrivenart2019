# ZMQ について

## ZeroMQ

* オフィシャルサイト: [ZeroMQ](https://zeromq.org)
* Message Queueライブラリの一種
  * 同様のものとして MQTT, nanomsg 等がある
* Brokerlessモデル
  * 要するに間にサーバーが要らない
  * 反対にサーバーが必要なものをBrokeredモデルと呼ぶ
* ここらへんも説明しだすと1日掛けても終わらないので興味がある人は [ZeroMQ覚書](https://qiita.com/gwappa/items/9677e1ea4adcf2d457f4) 等を参照のこと
  * もっと気になる人は [ØMQガイドブック(日本語版)](https://www.cuspy.org/diary/2015-05-07-zmq/) を
* [ofxZmq](https://github.com/satoruhiga/ofxZmq) で使われているのは pub/sub と呼ばれるデザインパターン（req/rep もあるが今回は省略）
  * ofxPubSubOscはこれを参考に作ったつもり
    * 結果的にはだいぶ違う形になったが概念的には踏襲しているはず...

----

## Pros.

* pythonでも使える
* サイズの大きなバイナリを扱える
* TCP通信やIPC (Inter-Process Communication) 通信も使える
  * UDPも使えるがUDPでやるならOSCの方が楽
* TCPやIPCであればデータ欠けが起こらない
* 基本的にはバイナリになる
  * データフォーマットは自分で決める
    * 自由度が高い

----

## Cons.

* 基本的にはバイナリになる
  * データフォーマットは自分で決める
    * 自由度が高過ぎる

----

## ofxZmq

* 本来の ZeroMQ から必要なものを絞ってシンプルなインターフェイスにしている
  * 痒いところには手が届かないことがある
  * 2020/01/11現在, libzmqの最新のリリースは [4.3.2](https://github.com/zeromq/libzmq/releases/tag/v4.3.2) だがofxZmqのライブラリは4.1.4 （2015/12/18リリース）
    * このバージョンのlibzmqだと再接続がうまく働かないことがあるためサーバー側（送り側）を先に立ち上げないとうまく繋がらないことが多い
    * 新しいバージョンに差し替えたもの作ろうと思い立ってもう2年くらい経った

----

## pub/sub

* publisher
  * データを送る側

* subscriber
  * データを受ける側
* サーバー/クライアントモデル
  * zmq では送り側がサーバー, 受け側がクライアントとなる
  * 1つの publisher に対し, subscriber は複数作ることが出来る
  * 複数の publisher に対し1つの subscriber や多対多の通信をしたい場合は XPub/XSub という形式を使う必要がある（が, 今の ofxZmq は未対応）

