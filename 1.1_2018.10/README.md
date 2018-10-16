![YMF825 musicBOX Ver.1.1a 2018.10](jpg/implemented.jpg)

YMF825 musicBOX Ver.1.1 2018.10
===============================

## 回路図 / PCB

回路図 | Top面 | Bottom面
:------------------------------:|:---------------------------:|:---------------------------------:
![schematic](png/schematic.png) | ![pcb top](png/pcb_top.png) | ![pcb bottom](png/pcb_bottom.png)
[PDF](pdf/schematic.pdf)        | [PDF](pdf/pcb.pdf)          | [PDF](pdf/pcb.pdf)

## 部品表

ref      | 数量 | 名称 | 形式 | 入手先URL
---------|-----|-----|-----|-----
C1 - 4   | 4 | 積層セラミックコンデンサ | 10μF, 5.08mmピッチ | http://akizukidenshi.com/catalog/g/gP-03095/
C5, C6   | 2 | 積層セラミックコンデンサ | 1μF, 2.54mmピッチ | http://akizukidenshi.com/catalog/g/gP-04066/
D1       | 1 | ショットキーダイオード   | 30V 1A | http://akizukidenshi.com/catalog/g/gI-01707/
J1       | 1 | DCジャック      | 外径5.5mm, 内径2.1mm | http://akizukidenshi.com/catalog/g/gC-01604/
J2       | 1 | 小型ステレオミニジャック | 3.5mm | http://akizukidenshi.com/catalog/g/gC-02460/
JP1      | 1 | ピンヘッダ *1   | 1x3 | http://akizukidenshi.com/catalog/g/gC-00167/
JP1      | 1 | ジャンパピン *1 | 2.54mmピッチ | http://akizukidenshi.com/catalog/g/gP-03689/
LED1     | 1 | 青緑色LED *2   | 3mm弾丸型 | http://akizukidenshi.com/catalog/g/gI-12719/
LED2     | 1 | 黄色LED *2     | 3mm弾丸型 | http://akizukidenshi.com/catalog/g/gI-11639/
LED3     | 1 | 赤色LED *2     | 3mm弾丸型 | http://akizukidenshi.com/catalog/g/gI-11577/
R1       | 1 | 金属皮膜抵抗 *2 | 4.7kΩ |
R2       | 1 | 金属皮膜抵抗 *2 | 1kΩ   |
R3       | 1 | 金属皮膜抵抗 *2 | 1kΩ   |
R4, R5   | 2 | 金属皮膜抵抗    | 10kΩ |
SW1, SW2 | 2 | タクトスイッチ *3 | 2.54mmピッチ用 | http://akizukidenshi.com/catalog/g/gP-03650/
TR1      | 1 | ポリスイッチ | 0.3A　0.6A遮断 | http://akizukidenshi.com/catalog/g/gP-00775/
U1       | 1 | Arduino Nano | | http://akizukidenshi.com/catalog/g/gM-09059/
U1       | 1 | ICソケット *4 | 平ピン 32P 600mil |
U2       | 1 | MicroSD card breakout board+ | | https://www.sengoku.co.jp/mod/sgk_cart/detail.php?code=EEHD-4WXB
U2       | 1 | ピンヘッダ *5 | 1x8 | http://akizukidenshi.com/catalog/g/gC-00167/
U2       | 2 | なべ小ねじ+六角ナットセット | M2 | http://akizukidenshi.com/catalog/g/gP-13238/
U2       | 1 | MicroSD カード | |
U3, U4   | 2 | YMF825Board *6 | | http://akizukidenshi.com/catalog/g/gM-12414/
U3, U4   | 2 | ピンヘッダ *7 | オスL型 1x9P | http://akizukidenshi.com/catalog/g/gC-01627/
U3, U4   | 4 | ピンソケット *8 | メスL型 1x9P | http://akizukidenshi.com/catalog/g/gC-01627/
VR1      | 1 | 2連ボリューム | Aカーブ 10kΩ | http://akizukidenshi.com/catalog/g/gP-03604/

### 実装の注意点

