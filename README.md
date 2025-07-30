<div align="center">
<h1>Lossless Genomic Data Storage and Sharing with Sensitive Masking and Learning-based Compression</h1>
</div>

# Setup
```
cd <EGDSF_PATH>
```
## 1. Set Permissions of executable files
```
chmod 777 ./egdsf
```
For convenient operation, you can set `egdsf` as the global environment variables:
```
export PATH=<EGDSF_PATH>:$PATH
```

## 2. Download the Sensitive Sequence Dictionary
```
wget https://drive.google.com/file/d/1ER9jGTI2UmBj_coyZ2Xm9TUsv7gb8Rkk/view?usp=drive_link
tar -zxvf dataBaseSrf.tar.gz
```
---

# Usage
**Compression**: 
```
egdsf c <input> <output>
```

**Decompression**: 
```
egdsf d <input> <output>
``` 

### Take the file `test` as an example:  
**Compression**: `egdsf c test test.encry` <br>
**Decompression**: `egdsf d test.encry test.decry` 

# Acknowledgment
The code is based on [PAC](https://github.com/mynotwo/Faster-and-Stronger-Lossless-Compression-with-Optimized-Autoregressive-Framework), [MSDZip](https://github.com/mhuidong/MSDZip), and [Reference-arithmetic-coding](https://github.com/nayuki/Reference-arithmetic-coding). Thanks for these great works.

# Change Logs
### 30/7/2025
Fixed some bugs, and now `EGDSF` is more user-friendly.

### 20/05/2025
Fixed some bugs.