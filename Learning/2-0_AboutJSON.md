# JSON について

## JSONフォーマット

* **J**ava**S**cript **O**bject **N**otation の略称
* JSONの説明は詳しくやると終わらないので [JSONの紹介](https://www.json.org/json-ja.html) 等を参照のこと

----

## oFでのJSONの扱い

* oF 0.9.8までは addon の形で様々なJSONの読み込み/書き出しが出来る C++ライブラリがラップされていた
  * [ofxJansson](https://github.com/roxlu/ofxJansson)
  * [ofxJSON](http://github.com/jeffcrouse/ofxJSON)
    * [ofxJsonSettings](http://github.com/mattfelsen/ofxJsonSettings)
    * [ofxConfigJSON](https://github.com/a2mSystemes/ofxConfigJSON)
  * [ofxJsonxx](http://github.com/satoruhiga/ofxJsonxx)
* oF 0.10.0からコアライブラリに `ofJson` という名前で [nlohmann json](https://www.json.org/json-ja.html) というシングルヘッダのライブラリが導入された
* 今後は基本的（依存関係等が無い限り）には `ofJson` を使うのが良い
  * 比較的素直に配列や `map`風のアクセスが出来る
  * 但し, 例外処理がかなり頻繁に発生するため一部のoFコアメンバーの中で問題として上がっている

----

## JSON で扱える型

* number型 (oF: `double`, 数値型は総てこの型になる)
* string型 (oF: `string`)
* bool型 (oF: `bool`)
* null (oF: 型無し)
* Array型 (oF: `std::vector<ofJson>`)
  * `[v1, v2, v3, ..., vN]` の様に書く
    * `vn` は JSON で扱える任意の型
* Object型 (oF: `std::map<std::string, ofJson>`)
  * `{key1: v1, key2: v2, key3: v3, ..., keyN: vN}` の様に書く
    *  `keyN` はstring型
    * `vN` は JSON で扱える任意の型

----

## 基本的な JSON の記述方法

```json
{
  "DOUBLE_NAME": 0.0,
  "STRING_NAME": "string",
  "BOOL_NAME1": true,
  "BOOL_NAME2": false
  "ARRAY_NAME": [
    0.0, 1.0, 2.0
  ],
  "OBJECT_NAME": {
    "NESTED_NAME1": 1.0,
    "NESTED_NAME2": "string",
    "NESTED_NAME3": [0.0, 1.0, "2.0"]
  }
}
```

* JavaScriptのObjectより制約が多い
  * Objectのキー (`:` の左側) は `"` で囲まれてないといけない
  * Objectの同じ階層に同じキーが存在してはいけない
  * 0xFF や 077 のような16進数や8進数は使えない
* 配列の要素の型が違っても良い
  * が, C++では扱い辛いのでオススメしない

----

## JSON を使うメリット

* 汎用的なシリアライズフォーマットで様々な言語で読み書き出来るライブラリが存在する
  * 例えばJavaScriptやPythonでは言語機能やコアモジュールとして存在する
* 同様のフォーマットで XML も存在する
  * 0.8.4 までは addon として `ofxXMLSettings`, 0.9.0 から基本機能として `ofXML` が存在する
  * しかし
    * XML では型情報が失われること
    * `ofXML` は Poco（0.9.8 までのoFが依存する巨大なフレームワーク）に依存しており, 0.10.0 になるのを機に Poco への依存を辞め,  `stl` と小さなライブラリへの依存へ舵を切った為, `ofXML` は全く別のAPIに変更された
    * XML では閉じタグが必要となり, 手で変更をした場合ミスが起きやすい
  * 等の理由から徐々に JSON が主流になりつつある

----

## ofJson の使い方

* 2-1_JSONを参照のこと