- __\*1__ 外部7-12V電源を使用する際は必要です。USBからのみ給電する場合は不要です。
- __\*2__ LEDと電流制限抵抗は任意のものを使用可能です。ただし順方向電流 I<sub>F</sub> は 最大20mA です。
- __\*3__ タクトスイッチは任意の色を使用可能です。
- __\*4__ 丸ピンでは Arduino Nano のピンヘッダが入らないため、**必ず平ピンのICソケットを使用してください**。
- __\*5__ モジュールはんだ付け用のピンヘッダです。基板本体とモジュールを、ピンヘッダで挟み込むようにはんだ付けし、ピンヘッダのプラスチック部分を取り去って、もう片方をはんだ付けしてください。
- __\*6__ YMF825Board は必ず 3.3V 化を施してください。5V のままでは使用できません。
- __\*7__ はんだ付け面を裏面にした場合、U3, U4 のピンソケットは 4つ、表面にした場合は 4つ または 2つ 必要です。また、裏面にはんだ付けした場合は余ったピン部分が基板を傷つけるため、適切な長さに切り取ってください。
- __\*8__ 4つ 使用する場合は下記画像のようにピンソケットを上下に2つ重ね、はんだ付けしてください。

![schematic](jpg/pin_socket.jpg) |
:------------------------------:|
U3, U4 のピンソケットを4つ使った例。YMF825Board のピンヘッダは裏面にはんだ付け |

## 1.0a からの変更点

- 部品の再配置を行いました。
  - microSDカードモジュールを基板端まで移動したため、カードの抜き差しが容易になりました。
  - イヤホンジャック J2 を基板左側に移動したため、接続ケーブルはすべて基板の左側に集約されました。
- 一部の部品リファレンスの修正を行いました。
  - SW2 -> SW1
  - SW3 -> SW2
- 一部の実装部品のピッチ幅の変更を行いました。
  - D1のピッチ: 7.62mm -> 5.04mm
- Arduino Nano のICソケット U1 のピン数を30ピンから32ピンに変更し、32ピンICソケットを無加工で実装できるようになりました。
- 配線とベタの角処理を角から円弧に変更しました。
- イヤホンジャック J2 の基板固定用のドリル穴径を 1.7mm へ大きくしました。
- 基板左下（U1の直下）の基板角のみ、誤って半径 4mm の丸まりになっていたものを、3mm に修正しました。

## インタフェース
### ボタン

- __PLAY__: 押下すると、演奏の一時停止・再開をします。
- __NEXT__: 押下すると、次の曲が演奏されます。

### LED

- __Lch, Rch__: YMF825Board との通信時に点灯します。ほとんどの場合、どちらも同時に点灯します。
- __READ__: 背面の MicroSD カードモジュールとの通信時に点灯します。モジュール本体の赤色LEDと点灯タイミングは同じです。

### ボリューム

イヤホンジャックから出力される音量を調節します。このボリュームでは YMF825Board からの出力 CN2 の音量は変わりません。

### 電源ジャンパ

- __VIN__: DCジャックから電源を供給する場合に短絡させてください。__VIN を短絡させた状態で Arduino Nano のUSB端子を接続しないでください。__
- __USB__: DCジャックとの接続を切り、Arduino Nano のUSB端子から給電できるようにします。

### DCジャック

外径5.5mm、内径2.1mm、センタープラスのDCプラグが接続できます。定格電圧DC7V〜12V。
定格外の電圧を接続しないでください。

## MicroSDカードとデータについて

8GB の MicroSDHC カードで動作を確認しています。ただしファイルシステムは **FAT32** でフォーマットしてください。NTFS では使えません。

演奏データは YMF825 に書き込まれるデータのダンプファイルになっています。データフォーマットの詳細は [こちら](../sketch/format.md) を参照してください。

拡張子は `.m25` 固定です。それ以外の拡張子にはできません。これ以外の拡張子を持ったファイルは無視されます。

### ダンプファイルの配置について

MicroSDカードのルートディレクトリにダンプファイルを直接配置してください。

新たなディレクトリの直下に配置したものは無視されます。

## 応用
### アクリルパネルについて

秋月電子の [B基板用アクリルパネル](http://akizukidenshi.com/catalog/g/gP-10243/) が適合します。ネジ径は 3M に対応しています。
サンハヤト製 基板保護用アクリル板 ACB-93 とはサイズとネジ穴位置が異なるため **適合しません**。

## 動作上の注意点

1. DCジャックに 12V以上 または 7V以下の電圧を絶対に加えないでください。
2. 電源ジャンパが VIN 位置にあるとき、絶対に Arduino Nano のUSB端子を接続しないでください。
2. USB給電を使用する際は JP1 のジャンパを USB 側に必ず挿してください。

## 基板画像

![PCB Top side](jpg/pcb_top.jpg) |![PCB Bottom side](jpg/pcb_bottom.jpg) |
:------------------------------:|:------------------------------:|
基板 Top 面 |基板 Bottom 面 |
