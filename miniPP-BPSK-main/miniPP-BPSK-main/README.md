# miniPP-BPSK
## Hướng dẫn build code trên máy tính

* Cài trình biên dịch. Nếu chưa có thì download source trình biên dịch về: [Link](https://drive.google.com/file/d/1j4FnG02kGxLTWbm-wnNh0U_J4BB1c5cz/view?usp=sharing)
* Giải nén và add đường dẫn thư mục *...\MinGW\bin* vào viến môi trường *PATH*. [Link hướng dẫn](https://quantrimang.com/them-bien-moi-truong-trong-windows-174960)
* Đọc hiểu File Makefile.
* Build bằng chương trình thực thi autoMake.bat hoặc vào Command Prompt chạy make (make trong các thư mục melpe/vad/crypto/fec/audio nữa (ko make được thì báo lại lỗi)).

Sơ đồ hệ thống ![](https://github.com/hsdfat8/miniPP-BPSK/blob/main/image/Capture.PNG)

Mục tiêu: đọc hiểu hàm Modulate() và Demodulate_8k() trong file modem/modem.c

## Hướng dẫn chạy

* File data là dữ liệu các mẫu giọng nói với tần số lấy mẫu là 8kHz. 
* Dùng hàm readBin('filename','short') trong thư mục Matlab để đọc dữ liệu. Dữ liệu mẫu: [Link](https://drive.google.com/drive/folders/1bHx8Py1nplFig0ZDf40omNmM-pI9yEFE?usp=sharing)
* Dùng lệnh `tx fileIn fileBPSK fileBufftx` để chạy code bên transmitter.
* Dùng lệnh `rx fileBPSK fileOut fileBuffrx` để chạy code bên receiver.
* Dùng matlab convert file bin về file âm thanh đuôi '.wav' để chấm PESQ để đo chất lượng. Dùng lệnh `PESQ +8000 fileIn.wav fileOut.wav`.

Các file dữ liệu được lưu dưới dạng binary được lưu ra file dưới dạng short.
| File      | Chức năng |
| ----------| ----------- |
| fileIn    |Data giọng nói đầu vào       |
| fileBPSK  |Dữ liệu giả giọng nói        |
| fileOut   |Dữ liệu giọng nói đã được tái tạo|
| fileBufftx|Dữ liệu bit truyền đi |
| fileBuffrx|Dữ liệu bit nhận diện được bên receive|

Cấu trúc buf trong 1 frame: buf[14] kiểu dữ liệu char

*Bên tranmisster* 
* buf[0]-> buf[10] lưu 81 bit dữ liệu (buf[10] chỉ dùng 1 bit LSB).
* buf[11] dùng 1 bit LSB cho biết frame đó có phải frame VAD hay không.
* buf[12] + [13] dùng để lưu số thứ tự frame bên tx.

*Bên receiver* 
* buf [0]-> buf [10] lưu 81 bit dữ liệu (buf 10 chỉ dùng 1 bit LSB).
* buf [11] dùng 1 bit LSB cho biết frame đó có phải frame VAD hay không, những bit còn lại dùng để báo đồng bộ (bit 7 dùng đồng bộ frame).
* buf [12] + 7 bit của buf[13] dùng để lưu số thứ tự frame bên rx.
* buf [13] dùng 1 bit cuối để báo tìm được đúng số thự tự frame ứng với bên tx.

Trong thư mục matlab có file `checkBER.m` dùng để check BER giữa bit ở truyền đi và bit giải mã được (Điền các đường dẫn thư mục).
