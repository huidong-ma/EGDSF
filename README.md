<div align="center">
<h1>Secure Genome Sharing via Masking-Enhanced Neural Lossless Compression</h1>
</div>

## 📄 Introduction
SeGen is a state-of-the-art framework designed for secure genomic data storage and sharing. It addresses the critical dilemma between strict privacy protection and high compression efficiency. Traditional "Encryption before Compression" pipelines often destroy the intrinsic redundancy of GD and incur prohibitive computational costs. 

To overcome this, SeGen introduces a novel **"Filtering Masking + Compression"** paradigm. By isolating the sensitive sequences (e.g., STRs, DRSs) and applying lightweight masking that maps back to the `{A, C, G, T}` alphabet, SeGen preserves the macro-redundancy of the genome. This allows the built-in Genomic Compression Neural Network (GCNN) to fully capture data dependencies, achieving extreme compression without sacrificing privacy.

<div align="center">
  <picture>
    <img src="https://github.com/huidong-ma/SeGen/blob/main/assets/framework.png" width="800" alt="Framework of proposed method.">
  </picture>
  <br>
  <b>Framework of proposed method.</b>
</div>

SeGen provides the following key features:
* 🔒 **Robust Data Privacy:** Accurately filters and masks only the highly sensitive strings (approx. 11.3% of the human genome) via Parallel Sensitive Sequence Masking (PSSM), ensuring computational infeasibility for unauthorized decryption.
* 📉 **Superior Compression Ratio:** Outperforms 19 state-of-the-art conventional and learning-based compressors by effectively exploiting local and global redundancies via custom Deep Mixers.
* 🚀 **Unmatched Efficiency:** Powered by the DNA Grouper and fully parallel filtering, SeGen achieves the fastest compression/decompression speeds among existing neural compressors while significantly minimizing peak memory usage.


<div align="center">
  <picture>
    <img src="https://github.com/huidong-ma/SeGen/blob/main/assets/results.png" width="800" alt="Performance of all methods.">
  </picture>
  <br>
  <b>Overall performance of all method on real-world datasets.</b>
</div>

---

## 💡 Usage

### Setup
Before running, navigate to the project directory and set the appropriate permissions for the executable:
```bash
cd <SeGen_PATH>
chmod 777 ./SeGen
```
*Optional but recommended:* Add `SeGen` to your global environment variables for convenient access from anywhere:
```bash
export PATH=<SeGen_PATH>:$PATH
```

### Running
You can easily compress and decompress files using the following commands:
```bash
# Compression
segen c <input_file> <output_file>

# Decompression
segen d <compressed_file> <decompressed_file>
```

**Example:**
Using the file `test` as an example:
```bash
segen c test test.encry
segen d test.encry test.decry
```

---

## 📦 Dataset
The datasets used in the paper can be directly downloaded from [datasets.tar.gz](https://drive.google.com/file/d/1LShFvdYzGvXiFhzEU7dZUfEx-jPFSlDO/view?usp=drive_link) and extracted by executing `tar -xzf datasets.tar.gz`. The detailed information is as follows.
| **Name** | **Size (B)** | **Entropy** | **Description** |
| :---: | :---: | :---: | :---: |
| **AeCa** | 1,591,049 | 1.987 | A medium-sized aeropyrum camini dataset |
| **HePy** | 1,667,825 | 1.964 | A medium-sized helicobacter pylori bacterial dataset |
| **HaHi** | 3,890,005 | 1.955 | A large-scale haloarcula hispanica archaea dataset |
| **EsCo** | 4,641,652 | 2.000 | A medium-sized escherichia coli bacteria dataset |
| **SnSt** | 6,254,100 | 1.983 | A small-sized Human high-level privacy-sensitive dataset with STRs and SNPs |
| **PlFa** | 8,986,712 | 1.974 | A small-scale plasmodium falciparum dataset |
| **WaMe** | 9,144,432 | 1.976 | A GD dataset of unknown species |
| **ScPo** | 10,652,155 | 1.964 | A medium-sized schizosaccharomyces pombe dataset |
| **HuMa** | 16,577,023 | 1.931 | A dataset of human genome data |
| **EnIn** | 26,403,087 | 1.951 | A medium-sized entamoeba invadens dataset |
| **DrMe** | 32,181,429 | 1.991 | A medium-sized chromosome-2 of the drosophila miranda |
| **OrSa** | 43,262,523 | 1.989 | A large chromosome-1 dataset of oryza sativa japonica |
| **DaRe** | 62,565,020 | 1.950 | The chromosome-3 of the danio rerio |
| **AnCa** | 142,189,675 | 1.968 | One genome dataset of unknown species |
| **GaGa** | 148,532,294 | 1.970 | A large-scale chromosome-2 of the gallus gallus |
| **HoSa** | 189,752,667 | 1.960 | A large-scale human genome dataset |


---

## 🔥 Change Logs
- *2025.07.30*: Fixed several bugs. SeGen is now more user-friendly. 
- *2025.05.20*: Initial bug fixes and improvements.

---

## 📖 Acknowledgment
The code is based on [PAC](https://github.com/mynotwo/Faster-and-Stronger-Lossless-Compression-with-Optimized-Autoregressive-Framework), [MSDZip](https://github.com/mhuidong/MSDZip), and [Reference-arithmetic-coding](https://github.com/nayuki/Reference-arithmetic-coding). Thanks for these great works!

## ✉️ Contact
Email: mahd@nbjl.nankai.edu.cn  
Nankai-Baidu Joint Laboratory (NBJL)
