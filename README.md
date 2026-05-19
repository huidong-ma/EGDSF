<div align="center">
<h1>EGDSF: Lossless Genomic Data Storage and Sharing with Sensitive Masking and Learning-based Compression</h1>
</div>

## 📄 Introduction
EGDSF is a state-of-the-art framework designed for secure genomic data (GD) storage and sharing. It addresses the critical dilemma between strict privacy protection and high compression efficiency. Traditional "Encryption before Compression" pipelines often destroy the intrinsic redundancy of GD and incur prohibitive computational costs. 

To overcome this, EGDSF introduces a novel **"Filtering Masking + Compression"** paradigm. By isolating the sensitive sequences (e.g., STRs, DRSs) and applying lightweight masking that maps back to the `{A, C, G, T}` alphabet, EGDSF preserves the macro-redundancy of the genome. This allows the built-in Genomic Compression Neural Network (GCNN) to fully capture data dependencies, achieving extreme compression without sacrificing privacy.

<div align="center">
  <picture>
    <img src="https://github.com/huidong-ma/EGDSF/tree/main/assets/framework.png" width="600" alt="Framework of proposed method.">
  </picture>
  <br>
  <b>Framework of proposed method.</b>
</div>

EGDSF provides the following key features:
* 🔒 **Robust Data Privacy:** Accurately filters and masks only the highly sensitive strings (approx. 11.3% of the human genome) via Parallel Sensitive Sequence Masking (PSSM), ensuring computational infeasibility for unauthorized decryption.
* 📉 **Superior Compression Ratio:** Outperforms 19 state-of-the-art conventional and learning-based compressors by effectively exploiting local and global redundancies via custom Deep Mixers.
* 🚀 **Unmatched Efficiency:** Powered by the DNA Grouper and fully parallel filtering, EGDSF achieves the fastest compression/decompression speeds among existing neural compressors while significantly minimizing peak memory usage.


<div align="center">
  <picture>
    <img src="https://github.com/huidong-ma/EGDSF/tree/main/assets/results.png" width="600" alt="Performance of all methods.">
  </picture>
  <br>
  <b>Overall performance of all method on real-world datasets.</b>
</div>

---

## 🔥 Change Logs
- *2025.07.30*: Fixed several bugs. EGDSF is now more user-friendly. 
- *2025.05.20*: Initial bug fixes and improvements.

---

## 💡 Usage

### Setup
Before running, navigate to the project directory and set the appropriate permissions for the executable:
```bash
cd <EGDSF_PATH>
chmod 777 ./egdsf
```
*Optional but recommended:* Add `egdsf` to your global environment variables for convenient access from anywhere:
```bash
export PATH=<EGDSF_PATH>:$PATH
```

### Running
You can easily compress and decompress files using the following commands:
```bash
# Compression
egdsf c <input_file> <output_file>

# Decompression
egdsf d <compressed_file> <decompressed_file>
```

**Example:**
Using the file `test` as an example:
```bash
egdsf c test test.encry
egdsf d test.encry test.decry
```

---

## 📦 Dataset
To reproduce the results or evaluate the method, please prepare the datasets as follows:
1. Download the Sensitive Sequence Dictionary and DNA Datasets used in the paper from [link]().
2. Unzip the downloaded archives:
```bash
tar -zxvf dataBaseSrf.tar.gz 
tar -zxvf datasets.tar.gz
```

---

## 📖 Acknowledgment
The code is based on [PAC](https://github.com/mynotwo/Faster-and-Stronger-Lossless-Compression-with-Optimized-Autoregressive-Framework), [MSDZip](https://github.com/mhuidong/MSDZip), and [Reference-arithmetic-coding](https://github.com/nayuki/Reference-arithmetic-coding). Thanks for these great works!

## ✉️ Contact
Email: mahd@nbjl.nankai.edu.cn  
Nankai-Baidu Joint Laboratory (NBJL)
