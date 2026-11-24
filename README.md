# BÀI TẬP LÝ THUYẾT MẬT MÃ – DES VÀ AES-128

**Học kỳ:** 2026.1
**Giảng viên:** Cô Tạ Thị Kim Huệ
**Ngôn ngữ lập trình:** C

## 1. Mục tiêu

Cài đặt hai thuật toán mã hóa DES và AES-128 bằng ngôn ngữ C.

Chương trình cần:
- Thực hiện đầy đủ các phép biến đổi của từng thuật toán.
- Sinh khóa con cho các vòng mã hóa.
- In trạng thái trung gian sau từng vòng.
- Kiểm thử bằng ví dụ trên lớp.
- Đối chiếu kết quả với các bộ kiểm thử chuẩn.

## 2. Thuật toán DES

- Kích thước khối: 64 bit.
- Kích thước khóa: 64 bit đầu vào, 56 bit hiệu dụng.
- Số vòng: 16.
- Cấu trúc: mạng Feistel.

Các phép biến đổi chính:
- Hoán vị khởi đầu IP.
- Sinh khóa con.
- Hàm vòng Feistel.
- Mở rộng E.
- Thay thế S-box.
- Hoán vị P.
- Hoán vị cuối FP.

Sử dụng các bảng hoán vị theo tài liệu giảng viên cung cấp.

## 3. Thuật toán AES-128

- Kích thước khối: 128 bit.
- Kích thước khóa: 128 bit.
- Số vòng: 10.

Các phép biến đổi:
- SubBytes.
- ShiftRows.
- MixColumns.
- AddRoundKey.
- Key Expansion.

## 4. Cấu trúc thư mục

- DES/: Mã nguồn thuật toán DES.
- AES/: Mã nguồn thuật toán AES-128.
- test_vectors/: Dữ liệu kiểm thử.
- results/: Kết quả thực thi và trạng thái trung gian.

## 5. Kiểm thử

Sử dụng ví dụ trên lớp và các bộ kiểm thử chuẩn để xác minh tính chính xác.
