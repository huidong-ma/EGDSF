# EGDSF
End-to-End Genomic Data Sharing with  Sensitive Masking and Learning-based
---

# Setup
```
cd <EGDSF_path>
```
## 1. Set Permissions of executable files
```
chmod 777 egdsf prsec skmer
```
For normal and convenient operation, please set `prsec` and `skmer` as the global environment variables:
```
export PATH=<prsec_path>:$PATH
export PATH=<skmer_path>:$PATH
```
You can also add these commands into `~/.bashrc`.

## 2. Download the Sensitive Sequence Dictionary
```
wget https://drive.google.com/file/d/1ER9jGTI2UmBj_coyZ2Xm9TUsv7gb8Rkk/view?usp=drive_link
tar -zxvf dataBaseSrf.tar.gz
```
---

# Running
**Compression**: 
```
./egdsf c <file>
```

**Decompression**: 
```
./egdsf d <file>
``` 

### Take the file `TEST.fasta` as an example:  
**Compression**: `./egdsf c TEST.fasta` to obtain a dir `TEST.fasta.out` including the compressed file `TEST.fasta.compressed`  
**Decompression**: `./egdsf d TEST.fasta.out/TEST.fasta.compressed` to obtain the decompressed file `TEST.fasta.decry` 

# Acknowledgment
The code is based on [PAC](https://github.com/mynotwo/Faster-and-Stronger-Lossless-Compression-with-Optimized-Autoregressive-Framework), [MSDZip](https://github.com/mhuidong/MSDZip), and [Reference-arithmetic-coding](https://github.com/nayuki/Reference-arithmetic-coding). Thanks for these great works.

# Contact
Email: mahd@nbjl.nankai.edu.cn  or sunh@nbjl.nankai.edu.cn  
Nankai-Baidu Joint Laboratory (NBJL)
