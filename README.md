# WioTerminal
WioTerminal用のいろいろ。

Wio Terminal とは、Seeed社のIoTコア（と言っていいのかな？）小さいコンピュータ。

https://wiki.seeedstudio.com/Wio-Terminal-Getting-Started/

ここら辺参照。

初期設定等は

https://webshop.pupu.jp/grape/WioTerminal/FirstStep.html

こちらが詳しいです。

Arduino IDE に環境をインストール出来たら、下記のプログラムをダウンロードしてWio Terminalに転送してください。



# NumberTron

数字が敷き詰められたフィールドを８方向へ移動可能。一歩目の数字の分だけ直線的に移動。移動中に空白や壁があったらゲームオーバー。というTRONやSNAKE GAME的なゲーム。

オリジナルは、JoachimさんによるMZ-80/700用のプログラム。

https://twitter.com/MickyAlbert/status/1324864888808775682

のツイートを参考に、（ホンモノはやったことないのに）見よう見まねで作成。

詳しい説明は
https://note.com/rasen/n/n357c972b4a52
を参照。

## 製作記

1. https://note.com/rasen/n/n78dead51ba93
2. https://note.com/rasen/n/nb3cd4285822d
3. https://note.com/rasen/n/n8f4cbb0ac2a6
4. https://note.com/rasen/n/nd828bad36f88
5. https://note.com/rasen/n/n4da87cee2bc5
6. https://note.com/rasen/n/n357c972b4a52


# CharTest

NumberTron用に特殊記号を調べるために作成。ASCIIコード順に画面に表示。上下でASCIIコード（十進）を表示しつつキャラを表示します。

