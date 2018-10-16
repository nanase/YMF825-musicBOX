YMF825 musicBOX Player スケッチ
==============================

YMF825 musicBOX上で m25 フォーマットを演奏させるためのArduino用スケッチです。

## 動作確認環境

- Arduino Nano
- Adafruit Pro Trinket 5V
  - YMF825 musicBOX 基板には適合しません。
  - INT0 が存在しないため、演奏の一時停止ができません。

## コンパイル方法

Arduino 公式のIDEから player.ino を読み込み、コンパイルを行ってください。
ただし以下のライブラリを使用しているため、事前にインクルードを行ってください。

### 使用ライブラリ

- **[PetitFS](https://github.com/greiman/PetitFS)**
  - 公式の SD ライブラリは動作が不安定のため非推奨です。

#### ヘッダファイルの書き換え

PetitFSの以下のヘッダファイルを書き換えてください。
この値は definitions.h の PIN_SD と同じ値にしてください。

```c
// PetitFS/src/pffArduino.h:7

// SD chip select pin
#define SD_CS_PIN 5
```

## m25 フォーマット

[こちら](format.md) を参照してください。

## definitions.h

定数値、マスク値などを記述しています。通常は変更する必要はありません。
Arduino Nano 以外のボードで使用する際は変更してください。

定数名              | 説明
--------------------|---------------------------------------------
PIN_SD              | microSD カードの CS が接続されているピンの番号
PORT_SS             | YMF825Board の SS が接続されているピンのポート名
PORT_SS_DDR_R       | YMF825Board の SS が接続されているピンの入出力方向レジスタ名
PORT_SS_DDR_V       | YMF825Board の SS が接続されているピンの入出力方向レジスタ値
BIT_SS_LCH          | Lch の YMF825Board の SS が有効になるときに PORT_SS とマスクされる値
BIT_SS_RCH          | Rch の YMF825Board の SS が有効になるときに PORT_SS とマスクされる値
BIT_SS_LR           | 両方の YMF825Board の SS が有効になるときに PORT_SS とマスクされる値
PORT_IC             | YMF825Board の RST_N が接続されているピンのポート名
PORT_IC_DDR_R       | YMF825Board の RST_N が接続されているピンの入出力方向レジスタ名
PORT_IC_DDR_V       | YMF825Board の RST_N が接続されているピンの入出力方向レジスタ値
BIT_IC              | YMF825Board の RST_N が有効になるときに PORT_IC とマスクされる値
PIN_BT_PLAY         | PLAY ボタンが接続されているピンの番号
PIN_BT_NEXT         | NEXT ボタンが接続されているピンの番号
MODE_BT_PLAY        | PIN_BT_PLAY で指定したピンのモード
MODE_BT_NEXT        | PIN_BT_NEXT で指定したピンのモード

以下は m25 ファイルのデコードに使用します。変更する必要はありません。

定数名              | 説明
--------------------|---------------------------------------------
WAIT_RESOLUTION     | ウェイトの単位時間（時間分解能）。単位は ミリ秒
SELX_MASK           | チャンクの SELx パートのマスク値
SELX_WAIT           | チャンクの SELx が WAIT であることを表すマスク値
SELX_LCH_ENABLE     | チャンクの SELx が Lch のみ有効であることを表すマスク値
SELX_RCH_ENABLE     | チャンクの SELx が Rch のみ有効であることを表すマスク値
SELX_LR_ENABLE      | チャンクの SELx が Lch および Rch が有効であることを表すマスク値
WOPX_MASK           | チャンクの WOPx パートのマスク値
WOPX_WRITE_DA_DD    | チャンクの WOPx が Write 命令であることを表すマスク値
WOPX_RESERVED       | 未使用（予約済みのため使用禁止）
WOPX_BURSTWRITE_TONE| チャンクの WOPx がトーンパラメータに対する BurstWrite 命令であることを表すマスク値
WOPX_BURSTWRITE_EQ  | チャンクの WOPx がイコライザに対する BurstWrite 命令であることを表すマスク値
LENX_MASK           | チャンクの LENx パートのマスク値

定数名              | 説明
--------------------|---------------------------------------------
SD_BUFFER_SIZE      | microSDカードから m25 ファイルを読み込む際のバッファサイズ。値を大きくすることで、一度に読み込むデータサイズを大きくできます。最低でも 486 バイト必要です。Arduino の SRAM 領域を確保するため、多すぎると動作が不安定になります。
