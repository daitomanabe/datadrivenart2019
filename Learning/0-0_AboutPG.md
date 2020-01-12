# ProjectGenerator について

## ProjectGenerator

* openFrameworksに付属する様々な環境のプロジェクトファイルを自動で生成するアプリケーション

* Electron製のGUIアプリで様々な設定が出来る
  * CUIでも利用できるがパス等のオプションがかなり煩雑になりがち

## addonの管理 (addons.make)

* PGを使うとaddonを追加する際にインクルードパス等の設定を行ってくれる
  * 但し, addonの構成によっては一筋縄で行かないこともある

* プロジェクトディレクトリ直下の `addons.make` にaddonの設定が保存される

  * oF直下の `addons` ディレクトリのものを使う場合はGUIから選択するか `addons.make` に addon 名を1行に1個ずつ書き加える
    * ex. `ofxOsc`
  * 基本的にaddonはoF直下の `addons` に入れる

* グローバルの `addons` ディレクトリ下にあるバージョンでは無く特定バージョンの addon 等を使いたい場合

  * プロジェクト直下等に `local_addons` や `addons` という名前のディレクトリを作成
  * 目的のaddonを追加
  * GUIアプリからは選択出来ないので, 直接 `addons.make` を編集する
  * `addons.make` 内では `addons.make` からの相対パスで指定する
    * ex. `local_addons/ofxPubSubOsc`

  * 複数人で共同開発する場合等はプロジェクト直下の適当なディレクトリに addon を追加しバージョン等を共有するのが良い

## config.make

* プロジェクト自体の設定を記述する
* `oF_dir/apps/project/AwesomeApp` という構造に従うのがシンプルで推奨されるが, これ以外の場所にプロジェクトを置きたい場合は `OF_ROOT` で `oF_dir` までの相対パスを指定する
* その他はC++に関する設定やプロジェクトに追加するファイル等の設定になる
  * 気になる人は config.make を読むとコメントも付いているので大体分かるはず

