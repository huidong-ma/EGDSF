import numpy as np
import struct
import torch.nn.functional as F

def loss_function(pred, target):
    loss = 1/np.log(2) * F.nll_loss(pred, target)
    return loss

def strided_app(a, L, S):  # Window len = L, Stride len/stepsize = S        # 也是生成X和Y
    nrows = ((a.size - L) // S) + 1
    n = a.strides[0]
    return np.lib.stride_tricks.as_strided(a, shape=(nrows, L), strides=(S * n, n))

def reorder_data(data, batchsize, iter_num):
    arr = list()
    for i in range(batchsize):
        for j in range(iter_num):
            arr.append(batchsize * j + i)
    return np.array(data[arr])

def var_int_encode(byte_str_len, f):  # 这段代码是用于对整数进行变长编码的函数。它的目的是将一个整数按照一定规则编码成一个字节序列，并将编码后的字节写入文件对象 f 中。
    while True:
        this_byte = byte_str_len & 127
        byte_str_len >>= 7
        if byte_str_len == 0:
            f.write(struct.pack('B', this_byte))
            break
        f.write(struct.pack('B', this_byte | 128))
        byte_str_len -= 1

def split_data(file, prefix, n, tempfile):
    with open(file, 'rb') as f:  # 一次一个byte = 8bit
        series = np.frombuffer(f.read(), dtype=np.uint8)
    f.close()
    vals = list(set(series))
    vals.sort()
    char2id_dict = {str(c): i for (i, c) in enumerate(vals)}
    id2char_dict = {str(i): c for (i, c) in enumerate(vals)}
    params = dict()
    segment_length = len(series) // n
    for i in range(n):
        start_index = i * segment_length
        # 最后一段可能包括剩余的部分
        end_index = start_index + segment_length if i < n - 1 else len(series)
        segment = series[start_index:end_index]
        fout = open(tempfile + '/' + prefix + '.' + str(i), 'wb')
        fout.write(bytearray(segment))
        fout.close()
        params[prefix + '.' + str(i)] = len(segment)
    params['char2id_dict'] = char2id_dict
    params['id2char_dict'] = id2char_dict
    with open(prefix + '.params', 'w') as f:
        f.write(str(params))
    f.close()

def var_int_decode(f):
    byte_str_len = 0
    shift = 1
    while True:
        this_byte = struct.unpack('B', f.read(1))[0]
        byte_str_len += (this_byte & 127) * shift
        if this_byte & 128 == 0:
            break
        shift <<= 7
        byte_str_len += shift
    return byte_str_len

def encode_array(arr):
    """
    将包含0/1/2/3的NumPy数组编码为0-255的数组。

    参数:
    arr (np.ndarray): 输入数组，元素只能是0/1/2/3

    返回:
    np.ndarray: 编码后的数组，元素为0-255的整数
    """
    # 验证输入
    if not isinstance(arr, np.ndarray):
        raise TypeError("输入必须是NumPy数组")
    if not np.all(np.isin(arr, [0, 1, 2, 3])):
        raise ValueError("数组元素只能是0/1/2/3")

    # 补充0使长度变为4的倍数
    pad_len = (4 - len(arr) % 4) % 4
    padded_arr = np.pad(arr, (0, pad_len), mode='constant', constant_values=0)

    # 将数组重塑为4列（自动计算行数）
    reshaped = padded_arr.reshape(-1, 4)

    # 计算每组的值：相当于4位基数为4的数转十进制
    # 公式：val = a*4^3 + b*4^2 + c*4^1 + d*4^0
    powers = np.array([64, 16, 4, 1])  # 4^3, 4^2, 4^1, 4^0
    encoded = np.sum(reshaped * powers, axis=1)

    return encoded

def recover_data(k, w, id2char_dict, series, additional_str):
    # 用于解压缩恢复K-Mer编码
    if (k == w):
        res = [id2char_dict[str(s)] for s in series]
        merged_string = ''.join(res)
        if len(additional_str) > 0:  # k - w 是交叉重叠的部分
            merged_string = merged_string + additional_str
        return merged_string
    else:

        res = [0] * len(series)
        #res = [id2char_dict[str(s)] for s in series]
        i = 0
        while (i != len(res)):
            if (i == 0):
                res[i] = id2char_dict[str(series[i])]
            else:
                res[i] = id2char_dict[str(series[i])][k-w:]
            i = i + 1
        merged_string = ''.join(res)
        if len(additional_str) > 0:  # k - w 是交叉重叠的部分
            merged_string = merged_string + additional_str
        return merged_string
