# BIM test code of autoBIM

`baseline` is the official code. `evolved-slower` and `evolved-faster` are our codes.

## Run

Simple test runs are listed below.

```bash
git clone https://github.com/smileyzyw/autoBIM-dev.git
cd autoBIM-dev/vvenc
```

Baseline:

```bash
python ../switch_commonlib_variant.py baseline
cmake -DCMAKE_BUILD_TYPE=Release && make clean && make -j8
./bin/release-static/vvencFFapp -c ./cfg/randomaccess_faster.cfg --Level 6.1 -i /home/Seq/BQSquare_416x240_60.yuv -s 416x240 --InputBitDepth 8 -r 60 -f 600 -b ./BQSquare_qp37_baseline_faster.bin -q 37 -t 8 -ip 64 --POC0IDR 1 -dph 1 -v 6 --MCTF 1 --BIM 1 > ./enc_log_BQSquare_qp37_baseline_faster.log
```

Evolved-slower:

```bash
python ../switch_commonlib_variant.py evolved-slower
cmake -DCMAKE_BUILD_TYPE=Release && make clean && make -j8
./bin/release-static/vvencFFapp -c ./cfg/randomaccess_slower.cfg --Level 6.1 -i /home/Seq/BQSquare_416x240_60.yuv -s 416x240 --InputBitDepth 8 -r 60 -f 600 -b ./BQSquare_qp37_evolved-slower.bin -q 37 -t 8 -ip 64 --POC0IDR 1 -dph 1 -v 6 --MCTF 1 --BIM 1 > ./enc_log_BQSquare_qp37_evolved-slower.log
```

Evolved-faster:

```bash
python ../switch_commonlib_variant.py evolved-faster
cmake -DCMAKE_BUILD_TYPE=Release && make clean && make -j8
./bin/release-static/vvencFFapp -c ./cfg/randomaccess_faster.cfg --Level 6.1 -i /home/Seq/BQSquare_416x240_60.yuv -s 416x240 --InputBitDepth 8 -r 60 -f 600 -b ./BQSquare_qp37_evolved-faster.bin -q 37 -t 8 -ip 64 --POC0IDR 1 -dph 1 -v 6 --MCTF 1 --BIM 1 > ./enc_log_BQSquare_qp37_evolved-faster.log
```

## Thanks

Thanks to the [vvenc project](https://github.com/fraunhoferhhi/vvenc).
Our corresponding `vvenc` commit is `7482e142fd61dca1ed48ffdfa8740ac072734934`.
