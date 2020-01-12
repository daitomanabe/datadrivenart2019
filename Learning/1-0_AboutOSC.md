# OSC について

## OpenSoundControl

* OSC: **O**pen**S**ound**C**ontrol

----

## UDP通信

* OSCは主にUDP上で（TCP上でも動くが実装が少ない）通信するプロトコル
  * 送信側は宛先のコンピュータのIPとポートを指定して送信する
  * 受信側はポートを指定して待ち受ける
  * ポートはユーザーが自由に決めて使う (16bit符号無し整数, 0-65535)
    * 0番ポートは特殊なポートとして使われる
    * 1-1023 はウェルノウンポートとして利用/予約されている
    * 1024-49151 もIANAという組織で管理されている
      * が, 実質的にこの範囲にも含まれる10000番以降あたりを使うことが多い
    * 厳密には 49152-65535 がユーザーが自由に使って良いとされる範囲
      * Linuxでは32768-61000, FreeBSDでは49152-65535 が指定されている
    * UDPでは受信側では送信側で送った順番と同じことは保証されない
    * UDPではデータ量やネットワーク状態によって受信できない (データが欠ける) 場合がある
  * 8000や8080等は他のネットワーク機器等から予想外のパケットが飛んでくることが多々あるので注意

----

## OSCアドレス

* OSCメッセージは `/` で始まる文字列のアドレスといくつかの値（とその型情報）のまとまりである
* 規格上はアドレスは `/` で始まる文字列であるが実装的には文字列であれば何でも良いことが多い

----

## OSCのフォーマット

* それぞれが4の倍数サイズにパディングされる

----

### OSC 1.0

* [OSC 1.0](http://opensoundcontrol.org/spec-1_0) では以下の4種類が必須とされている
  * int32: 32bit整数
  * float32: 32bit浮動小数点数
  * String: NULL終端文字列
  * Blob: バイナリデータ

----

### OSC 1.0 Additional Types

* また, 以下の型がAdditional Typesとして定義されている
  * Int64
  * OSC-Timetag
  * 64bit float (double)
  * Symbol (NULL終端文字列とほぼ同義)
  * char
  * RGBA color
  * MIDI Message (port, status, data1, data2)
  * True
  * False
  * Null
  * Infinitum
  * Beginning of an array
  * End of an array

----

### OSC 1.1

* [OSC 1.1](https://s3.amazonaws.com/academia.edu.documents/27533076/nime09oscfinal.pdf?response-content-disposition=inline%3B%20filename%3DNime09oscfinal.pdf&X-Amz-Algorithm=AWS4-HMAC-SHA256&X-Amz-Credential=AKIAIWOWYYGZ2Y53UL3A%2F20191225%2Fus-east-1%2Fs3%2Faws4_request&X-Amz-Date=20191225T154153Z&X-Amz-Expires=3600&X-Amz-SignedHeaders=host&X-Amz-Signature=dc44ed7946a6dc889bf273e34d68498ce41d47dfd4224e95579e5b7fcf38f184) では上記に加え以下の型が必須として追加されている
  * True
  * False
  * Null
  * Infinitum (Impulse)

----

### その他

* OSCバンドルという概念もあり, 複数のOSCメッセージをまとめて一つのパケットとして送ることが出来る
* 実装によって独自拡張で扱える型に違いがあり対応してない型を含んでいると通信出来ない場合がある
  * 例えばMaxの `udpreceive` では `long` (`int64`) や `double` が正しく受信されない

----

### ofxOSC

* ofxOsc のベースとなるoscpackでは以下の型が実装されている（ArrayはofxOscでは利用されない）

  * True Type / False Type (`bool`)
  * Null Type
  * Impulse
  * Int32 (`std::int32_t`)
  * Int64 (`std::int64_t`)
  * Float (`float`)
  * Double (`double`)
  * Char (`char`)
  * String / Symbol (`std::string`)
  * Blob (`ofBuffer`)
  * RGBA Color (`ofColor`)
  * MIDI Message (`std::uint32_t`)

