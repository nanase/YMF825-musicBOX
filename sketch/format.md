## 拡張子

`.m25` で固定。

## ヘッダ

なし。

## チャンク

チャンクヘッダは 1 バイト。

 +0 | +1 | +2 | +3 | +4 | +5 | +6 | +7
----|----|----|----|----|----|----|----
SEL0|SEL1|WOP0|WOP1|LEN0|LEN1|LEN2|LEN3

### SELx

SEL0 | SEL1 | Operation
-----|------|-----------
`0`  | `0`  | Wait
`0`  | `1`  | Only Lch enable
`1`  | `0`  | Only Rch enable
`1`  | `1`  | Lch and Rch enable 

Wait の分解能は 10ms。
デフォルト値は `1` `1` (Lch and Rch enable)。

### WOPx

WOP0 | WOP1 | Operation
-----|------|-----------
`0`  | `0`  | Write
`0`  | `1`  | (Reserved)
`1`  | `0`  | BurstWrite to tone parameters
`1`  | `1`  | BurstWrite to equalizer

SELx が Wait のときは WOPx は無視される。

### LENx

SEL0 | SEL1 | WOP0 | WOP1 | Operation
-----|------|------|------|----------
`0`  | `0`  | n/a  | n/a  | Wait数 (1 〜 16)
n/a  | n/a  | `0`  | `0`  | Address/Data 数。2 〜 32 バイト。
n/a  | n/a  | `0`  | `1`  | (Reserved)
n/a  | n/a  | `1`  | `0`  | 書き込みパート数。1 〜 16 パート、1 + 30 〜 480 + 5 バイト。
n/a  | n/a  | `1`  | `1`  | LENx の値は無視。1 + 15バイト固定。
