# Phép nhân đa thức trên trường hữu hạn GF(2^4) và GF(2^8)

Dự án này cài đặt thuật toán nhân đa thức trên hai trường hữu hạn (finite fields) thường gặp trong mã hóa thông tin:

- GF(2^4): sử dụng đa thức bất khả quy x^4 + x + 1
- GF(2^8): sử dụng đa thức bất khả quy x^8 + x^4 + x^3 + x + 1 (chuẩn AES)

## Thuật toán hoạt động

Các toán hạng được biểu diễn dưới dạng đa thức nhị phân. Quá trình nhân thực hiện qua các bước:

1. Dùng phép XOR để cộng các đa thức.
2. Dùng phép dịch trái (left shift) để nhân với x.
3. Rút gọn modulo đa thức bất khả quy tương ứng.
4. Lặp lại quá trình trên theo từng bit của toán hạng thứ hai.

## Bảng đa thức bất khả quy

| Trường | Đa thức bất khả quy | Dạng Hex |
| --- | --- | --- |
| GF(2^4) | x^4 + x + 1 | 0x13 |
| GF(2^8) | x^8 + x^4 + x^3 + x + 1 | 0x11B |

## Yêu cầu hệ thống

- Trình biên dịch GCC (hỗ trợ C11 trở lên)
- Terminal / Command Line

## Biên dịch (Build)

```bash
gcc -std=c11 -Wall -Wextra -Wpedantic -Werror src/main.c -o gf2mul.exe
Chạy chương trình dùng lệnh ./gf2mul.exe
