# Phép nhân đa thức trên trường hữu hạn GF(2⁴) và GF(2⁸)

## 1. Giới thiệu

Dự án triển khai thuật toán nhân hai đa thức trên hai trường hữu hạn GF(2⁴) và GF(2⁸) bằng ngôn ngữ C.

Phép nhân được thực hiện theo phương pháp dịch bit (shift), XOR và rút gọn modulo đa thức bất khả quy. Chương trình hỗ trợ nhập các toán hạng ở dạng hexadecimal, kiểm tra dữ liệu đầu vào và hiển thị kết quả phép nhân.

## 2. Cơ sở lý thuyết

Trong trường hữu hạn GF(2ᵐ), mỗi phần tử được biểu diễn bằng một đa thức có hệ số thuộc GF(2), với bậc nhỏ hơn m.

Các phép toán cơ bản:

- Phép cộng đa thức được thực hiện bằng XOR.
- Phép nhân đa thức được thực hiện bằng dịch bit và XOR.
- Kết quả được rút gọn modulo một đa thức bất khả quy để bảo đảm thuộc trường tương ứng.

### Các đa thức bất khả quy được sử dụng

| Trường | Đa thức bất khả quy | Dạng hexadecimal |
|---|---|---|
| GF(2⁴) | x⁴ + x + 1 | `0x13` |
| GF(2⁸) | x⁸ + x⁴ + x³ + x + 1 | `0x11B` |

Trong đó, GF(2⁸) sử dụng đa thức bất khả quy được dùng trong thuật toán AES.

## 3. Thuật toán

Thuật toán nhân hai phần tử A và B được thực hiện theo các bước:

1. Khởi tạo biến kết quả bằng 0.
2. Kiểm tra từng bit của toán hạng B.
3. Nếu bit đang xét bằng 1, XOR giá trị hiện tại của A vào kết quả.
4. Dịch trái A một bit để thực hiện phép nhân với x.
5. Nếu xuất hiện bit vượt quá bậc của trường, XOR với đa thức bất khả quy tương ứng để rút gọn.
6. Dịch phải B một bit và tiếp tục cho đến khi tất cả các bit được xử lý.
7. Trả về kết quả phép nhân trong trường hữu hạn tương ứng.

## 4. Yêu cầu hệ thống

- GCC hỗ trợ chuẩn C11 trở lên.
- MSYS2 UCRT64, Linux hoặc môi trường terminal có GCC và Bash.
- Git (nếu muốn tải mã nguồn từ GitHub).

## 5. Cấu trúc dự án

```text
gf2-polynomial-multiplication/
├── src/
│   └── main.c
├── tests/
│   └── test_cli.sh
├── .gitignore
└── README.md
```

| Thành phần | Chức năng |
|---|---|
| `src/main.c` | Chứa chương trình chính, xử lý đầu vào và thuật toán nhân đa thức |
| `tests/test_cli.sh` | Script tự động biên dịch và kiểm thử chương trình |
| `.gitignore` | Loại trừ các file không cần thiết khỏi Git |
| `README.md` | Tài liệu hướng dẫn và mô tả dự án |

## 6. Biên dịch và chạy chương trình

### Biên dịch

Sử dụng lệnh:

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -Werror src/main.c -o gf2mul.exe
```

Các tùy chọn biên dịch:

- `-std=c11`: Sử dụng chuẩn ngôn ngữ C11.
- `-Wall -Wextra`: Bật các cảnh báo phổ biến.
- `-Wpedantic`: Kiểm tra tính tuân thủ chuẩn C.
- `-Werror`: Xem cảnh báo là lỗi biên dịch.

### Chạy chương trình

```bash
./gf2mul.exe
```

Chương trình yêu cầu lựa chọn trường hữu hạn và nhập hai toán hạng A, B dưới dạng hexadecimal.

Ví dụ các bộ dữ liệu kiểm thử:

**GF(2⁴)**

```text
Field: 1
A = 7
B = 8
Expected result: 0xD
```

**GF(2⁸)**

```text
Field: 2
A = 57
B = 83
Expected result: 0xC1
```

## 7. Kiểm tra và xử lý dữ liệu đầu vào

Chương trình có kiểm tra dữ liệu đầu vào nhằm hạn chế các trường hợp nhập không hợp lệ:

- Không chấp nhận lựa chọn trường hữu hạn không hợp lệ.
- Không chấp nhận toán hạng âm.
- Không chấp nhận ký tự không thuộc hệ hexadecimal.
- Không chấp nhận toán hạng vượt quá phạm vi của trường.
- Không chấp nhận dữ liệu hexadecimal quá lớn gây tràn số.
- Không chấp nhận trường hợp thiếu toán hạng.
- Không chấp nhận dữ liệu thừa sau khi nhập hai toán hạng.

Phạm vi giá trị hợp lệ:

| Trường | Giá trị nhỏ nhất | Giá trị lớn nhất |
|---|---|---|
| GF(2⁴) | `0x0` | `0xF` |
| GF(2⁸) | `0x00` | `0xFF` |

Chương trình hỗ trợ nhập chữ hexadecimal viết hoa và viết thường.

## 8. Kiểm thử tự động

Dự án sử dụng Bash script để tự động biên dịch và kiểm tra chương trình.

### Chạy kiểm thử

```bash
bash tests/test_cli.sh
```

Script sẽ tự động biên dịch chương trình với các tùy chọn cảnh báo nghiêm ngặt trước khi thực hiện các test case.

### Các nhóm kiểm thử

**1. Kiểm thử phép nhân cơ bản**

- Nhân với 0.
- Nhân với 1.
- Kiểm tra phép nhân đa thức.
- Kiểm tra rút gọn modulo đa thức bất khả quy.
- Kiểm tra toán hạng có giá trị lớn nhất.
- Kiểm tra đầu vào hexadecimal viết thường.

**2. Kiểm thử dữ liệu đầu vào không hợp lệ**

- Toán hạng vượt phạm vi.
- Lựa chọn trường không hợp lệ.
- Ký tự không phải hexadecimal.
- Thiếu toán hạng.
- Toán hạng âm.
- Dữ liệu hexadecimal quá lớn.
- Nhập thừa toán hạng.
- Ký tự hexadecimal không hợp lệ.

**3. Kiểm thử tính giao hoán**

Kiểm tra tính chất:

A * B = B * A

Thực hiện trên nhiều bộ dữ liệu thuộc cả GF(2⁴) và GF(2⁸).

**4. Kiểm thử tính phân phối**

Kiểm tra tính chất:

A * (B XOR C) = (A * B) XOR (A * C)

Trong đó XOR biểu diễn phép cộng trong trường hữu hạn GF(2ᵐ).

### Kết quả kiểm thử

Dự án hiện có tổng cộng 40 test case tự động, bao gồm kiểm thử phép nhân, kiểm tra dữ liệu đầu vào, tính giao hoán và tính phân phối.

Kết quả gần nhất:

```text
Passed: 40
Failed: 0
Total:  40
```

## 9. Công nghệ sử dụng

- **C (C11):** Ngôn ngữ lập trình chính.
- **GCC:** Trình biên dịch.
- **Bash:** Tự động hóa quá trình kiểm thử.
- **Git và GitHub:** Quản lý phiên bản và lưu trữ mã nguồn.
