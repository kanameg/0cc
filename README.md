# 0cc 0 C compiler

0cc is original compiler for C language.

## Build

Run make to build.
```
$ make
```

Run unit test. You run make command with "test" as an argument.
```
$ make test
```


## 開発
### 数字を返すプログラム

### 四則演算

### スタックマシン

#### 数値
ノードが数値の場合registerに取っておいても良いが、レジスタに余りがあるか不明のため次の演算子のために一度スタックにpushする。このコードは非常に無駄に見えるが、無駄なコードは最適化フェーズで削除する。

#### 変数
ノードが変数の場合も必ず値を一度スタックにpushを行い、次の演算子のノードのときにスタックからpopを行い演算を行う。このコードも非常に無駄だが最適化のフェーズでコードの消去を行う。

#### 二項演算子
二項演算子の場合、演算すべき値は事前にスタックに積まれているので、第一演算子をaxにpop、第二演算子をrdiにpopし演算を行う。

#### 単項演算子



### 変数の追加

関数呼び出し時（プロローグ）の動作
```
push rbp       ; 呼び出し元のrbp(ベースポインタ)を保存
mov rbp, rsp   ; 現在のrsp(スタックポインタ)をrbpに代入
sub rsp, 16    ; 関数フレームで確保する変数分rspを移動
```

関数からの戻り方（エピローグ）の動作
```
mov rsp, rbp   ; 関数フレームを破棄して現在のrbpをrspに代入
pop rbp        ; スタックに保存された呼び出し元のrbpをrbpに戻す
ret            ; 呼び出し元に戻る
```

一文字変数の追加


## Author
Kaname (kaname.g@gmail.com)
