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
